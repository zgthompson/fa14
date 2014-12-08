from word_finder import WordFinder

finder = WordFinder()

LETTERS = 6

wordCount = 0
wordMaxNeighborCount = 0
wordMax = None
wordZeroNeighborCount = 0

for word in finder.dictionary:

    if len(word) != LETTERS:
        continue

    wordCount += 1

    count = finder.totalWordsSimilarTo(word)

    if count == 0:
        print word
        wordZeroNeighborCount += 1

    if wordMaxNeighborCount < count:
        wordMaxNeighborCount = count
        wordMax = [ word ]
    elif wordMaxNeighborCount == count:
        wordMax.append(word)

print "There are %d %d letter words" % (wordCount, LETTERS)
print "%s each have %d neighbors" %(str(wordMax), wordMaxNeighborCount)
print "There are %d words with no neighbors" % wordZeroNeighborCount
print finder.wordsSimilarTo(wordMax[0])
