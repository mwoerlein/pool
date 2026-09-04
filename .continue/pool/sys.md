# POOL Standard Library (`sys`) & Core Concepts

Overview of fundamental namespaces, core classes, streams, and design conventions in the **POOL** standard library (`sys`).

---

## 1. Core Runtime & Object Model (`sys::core`)

* **`sys::core::Object`**: The root base class for all objects in POOL. Provides lifecycle management methods (such as `destroy`, `own`, etc.).
* **Runtime & Instantiation**: Objects are instantiated via the runtime (`this.rt().createInstance(...)`) or via helper wrappers like `this.createOwn(...)` to ensure proper ownership tracking and memory management.
* **Primitive Wrappers**: While POOL relies heavily on native primitives (`int`, `cstring`) for performance, it provides full object wrappers for object-oriented completeness (`sys::core::Integer`, `sys::core::Bool`, `sys::core::Float`, `sys::core::String`).
* **`Flt`**: Static helper class for `float32` operations (conversions, math functions like `sqrt`, `sin`, `cos`, `log`, `floor`, comparison methods like `compare`).

---

## 2. Input/Output & Streams (`sys::stream`)

POOL uses stream abstractions for reading and writing data without depending on external libraries.

* **`OStream`**: The output stream interface/class used for printing text, numbers, objects, and vectors.
  * Usage: `out.printCString("Hello").printNewline();`
* **`IStream` / `SeekableIOStream`**: Used for reading input streams and files (e.g., loading `.safetensors` weight files).

---

## 3. Printable Protocol & `printToStream` Convention

To enable seamless printing of complex objects (such as `FltVector` or `FltMatrix`) directly via an output stream:
* Classes implement the `printToStream(OStream stream)` method.
* This allows callers to write `out.print(myObject);`, which automatically invokes the object's `printToStream` implementation to output its representation (e.g., `[0, 1.1, 2.2, ...]`).

---

## 4. Utilities (`sys::core::utils` / `sys::math`)

* **`Bitset` (`sys::core::utils::Bitset`)**: Provides bitwise operations (shifts, `and`, `or`, etc.) on native `int` values.
* **`FltVector` & `FltMatrix` (`sys::core::math`)**: High-performance 1D and 2D arrays of `float32` values, stored flat in memory, optimized for mathematical operations (matrix multiplication, dot product, normalizations, etc.).
* **Collections & Iterators**: When working with sequential collections (e.g., `AnyList`), prefer using iterators (`AnyIterator`) for linear $O(N)$ sequential processing instead of index-based lookups (`at(idx)`), which scale quadratically $O(N^2)$ on linked structures.
