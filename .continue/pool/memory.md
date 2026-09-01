# Memory Management & Memory Manager in POOL - Technical Documentation

This documentation is based on the analysis of `sys::memory::MemoryManager.pool`, `Allocator.pool`, and related memory classes under `bootstrap-pool/src/sys/memory/`.

---

## 1. Core Principle & No Automatic Garbage Collection

* **No automatic GC**: In POOL, there is **no automatic garbage collection** for dead instances. Memory management must be handled consciously and explicitly in the code.
* **Allocator Interface**: Memory operations (allocation and deallocation) are based on the `Allocator` interface, whose main implementation is the `MemoryManager`.

---

## 2. Core Components of the `MemoryManager`

The `MemoryManager` manages memory in various doubly linked lists of `MemoryInfo` blocks:
1. **`available`**: List of currently available, free memory regions from which new requests can be satisfied.
2. **`free`**: List of freed memory blocks collected before reuse and sorted / merged into the `available` list using a **merge sort optimization mechanism** (`optimize()`) once a certain threshold (`OPTIMIZE_THRESHOLD`) is reached.
3. **`used`**: List of currently allocated memory regions.
4. **`reserved`**: Reserved memory regions.

---

## 3. Allocation and Deallocation (`allocate` & `free`)

* **Allocation (`allocate(int size)`)**:
  * Calculates the required memory including metadata (`required = size + MemoryInfo:SIZEOF`).
  * Searches for a matching free block via `_findInfoByLen`.
  * Splits blocks (`_splitAvailableMemoryInfo`) if they are larger than needed (either from top or via `_allocateFromBottom`).
  * Moves the block into the `used` list.
* **Deallocation (`free(MemoryInfo info)`)**:
  * Marks the block as `FLAGS_AVAILABLE`.
  * Removes it from the `used` list and appends it to the `free` list (`freeCounter++`).
  * As soon as `freeCounter` reaches the threshold (`OPTIMIZE_THRESHOLD := 0x1000`), `optimize()` performs defragmentation, sorts free blocks (`_sortElements`), and merges adjacent memory regions.

---

## 4. Memory Metadata (`MemoryInfo`)
* **`MemoryInfo`**: Every allocated memory piece is accompanied by a `MemoryInfo` structure that stores metadata such as buffer address (`buf`), length (`len`), and flags (`flags`, e.g., `FLAGS_USED`, `FLAGS_AVAILABLE`).
* **`owner` field in `MemoryInfo`**: Intended for future low-level memory allocations occurring outside normal object instances (not actively used yet; the object-based ownership system runs via the `Object` class itself instead, see `oop.md`).

---

## 5. Object-Level Ownership (Owned List)
*(Note: The object-based ownership system is implemented in `sys::core::Object` – see `oop.md`)*.
* Every object can take other instances into its owned list via `own(obj)` or `createOwn(name)`.
* When an owner is destroyed (`destroy()`), all subordinate objects are automatically co-destroyed (`__destructOwned()`) via the doubly linked list (`_owned_entry`, `_owned_prev`, `_owned_next`), preventing manual memory leaks.
