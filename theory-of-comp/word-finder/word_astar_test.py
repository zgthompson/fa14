from word_astar import *
import sys

def main():
    if len(sys.argv) != 3:
        print "%s WORD1 WORD2" % sys.argv[0]
        return

    a = WordAStar(sys.argv[1], sys.argv[2])
    print "start"
    a.search()
    print "end"

main()
