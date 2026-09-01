# Documentation Index: POOL (Language, OOP & Memory)

Overview of the documented aspects of the **POOL** programming language. Note that these files complement each other cleanly without overlapping:

* **[syntax.md](syntax.md)**:
  * Program structure (`namespace`, `use`, `class`, `struct`)
  * Base types (`int`, `cstring`, `__all__`, `__any__`)
  * Expressions (literals, variables, arithmetic & logical operations)
  * Instructions (assignments, loops, conditions, inline PASM)

* **[oop.md](oop.md)**:
  * Class and struct declarations including inheritance (`extends`)
  * Instantiation, lifecycle & runtime (`Runtime.createInstance`, `destroyInstance`)
  * Initialization convention (`_init`, method chaining)
  * Destructors (`__destruct()`) and object ownership (`owner`, `_owned_entry`, `_owned_prev`, `_owned_next`)
  * Method modifiers (`global`, `abstract`, `__entry__`) and low-level code (`__naked__`, `__pasm__`)

* **[memory.md](memory.md)**:
  * Low-level memory management without automatic garbage collection
  * `MemoryManager` & `Allocator` interface
  * Memory lists (`available`, `free`, `used`, `reserved`)
  * Defragmentation & optimization (`optimize`, `OPTIMIZE_THRESHOLD`)
  * `MemoryInfo` and distinction from object-level ownership (`oop.md`)
