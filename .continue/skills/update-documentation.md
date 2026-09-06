# Skill: Update Documentation and Analyze Codebase

Execute the following steps to review and transfer new insights from `.continue/lessons_learned/` and actively analyze/restructure the existing project documentation under `.continue/`.

*(Ensure compliance with `.continue/rules/documentation.md` regarding language and style).*

## Workflow

1. **Read and Analyze**:
   - Scan the `.continue/lessons_learned/` directory for raw lesson files.
   - **Explicit Content Verification**: Read the full content of *every* file found in `.continue/lessons_learned/` using the read tool. Never assume file contents based on past sessions, filenames, or previous assumptions.
   - **Comprehensive Coverage Check**: Perform an itemized coverage check of *all* distinct sub-points across every lesson file to ensure 100% of the lessons are fully processed without omissions.
   - **Pre-Check for Duplicates**: Search existing `.continue/` documentation files before proposing changes to ensure uniqueness and prevent duplicate or conflicting entries.
   - Review the existing project documentation under `.continue/` (such as `.continue/overview.md`, `.continue/pool/`, `.continue/pasm/`, etc.) to identify out-of-date sections, inconsistencies, architectural drift, or opportunities for structural optimization and consolidation.

2. **Propose Changes & Await Approval**:
   - Present a clear, concrete proposal (including exact text adjustments rather than vague summaries) to the developer specifying:
     - Which insights from `lessons_learned/` (if any) to integrate and where (backed by the itemized coverage check).
     - Which structural optimizations, updates, or refactoring of existing documentation under `.continue/` are recommended based on codebase and documentation analysis.
     - Which insights are too specific/temporary and should be discarded.
   - **Stop and wait for developer approval** before making any modifications.

3. **Apply Changes & Summarize**:
   - Upon approval, update, restructure, or create the documentation files under `.continue/` following the project's documentation rules.
   - Provide a concise summary of the changes made.

4. **Review & Commit by Developer**:
   - **Stop here** and prompt the developer to review the changes and create the commit.

5. **Clean up Source Files**:
   - Once the developer has confirmed the commit, **delete** any processed source files from `.continue/lessons_learned/`.
