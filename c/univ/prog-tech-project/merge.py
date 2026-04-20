import os
import re
import shutil

WORK_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "work")
SRC_DIR  = os.path.join(os.path.dirname(os.path.abspath(__file__)), "src")

HEADERS = [
    "lib/collections/vector/vector.h",
    "lib/collections/hash/hash_key_functions.h",
    "lib/collections/hash/hashtable/hashmap_entry.h",
    "lib/collections/hash/hashtable/hashmap.h",
    "lib/collections/hash/hashset/hashset.h",
    "src/utils/hash_functions/hash_functions.h",
    "src/utils/file_utils/file_utils.h",
    "src/file_paths/file_paths.h",
]

SOURCES = [
    "lib/collections/vector/vector.c",
    "lib/collections/hash/hashtable/hashmap_entry.c",
    "lib/collections/hash/hashtable/hashmap.c",
    "lib/collections/hash/hashset/hashset.c",
    "src/utils/hash_functions/hash_functions.c",
    "src/utils/file_utils/file_utils.c",
    "src/file_paths/file_paths.c",
    "src/filededup.c",
]

LOCAL_INCLUDE  = re.compile(r'^\s*#include\s+"[^"]+"\s*\n', re.MULTILINE)
SYSTEM_INCLUDE = re.compile(r"^\s*(#include\s+<[^>]+>)\s*\n", re.MULTILINE)


def read(rel_path):
    with open(os.path.join(WORK_DIR, rel_path), encoding="utf-8") as f:
        return f.read()


def collect_system_includes(rel_paths):
    seen = set()
    result = []
    for p in rel_paths:
        for m in SYSTEM_INCLUDE.finditer(read(p)):
            inc = m.group(1).strip()
            if inc not in seen:
                seen.add(inc)
                result.append(inc)
    return result


def strip_includes(content):
    content = LOCAL_INCLUDE.sub("", content)
    content = SYSTEM_INCLUDE.sub("", content)
    return content


def main():
    os.makedirs(SRC_DIR, exist_ok=True)

    shutil.copy(
        os.path.join(WORK_DIR, "src", "filededup.h"),
        os.path.join(SRC_DIR, "filededup.h"),
    )
    print("Copied -> src/filededup.h")

    all_files = HEADERS + SOURCES
    system_includes = collect_system_includes(all_files)

    parts = []
    parts.append("/* Auto-generated - do not edit manually. */\n\n")
    parts.append("\n".join(system_includes) + "\n\n")
    parts.append("#include \"filededup.h\"\n")

    for rel_path in HEADERS:
        content = strip_includes(read(rel_path))
        parts.append(f"\n/* ===== {rel_path} ===== */\n\n")
        parts.append(content.strip() + "\n")

    for rel_path in SOURCES:
        content = strip_includes(read(rel_path))
        parts.append(f"\n/* ===== {rel_path} ===== */\n\n")
        parts.append(content.strip() + "\n")

    out_path = os.path.join(SRC_DIR, "filededup.c")
    with open(out_path, "w", encoding="utf-8") as f:
        f.write("".join(parts))
    print("Created -> src/filededup.c")


if __name__ == "__main__": main()
