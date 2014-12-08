from tag_systems import *
import itertools
"""
for startTag in ["".join(seq) for seq in itertools.product("01", repeat=3)]:
    for tag in tags(startTag):
        print tag
    print "-" * 10
"""

startTag = "0101010101"
for count, tag in enumerate(tags(startTag)):
    print "%2d %30s, len: %2d" % (count + 1, tag, len(tag))
print "-" * 15




