# Skill: Update Documentation from Lessons Learned

Execute the following steps to transfer new insights from `.continue/lessons_learned/` into the general project documentation under `.continue/`.

*(Ensure compliance with `.continue/rules/documentation.md` regarding language and style).*

## Workflow

1. **Read and Analyze**:
   - Scan the `.continue/lessons_learned/` directory for raw lesson files and analyze their content.

2. **Propose Changes & Await Approval**:
   - Present a clear proposal to the developer specifying:
     - Which insights to integrate and where (including updating/restructuring existing documentation files or proposing new ones when justified).
     - Which insights are too specific/temporary and should be discarded.
   - **Stop and wait for developer approval** before making any modifications.

3. **Apply Changes & Summarize**:
   - Upon approval, update, restructure, or create the documentation files under `.continue/` following the project's documentation rules.
   - Provide a concise summary of the changes made.

4. **Review & Commit by Developer**:
   - **Stop here** and prompt the developer to review the changes and create the commit.

5. **Clean up Source Files**:
   - Once the developer has confirmed the commit, **delete** the processed source files from `.continue/lessons_learned/`.
