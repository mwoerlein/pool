#!/usr/bin/env python3
import os
import json
import glob
import sys

def main():
    sessions_dir = os.path.expanduser("~/.continue/sessions")
    if not os.path.exists(sessions_dir):
        print(f"Sessions directory not found: {sessions_dir}")
        sys.exit(1)

    pattern = os.path.join(sessions_dir, "*.json")
    files = glob.glob(pattern)
    print(f"Found {len(files)} session files in {sessions_dir}\n")

    files.sort(key=os.path.getmtime, reverse=True)

    for filepath in files[:10]:
        filename = os.path.basename(filepath)
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                data = json.load(f)
            
            title = data.get("title", "No Title")
            history = data.get("history", [])
            
            print(f"=== Session: {filename} ===")
            print(f"Title: {title}")
            print(f"Messages count: {len(history)}")
            
            user_messages = 0
            for h in history:
                role = h.get("role")
                msg = h.get("message", {})
                content = msg.get("content", "") if isinstance(msg, dict) else str(msg)
                
                if role == "user" or (isinstance(msg, dict) and msg.get("role") == "user"):
                    user_messages += 1
                    preview = content.replace("\n", " ")[:120]
                    print(f"  [User] {preview}...")
            
            print(f"Total user messages found: {user_messages}\n")
        except Exception as e:
            print(f"Error reading {filename}: {e}\n")

if __name__ == "__main__":
    main()
