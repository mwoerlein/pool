---
invokable: false
---

# Coding & Testing Standards

1. **POOL Language Syntax & Structure**: Ensure correct syntax, namespaces (`namespace ...;`), imports (`use ...;`), type declarations, and class structures (`class ... extends ...`).
2. **Parser & Lexer Consistency**: Check that changes to `.gram` grammar files correctly align with generated parser/tokenizer expectations and ensure `make regenerate-parsers` is executed when grammar changes occur.
3. **Memory Management & Object Lifecycle**: Proper object allocation (`createOwn`) and explicit iterator destruction (`it.destroy()`) enclosed within dedicated lexical blocks (`{ AnyIterator it = ...; ... it.destroy(); }`) must be enforced to prevent memory leaks.
4. **Error Handling & Logging**: Verify that compiler errors (`compiler.hasErrors()`) and logging (`Logger`, `LoggerAware`) are handled appropriately and proper error codes are returned on failure.
5. **Architecture & Assembly Bindings**: When working with architecture-specific code (`linux::elf`, `pool::asm`, `.pasm`), ensure correct register usage, binary output formatting, and memory offsets.
6. **Test Architecture & Patterns**: Test classes should use structured test cases (`startCases`, `case...`, `successCases`) combined with base test helpers instead of manual inline allocations or monolithic assertions, and ensure the `pooltest` suite passes successfully (`make tests`).
7. **Method Signatures & Formatting**: Preserve exact method signature formatting (such as spacing conventions in parameter lists) during edits.
