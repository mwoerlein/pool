# POOL Language - Technical Documentation & Syntax

This documentation describes the grammar, structure, and syntax of the **POOL** programming language (based on the analysis of `bootstrap-pool/src/pool/compiler/parser/Parser.gram` and associated test and source files).

---

## 1. Program Structure & Translation Unit

A POOL source file consists of optional namespace declarations, use statements, and class and struct declarations.

```pool
namespace my::project;

use other::module;
use external::Class as Alias;

class MyClass extends BaseClass {
    // Variables and methods
}

struct MyStruct {
    int field;
}
```

---

## 2. Data Types & Primitive Types (`int` and `cstring`)

POOL features a very minimalist and rudimentary type system with two core primitive types that map directly to low-level machine representations:

### A) `int` (Universal 32-Bit Type)
The `int` type is a native 32-bit integer serving multiple roles across the language:
* **Integers**: Standard whole numbers (`int x = 42;`)
* **Booleans / Truth Values**: `0` represents `false`, while any non-zero value represents `true`.
* **Memory Addresses & Pointers**: Representing object references, raw memory addresses (`sys::memory::Address`), and pointers.
* **Float32 Values**: Floating-point numbers are stored as raw 32-bit bit patterns inside standard `int` variables. 
*(For specialized math, bit operations, and float helper methods like `Flt`, see [sys.md](sys.md)).*

### B) `cstring` (C-Style Null-Terminated String Pointer)
The `cstring` type is the second primitive type with direct language support. 
* Under the hood, because all variables are 32-bit numbers (either an `int` or a 32-bit object pointer), a `cstring` is effectively a **pointer** pointing directly to a memory location containing a `\0` (null)-terminated sequence of characters.
* **String Literals**: Any string literal enclosed in double quotes (`"..."`) in the source code is placed globally into the binary during compilation, and its usage is automatically replaced with a pointer (`cstring`) pointing directly to that address in memory.

### Object Variants vs. Primitives & Performance
Despite POOL aiming to be a **"Pure Object-Oriented"** language (and offering full object wrappers like `sys::core::Integer`, `sys::core::Bool`, `sys::core::Float`, and `sys::core::String`), numeric and string computations using instance method calls on these objects introduce significant performance overhead compared to native primitives and static helper methods (e.g. `Flt`). Consequently, the language pragmatically moves away from strict object wrapping for heavy math and primitive workloads in favor of native `int` / `cstring` usage.

---

## 3. Code Conventions

To keep the codebase clean and consistent, adhere to the following naming and structuring conventions:
* **Variables & Parameters**: Instance variables, method parameters, and local/helper variables are written in `lowerCamelCase` (e.g. `promptTokens`, `maxNewTokens`).
* **Constants**: Constants are written in `UPPER_SNAKE_CASE` (e.g. `MAX_SIZE`).
* **Classes & Structs**: Class and struct names are written in `UpperCamelCase` (e.g. `GenerationLoop`, `LlamaModel`).
* **Namespaces**: Namespace segments are completely lowercase (`lower`), consisting of a single word or a concise abbreviation (e.g. `llm`, `sys::core`, `llm::model`).

---

## 4. Classes and Structs

### Classes (`class_decl`)
Classes can contain variables (instance variables, global variables) and methods. They support inheritance via `extends type_list`.
```pool
class Calculator extends Base {
    int value;
    global int instanceCount;

    [int] add(int a, int b) {
        return a + b;
    }
}
```

### Structs (`struct_decl`)
Structs define data structures without methods or with limited initialization instances.
```pool
struct Point {
    int x;
    int y;
}
```

---

## 5. Method Declarations

Methods have a special syntax with return types in square brackets before the method name:

* **Standard Method**: `[type_list] name(parameters) { body }`
* **Global Method**: `global [type_list] name(parameters) { body }`
* **Entry Point**: `__entry__ [type_list] name(parameters) { body }`
* **Naked Method**: `__naked__ [type_list] name(parameters) { body }`
* **Abstract Method**: `abstract [type_list] name(parameters);`

Example:
```pool
[int, cstring] process(int code) {
    return [code, "OK"];
}
```

---

## 6. Expressions

* **Values**: `true`, `false`, integers (`NUMBER`, incl. hex, octal, binary, char literals), strings (`SLSTRING`, `MLSTRING`), `this`, `null`.
* **Variables**: Local variables, member access (`expr.ID`), static access (`class_ref::ID`).
* **Method Calls**: `name(args)`, `expr.method(args)`, `class_ref::method(args)`.
* **Arithmetic**: `+`, `-`, `*`, `/`, `%`, pre-/post-increment/decrement (`++`, `--`), assignments `=`, `+=`, `-=`, `*=`, `/=`, `%=`.
* **Logic & Comparison**: `!`, `&&`, `||`, `==`, `!=`, `<`, `<=`, `>`, `>=`.

---

## 7. Instructions

* **Expression as Statement**: `expression;`
* **Variable Initialization**: 
  * `int x = 5;` (mutable)
  * `int x := 5;` (final / immutable)
  * Multiple assignment: `[int a, int b] = calculate();` or `[int a, int b] := calculate();`
* **Return**: `return;`, `return expr;`, or `return [expr1, expr2];`
* **Control Structures**:
  * `if (expr) { ... } else { ... }` (incl. `else if` cascades)
  * `while (expr) { ... }`
  * `do { ... } while (expr);`
  * **Note on Ternary Operators**: POOL does **not** support a ternary conditional operator (`? :`). Conditional assignments must be written explicitly using standard `if / else` statements.
* **Inline PASM**: Direct assembler code inside POOL code:
  ```pool
  __pasm__("mov %eax, %ebx", { "ebx": inputVal }, { "eax": outputVal });
  ```
