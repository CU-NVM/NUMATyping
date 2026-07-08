input_file  = "wikipedia_text.txt"
output_file = "wikipedia_ascii.txt"

with open(input_file, "r", encoding="utf-8", errors="ignore") as fin, \
     open(output_file, "w", encoding="ascii", errors="ignore") as fout:
    while True:
        chunk = fin.read(1024 * 1024)  # 1 MB chunks
        if not chunk:
            break
        # Keep only ASCII range (codepoints < 128)
        cleaned = ''.join(c for c in chunk if ord(c) < 128)
        fout.write(cleaned)