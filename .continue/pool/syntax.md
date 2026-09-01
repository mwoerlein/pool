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

## 2. Data Types

POOL supports the following base types and references:
* `int` (Integer)
* `cstring` (C-String / string type)
* `__all__` / `__any__` (Generic / universal types)
* User-defined class types (`class_ref`, e.g., `my::project::Class` or local class names).

---

## 3. Classes and Structs

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

## 4. Method Declarations

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

## 5. Expressions

* **Values**: `true`, `false`, integers (`NUMBER`, incl. hex, octal, binary, char literals), strings (`SLSTRING`, `MLSTRING`), `this`, `null`.
* **Variables**: Local variables, member access (`expr.ID`), static access (`class_ref::ID`).
* **Method Calls**: `name(args)`, `expr.method(args)`, `class_ref::method(args)`.
* **Arithmetic**: `+`, `-`, `*`, `/`, `%`, pre-/post-increment/decrement (`++`, `--`), assignments `=`, `+=`, `-=`, `*=`, `/=`, `%=`.
* **Logic & Comparison**: `!`, `&&`, `||`, `==`, `!=`, `<`, `<=`, `>`, `>=`.

---

## 6. Instructions

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
* **Inline PASM**: Direct assembler code inside POOL code:
  ```pool
  __pasm__("mov %eax, %ebx", { "ebx": inputVal }, { "eax": outputVal });
  ```
