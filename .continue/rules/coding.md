---
invokable: false
---

# Coding & Testing Standards

1. **Iterator Scoping & Lifecycle**: Iterators instantiated via `.iterator()` must always be explicitly destroyed (`it.destroy()`). Encapsulate them within dedicated lexical blocks (`{ AnyIterator it = ...; ... it.destroy(); }`) to prevent memory leaks.
2. **Test Architecture & Patterns**: Test classes should use structured test cases (`startCases`, `case...`, `successCases`) combined with base test helpers instead of manual inline allocations or monolithic assertions.
3. **Method Signatures & Formatting**: Preserve exact method signature formatting (such as spacing conventions in parameter lists) during edits.
