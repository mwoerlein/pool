# Object-Oriented Programming & Classes in POOL - Technical Documentation

This documentation is based exclusively on the source and test files under `bootstrap-pool/src/` and `bootstrap-pool/tests/` (especially `Runtime.pool`, `CompilerTest.pool`, `Parser.gram`, and AST declarations).

---

## 1. Class and Struct Declarations

### Classes (`class_decl`)
* **Syntax**:
  ```pool
  class ClassName extends BaseClass1, BaseClass2 {
      // Global variables / constants
      global int CONST := 1;
      int a := 1;
      int b = 1;

      // Instance variables
      OtherClass obj;
      __any__ anyField;
      __all__ allField;

      // Methods
      [int] myMethod(int param) {
          return param;
      }
  }
  ```
* **Inheritance**: Classes can inherit from multiple base classes / interfaces, specified via `extends type_list` (comma-separated).

### Structs (`struct_decl`)
* Structs contain data fields and global initializations, but no methods.
  ```pool
  struct Point {
      global int DEFAULT_X := 0;
      int x;
      int y;
  }
  ```

---

## 2. Instantiation, Lifecycle & Memory Management

In POOL, object instances are not created using a classic `new` keyword, but are managed via the runtime system class `sys::runtime::Runtime`. Since there is **no automatic garbage collection** for dead instances, memory must be managed explicitly.

### Creation and Destruction
* **Create Instance**:
  ```pool
  A a := rt.createInstance(A:CLASSNAME);
  // or with direct cast:
  Task task = rt.createInstanceAs(name, Task:CLASSNAME);
  ```
* **Destroy Instance**:
  ```pool
  rt.destroyInstance(a);
  ```
* **Background**: `Runtime.createInstance` allocates memory via the allocator (`MemoryInfo`), initializes the instance template, sets the runtime context, and automatically calls the parameterless `__init()` method. When destroyed via `destroyInstance` / `__destruct()`, the memory is released.

### Initialization Conventions (Constructors)
Since `__init()` cannot accept parameters, the following **code convention** is used for parameter-based initialization:
1. After raw instance creation, the creator explicitly calls an initialization method (e.g., `_init(...)` or specific variants like `_initFromA(...)`, `_initFromB(...)`).
2. **Modern Pattern (Method Chaining)**: As a newer code pattern, these initialization methods return the instance itself to enable method chaining directly upon creation:
   ```pool
   Point p := rt.createInstance(Point:CLASSNAME);
   p._init(1, 2).setVisible(true);
   ```

### Destruction, Destructors (`__destruct()`) & Ownership
Analogous to `__init()`, there is a `__destruct()` method where an instance can clean up before its memory is freed.
* Since there is no automatic garbage collection, this is essential for memory management.
* **Ownership / Owned List**: Every `Object` has instance variables for the ownership relationship:
  * `owner`: Points to the owning object.
  * `_owned_entry`, `_owned_prev`, `_owned_next`: Form a doubly linked list of child instances subordinate ("owned") to the object.
* **Automatic Co-Destruction**: When an object is destroyed (via `destroy()`), all child objects registered in its `_owned` list are automatically and iteratively destroyed (`__destructOwned()`), preventing memory leaks in complex object graphs.
* **Collection & Temporary Memory Guidelines (`AnyList` / Custom Collections)**:
  * **Returned Temporary Collections**: When helper methods (such as `toAnyList`) construct temporary collections (e.g. `AnyList`, `TokenList`) as return values, the collection itself must **not** be bound to `this` via `createOwn(...)` (otherwise returning it creates a memory hole/leak or double-free). Instantiate it directly via `this.rt().createInstance(AnyList:CLASSNAME)`.
  * **Child Elements in Collections**: Items stored inside custom collections (like `Integer` wrappers) should be allocated directly through the collection owner using `list.createOwn(...)` so the collection cleans them up upon destruction.
  * **Linear Iteration vs. Direct Access**: For sequential collection traversals (e.g., in Embedding or Token processing layers), always use `AnyIterator` (`list.iterator()`) to guarantee `O(N)` linear performance instead of index-based `at(idx)` access (`O(N²)`, quadratic overhead).
* **Ownership Caution in Test Cases & Factories**: Helper methods or test cases returning temporary/constructed instances (such as collections or data objects) should **not** bind these return values to `this` via `createOwn(...)`. Binding them to `this` (e.g., a test case instance) when they are meant to be returned or managed by the caller leads to ownership leaks or double-frees. Instead, instantiate them via `rt.createInstance(...)` or ensure ownership is correctly transferred to the recipient.
* *(Note on MemoryInfo Owner: The `owner` field in the `MemoryInfo` structure is separate and intended for future low-level memory allocations outside of normal object instances).*

---

## 3. Method Modifiers & Signatures

Methods support various qualifiers before the return type declaration (in square brackets `[...]`):

* **Standard Method**: `[type] name(...) { ... }`
* **Multiple Return**: `[int, int] method(...) { ... }` (Returned as an array/tuple via `return [val1, val2];`)
* **Global Method** (`global`): Static methods or global functions.
* **Abstract Method** (`abstract`): Without method body, ending with a semicolon `;`.
* **Entry Point** (`__entry__`): Marks the program entry point (e.g., `__entry__ [int] entry(int argc, int argv)`).

---

## 4. Variables, Initialization & Immutability

* **Mutable Variables** (`=`):
  ```pool
  int x = 10;
  ```
* **Final / Immutable Variables** (`:=`):
  ```pool
  int x := 5;
  ```
* **Multiple Initialization / Destructuring**:
  ```pool
  [int x, int y, int z] := my::B:numbers(s, p);
  [int l, int m, int n] = b.numbers(x, y);
  ```

---

## 5. Low-Level & Machine-Level Code (Inline-PASM & `__naked__`)

For low-level code and assembler-level workarounds, POOL provides two mechanisms:

### Inline-PASM (`__pasm__`)
Allows embedding assembler code within methods, including explicit mapping of input and output variables:
```pool
__pasm__(<"
    addl %eax, %ecx
    addl %ebx, %ecx
">, {"%eax": this.a, "%ebx": b, "%ecx": i}, {"%ecx": i});
```

### Naked Methods (`__naked__`)
A workaround where a method is declared as `__naked__` to bypass the generated function prologue/epilogue and execute raw assembler code (typically via `__pasm__`):
```pool
__naked__ [] halt_loop() {
    __pasm__(<"
    halt_loop: hlt; jmp halt_loop
    ">, {}, {});
}
```
