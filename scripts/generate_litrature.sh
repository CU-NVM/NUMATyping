#!/bin/bash
set -euo pipefail

mkdir -p ~/literature
cd ~/literature

echo "📚 Downloading source texts..."
urls=(
  "https://www.gutenberg.org/files/2600/2600-0.txt"  # War and Peace
  "https://www.gutenberg.org/files/135/135-0.txt"    # Les Misérables
  "https://www.gutenberg.org/files/996/996-0.txt"    # Don Quixote
  "https://www.gutenberg.org/files/2701/2701-0.txt"  # Moby Dick
  "https://www.gutenberg.org/files/1400/1400-0.txt"  # Great Expectations
  "https://www.gutenberg.org/files/43/43-0.txt"      # Jane Eyre
  "https://www.gutenberg.org/files/84/84-0.txt"      # Frankenstein
  "https://www.gutenberg.org/files/345/345-0.txt"    # Dracula
  "https://www.gutenberg.org/files/1661/1661-0.txt"  # Sherlock Holmes
  "https://www.gutenberg.org/files/120/120-0.txt"    # Treasure Island
)

for u in "${urls[@]}"; do
  f=$(basename "$u")
  [ -f "$f" ] || wget -q "$u" -O "$f"
done

echo "🧩 Combining into seed..."
cat *.txt > seed.txt

# ---- build 1 GB safely ----
target=$((1024 * 1024 * 1024))   # 1 GiB
outfile="literature.txt"
cp seed.txt "$outfile"

seed_bytes=$(stat -c%s seed.txt)
current=$(stat -c%s "$outfile")

echo "📈 Expanding to ~1 GB..."
while [ "$current" -lt "$target" ]; do
  remain=$((target - current))
  if [ "$remain" -ge "$seed_bytes" ]; then
    cat seed.txt >> "$outfile"
    current=$((current + seed_bytes))
  else
    dd if=seed.txt bs=1 count="$remain" status=none >> "$outfile"
    break
  fi
done

echo "🎉 Done: $(pwd)/$outfile"
du -h "$outfile"
