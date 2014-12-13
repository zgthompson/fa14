from tag_systems import *
import itertools

"""
results = []
for startTag in ["".join(seq) for seq in itertools.product("01", repeat=19)]:
    steps = 0
    for tag in tags(startTag):
        steps += 1
    results.append(steps)

print max(results)
"""


startTag = "101110111011"
tracker = {}
maxLength = 0
maxLengthGen = 0
lastTag = None
lastGen = None
for count, tag in enumerate(tags(startTag)):
    if maxLength < len(tag):
        maxLength = len(tag)
        maxLengthGen = count
    tracker[tag] = count
    lastTag = tag
    lastGen = count


print "Max length: %d" % maxLength
print "Max length generation: %d" % maxLengthGen
print lastGen
"""
print "Cycle start generation: %d" % tracker[repeat]
print "Cycle length: %d" % (lastGen - tracker[repeat])
"""





