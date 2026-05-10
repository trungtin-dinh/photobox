from pathlib import Path

OLD_PREFIX = "/home/vm/M2SIA-projet-QT/DATA/Images/"
NEW_PREFIX = "./../Images/"

root = Path(".")
json_files = list(root.rglob("*.json"))

changed = 0

for json_file in json_files:
    text = json_file.read_text(encoding="utf-8")

    if OLD_PREFIX not in text:
        continue

    new_text = text.replace(OLD_PREFIX, NEW_PREFIX)

    # backup original file
    backup_file = json_file.with_suffix(json_file.suffix + ".bak")
    backup_file.write_text(text, encoding="utf-8")

    json_file.write_text(new_text, encoding="utf-8")
    changed += 1

    print(f"Updated: {json_file}")

print(f"\nDone. Updated {changed} JSON file(s).")
