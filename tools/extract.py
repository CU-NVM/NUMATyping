import mwxml
import bz2

dump_path = "enwiki-latest-pages-articles.xml.bz2"

with bz2.open(dump_path, "rb") as f:
    dump = mwxml.Dump.from_file(f)
    with open("wikipedia_text.txt", "w", encoding="utf-8") as out:
        for page in dump:
            for revision in page:
                text = revision.text or ""
                out.write(text + "\n")