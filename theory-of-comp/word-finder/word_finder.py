from collections import defaultdict
import json
import os.path

DICTIONARY_FILE = "sowpods.txt"

class WordFinder(object):

    DICTIONARY_JSON = "dictionary.json"
    SIMILILAR_WORDS_JSON = "similar_words.json"

    def __init__(self):
        self.dictionary = self.loadDictionary()
        self.similarWords = self.loadSimilarWords()


    def loadDictionary(self):

        if not os.path.isfile( self.DICTIONARY_JSON ):
            WordFinder.generateJSON()

        return json.loads( open(self.DICTIONARY_JSON).read() )


    def loadSimilarWords(self):

        if not os.path.isfile( self.DICTIONARY_JSON ):
            WordFinder.generateJSON()

        return json.loads( open(self.SIMILILAR_WORDS_JSON).read() )


    def wordsSimilarTo(self, word):

        wordsSimilar = set()

        for matchKey in WordFinder.matchKeys(word):
            curMatches = self.similarWords[matchKey]
            for match in curMatches:
                wordsSimilar.add(match)

        wordsSimilar.remove(word)
        return wordsSimilar


    def totalWordsSimilarTo(self, word):
        return len( self.wordsSimilarTo(word) )

    @staticmethod
    def matchKeys(word):

        matchKeys = []

        for charIndex in range( len(word) ):
            matchKey = word[0:charIndex] + "_" + word[charIndex+1:len(word)]
            matchKeys.append(matchKey)

        return matchKeys

    @staticmethod
    def generateJSON():
        dictionary = WordFinder.generateDictionary()
        similarWords = WordFinder.generateSimilarWords(dictionary)

        with open(WordFinder.DICTIONARY_JSON, 'w') as dictFile:
            dictFile.write( json.dumps(dictionary) )

        with open(WordFinder.SIMILILAR_WORDS_JSON, 'w') as similarWordsFile:
            similarWordsFile.write( json.dumps(similarWords) )

    @staticmethod
    def generateDictionary():

        dictionary = []

        with open(DICTIONARY_FILE) as wordDict:
            fullDictionary = [word.strip() for word in wordDict] 
            dictionary = [word for word in fullDictionary] 

        return dictionary

    @staticmethod
    def generateSimilarWords(dictionary):

        similarWords = defaultdict(list)

        for word in dictionary:
            for matchKey in WordFinder.matchKeys(word):
                similarWords[matchKey].append(word)

        return similarWords











