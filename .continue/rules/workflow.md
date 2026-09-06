---
invokable: false
---

# Workflow & Collaboration Rules

1. **Proactive Communication & Proposal**: Propose concrete solutions and code changes directly rather than asking permission for every intermediate step.
2. **Session Closure & Lessons Learned**: At the end of a successful session, suggest capturing insights using the `create-lessons-learned` skill.
3. **Roles & Execution**: Discuss code changes and architectural patterns in dialogue before applying modifications. The developer exclusively executes local builds and test suites.
4. **Commit Messages**: Keep commit messages clean and direct. Do not add automated tool annotations or `Co-authored-by` trailers unless explicitly requested.
5. **Skill Structure & Discovery**: Project-local Continue skills must reside in a dedicated subdirectory containing a `SKILL.md` file with proper YAML frontmatter (`name`, `description`, `invokable: true`). System/background rules use `invokable: false` in their frontmatter to automatically hook into sessions.
6. **Documentation Generalization**: When updating project documentation, distill concrete insights into generalized rules and rely on clean cross-referencing to prevent duplication or architectural drift.
7. **Automatic Staging of New Files**: Whenever new source files (`src/`), test files (`tests/`), or documentation/rule files (`.continue/`) are created by the agent, they must be automatically staged using `git add` to prevent untracked file clutter.
