---
name: create-lessons-learned
description: Create a new lessons learned record based on session insights.
invokable: true
---

# Skill: Create Lessons Learned

Create a new lessons learned record based on the key insights and achievements of the current session.

## Workflow

1. **Review Session Context & Complete Capture**:
   - Analyze the entire conversation and code modifications made during the current session.
   - **Exhaustive Extraction**: Ensure that **all** relevant topics, communication observations, workflow nuances, syntax/memory rules, and architectural decisions are captured—avoiding rudimentary summaries or forgotten topics.

2. **Propose Summary & Filename**:
   - Determine an appropriate sequential filename (e.g., `04-topic-name.md`) under `.continue/lessons_learned/`.
   - Propose a comprehensive, well-structured outline of the content (in English, structured with clear headings covering all session dimensions).
   - **Stop and ask the developer for approval** before writing the file.

3. **Create File**:
   - Upon approval, create the new `.md` file under `.continue/lessons_learned/`.

4. **Summarize**:
   - Inform the developer that the lessons learned file has been created successfully.
