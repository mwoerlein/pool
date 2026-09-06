# Repository Overview

## Project Description
- **What this project does**: This repository contains the bootstrap implementation, compiler infrastructure, runtime, standard library, assembler, parser generator, and tools for the **POOL** programming language.
- **Main purpose and goals**: To provide a self-hosting compiler toolchain (`poolbc`, `poolbsc`, `pool`), assembler (`pbasm`), parser generator (`parsergen`), storage utility (`storeb`), language server (`pls`), and test runner (`pooltest`) written in POOL itself.
- **Key technologies used**: POOL programming language (`.pool`), custom assembly (`.pasm`), grammar parser definitions (`.gram`), C/Linux ELF binary bindings, and Makefiles for build automation.

## Architecture Overview
- **High-level architecture**: The project is structured around the POOL compiler toolchain pipeline: source code (`.pool`) -> Parser/Lexer generation -> AST Compilation (`poolbc` / `poolbsc`) -> Assembler (`pbasm`) -> Executable Linux ELF binaries or bytecode.
- **Main components and their relationships**:
    - `poolbsc` (`PoolStaticCompiler`): The bootstrap static compiler for compiling POOL code to Linux ELF binaries.
    - `poolbc` (`PoolCompiler`): Compiles POOL source files to assembly (`.pasm`) or binary format.
    - `parsergen` (`ParserGen`): Generates parsers and tokenizers from `.gram` grammar files.
    - `pbasm`: Assembler for compiling POOL assembly (`.pasm`) into machine code or binary structures.
    - `pls`: POOL Language Server for IDE integration.
    - `pooltest`: Comprehensive test suite runner.
- **Data flow and system interactions**: Source files are tokenized and parsed using generated parsers, compiled into intermediate representations or assembly, assembled into target binaries (`linux::elf`), and executed via the POOL runtime environment.

## Directory Structure
- **Important directories and their purposes**:
    - `bootstrap-pool/src/`: Core source code for the POOL language runtime, compiler, standard library, and tools (structured by namespaces):
        - `pool/`: Core compiler, assembler, generator, language server, and storage modules.
        - `linux/`: Linux-specific commands, ELF bindings, and static compiler (`PoolStaticCompiler.pool`).
        - `sys/`: System runtime, threads, TUI, and core utilities.
    - `bootstrap-pool/tests/`: Test cases and test suite command runner (`tests/test/TestSuite.pool`), mirroring the source namespace structure.
    - `bootstrap-pool/bin/`: Compiled tool binaries (generated during build).
- **Key files and configuration**:
    - `bootstrap-pool/Makefile`: Main build orchestrator for tools and test runners.
    - `bootstrap-pool/Makefile.inc`: Shared make variables (`POOLSC`, `PSC_FLAGS`, `SRCDIR`, `BINDIR`, etc.).
    - `.continueignore`, `.gitignore`: Version control exclusions.

## Development Workflow
- **How to build/run the project**:
    - Change to the working directory: `cd bootstrap-pool`
    - Build all tools: `make` (or `make all`)
    - Regenerate parsers from `.gram` files: `make regenerate-parsers`
    - Install binaries: `make install`
- **Testing approach**:
    - Run the test suite: `make tests` (or `make parser-tests`) from within `bootstrap-pool/`.
    - Uses `pooltest` executing test modules under `tests/`.
- **Collaborative Workflow & Role Separation**: 
    - **Roles**: Strict separation of roles during development. The agent discusses design and code snippets in dialogue before applying modifications. The developer executes builds, runs test suites locally, and creates commits.
    - **Commit Conventions**: Keep commit messages clean and direct. Do not add automated tool annotations or `Co-authored-by` trailers unless explicitly requested.
    - **Rule Loading & Discovery**: System/background rules use `invokable: false` in their frontmatter to automatically hook into sessions alongside root `AGENTS.md`.
    - **Documentation Updates**: Ensure 100% of insights from `lessons_learned/` are integrated without omissions, performing a pre-check to prevent duplicates.
- **Development environment setup**:
    - Requires a working `poolbsc` bootstrap compiler in the parent directory (`../bin/poolbsc` as referenced in `Makefile.inc`) and a Linux/ELF build environment.
- **Lint and format commands**:
    - Clean build artifacts: `make clean`
    - Clear test parsers: `make clear-tests`
