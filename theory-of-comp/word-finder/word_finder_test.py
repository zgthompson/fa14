from word_finder import WordFinder

finder = WordFinder()

maxCount = 0
maxWord = ""

for word in finder.dictionary:

    count = finder.totalWordsSimilarTo(word)

    if count > maxCount:
        maxCount = count
        maxWord = word

print maxWord
print maxCount




