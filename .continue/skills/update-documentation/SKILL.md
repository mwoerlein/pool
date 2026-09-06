---
name: update-documentation
description: Analyze codebase, lessons learned records, and recent Continue session logs to incrementally update and consolidate project documentation.
invokable: true
---

# Skill: Update Documentation and Analyze Codebase

Execute the following steps to review and transfer new insights from `.continue/lessons_learned/` and recent Continue sessions (using `.continue/skills/update-documentation/summarize_sessions.py`), and actively analyze/restructure existing project documentation under `.continue/`.

*(Ensure compliance with `.continue/rules/documentation.md` regarding language and style).*

## Workflow

1. **Read and Analyze**:
   - Check the `.continue/lessons_learned/` directory for raw lesson files.
   - Run or inspect recent session logs (e.g. via `python3 .continue/skills/update-documentation/summarize_sessions.py`) to capture recent architectural decisions, resolved issues, and insights from recent sessions.
   - **Explicit Content Verification**: Read the full content of *every* file found (lessons learned or selected session logs) using the read tool. Never assume file contents based on past sessions, filenames, or previous assumptions.
   - **Comprehensive Coverage Check**: Perform an itemized coverage check of *all* distinct sub-points across every source file to ensure 100% of the inputs are fully processed without omissions.
   - **Pre-Check for Duplicates**: Search existing `.continue/` documentation files before proposing changes to ensure uniqueness and prevent duplicate or conflicting entries.
   - Review the entire documentation hierarchy under `.continue/`—specifically covering **`.continue/overview.md`**, **`.continue/rules/`**, **`.continue/pool/`** (`overview.md`, `syntax.md`, `oop.md`, `memory.md`, `sys.md`), **`.continue/pasm/`** (`overview.md`, `syntax.md`), and **`.continue/projects/`**—to ensure complete coverage and prevent omissions.

2. **Propose Changes & Await Approval**:
   - Present a clear, concrete proposal (including exact text adjustments rather than vague summaries) to the developer specifying:
     - Which insights from `lessons_learned/` or recent sessions to integrate and where (backed by the itemized coverage check).
     - Which structural optimizations, updates, or refactoring of existing documentation under `.continue/` are recommended based on codebase and documentation analysis.
     - Which insights are too specific/temporary and should be discarded.
   - **Stop and wait for developer approval** before making any modifications.

3. **Apply Changes & Summarize**:
   - Upon approval, update, restructure, or create the documentation files under `.continue/` following the project's documentation rules.
   - Provide a concise summary of the changes made.

4. **Review & Commit by Developer**:
   - **Stop here** and prompt the developer to review the changes and create the commit.

5. **Clean up Source Files**:
   - Once the developer has confirmed the commit:
     - **Delete** any processed source files from `.continue/lessons_learned/` (if lessons learned were used).
