# PASM (POOL Assembler) - Technical Documentation & Syntax

This documentation describes the grammar, structure, and syntax of the **PASM** assembler (based on the analysis of `bootstrap-pool/src/pool/asm/Parser.gram` and associated source and test files).

---

## 1. Program Structure (`Compilation Unit`)

A PASM program consists of a sequence of lines that can contain labels, definitions, or assembler instructions. Separators between commands are semicolons (`;`) or newlines (`\n`).

```pasm
// Example PASM structure
GLOBAL_VAR := 42

entry_label:
    mov %eax, GLOBAL_VAR
    add %eax, 1
    ret
```

---

## 2. Tokens & Lexical Elements

* **IDENTIFIER**: `[\._a-zA-Z][_a-zA-Z0-9]*` (Identifiers, labels, symbols, e.g., `main`, `.text`, `my_label`)
* **REGISTER**: `%[a-zA-Z][_a-zA-Z0-9]*` (Processor registers, e.g., `%eax`, `%esp`)
* **CHAR**: Characters enclosed in single quotes, e.g., `'a'`
* **STRING**: Strings enclosed in double quotes, e.g., `"Hello"`
* **NUMBER**: Integers (decimal, octal `0...`, binary `0b...`, hexadecimal `0x...`, optional negative `-`).
* **OPERATOR**: `+`, `-`, `*`, `/`, `%`, `<<`, `>>`, `.`

---

## 3. Language Constructs & Rules

### Labels
A label defines a jump target in the code:
```pasm
label_name:
```

### Definitions
Constants or symbols can be defined using `:=`:
```pasm
BUFFER_SIZE := 1024
```

### Instructions
Instructions consist of a mnemonic command and up to three operands (comma-separated):
* `instruction` (0 operands)
* `instruction op1` (1 operand)
* `instruction op1, op2` (2 operands)
* `instruction op1, op2, op3` (3 operands)

---

## 4. Operand Types

Operands in PASM can be:
1. **Identifiers / Symbols**: `my_var`
2. **Registers**: `%eax`, `%ebx`
3. **Numbers**: `42`, `0xFF`
4. **Characters / Strings**: `'x'`, `"text"`
5. **Indirect Addressing (Memory Addressing)**:
   * `(base)`
   * `(base, index)`
   * `(base, index, scale)`
   * `(, index)` / `(, index, scale)`
   * `disp(base)`
   * `disp(base, index)`
   * `disp(base, index, scale)`
   * `disp(, index)` / `disp(, index, scale)`
6. **Formulas (Calculations in Operands)**:
   * `(operand OPERATOR operand)` (e.g., `(offset + 4)`)
