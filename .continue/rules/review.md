---
invokable: true
---

Review this code for potential issues, including:

1. **POOL Language Syntax & Grammar**: Ensure correct syntax, namespaces (`namespace ...;`), imports (`use ...;`), type declarations, and class structures (`class ... extends ...`).
2. **Parser & Lexer Consistency**: Check that changes to `.gram` grammar files correctly align with generated parser/tokenizer expectations and that `make regenerate-parsers` has been run if necessary.
3. **Memory Management & Object Lifecycle**: Look for proper object allocation (`createOwn`), iterator destruction (`it.destroy()`), and avoiding resource leaks in loops or error paths.
4. **Error Handling & Logging**: Verify that compiler errors (`compiler.hasErrors()`) and logging (`Logger`, `LoggerAware`) are handled appropriately and error codes are returned on failure.
5. **Linux / ELF / Assembly Binding**: When working with architecture-specific code (`linux::elf`, `pool::asm`, `.pasm`), ensure correct register usage, binary output formatting, and memory offsets.
6. **Testing**: Ensure new features or bug fixes have corresponding test cases in `tests/` and pass the `pooltest` suite (`make tests`).
7. **Collaborative Workflow & Verification**: 
   - Discuss code changes and snippets in dialogue before applying modifications.
   - Maintain clear separation of roles during testing (developer executes builds and runs test suites locally).
   - Capture insights and lessons learned at the end of sessions for continuous documentation improvement (invoking the `create-lessons-learned.md` skill).

Provide specific, actionable feedback for improvements.
