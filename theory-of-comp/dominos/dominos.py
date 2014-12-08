from itertools import product

class Domino(object):

    def __init__(self, top, bottom):
        self.top = top
        self.bottom = bottom

    def __str__(self):
        return "%s|%s" % (self.top, self.bottom)

    @staticmethod
    def dominosMatch(dominos):

        posOfDomino = { domino: index+1 for index, domino in enumerate(dominos) }

        for count in range(9):
            for ordering in product(dominos, repeat=count):

                tops = "".join( [domino.top for domino in ordering] )
                bottoms = "".join( [domino.bottom for domino in ordering] )

                if ordering and tops == bottoms:
                    print "-" * 20
                    print [posOfDomino[domino] for domino in ordering]
                    print ", ".join([str(item) for item in ordering])
                    print "-" * 20

def main():
    """
    print "Problem 1"
    Domino.dominosMatch( [ Domino("AAA", "AA"), Domino("BAA", "ABAAA") ] )
    print "Problem 2"
    Domino.dominosMatch( [ Domino("A", "AAA"), Domino("ABAAA", "AB"), Domino("AB", "B") ] )
    print "Problem 3"
    Domino.dominosMatch( [ Domino("AB", "ABA"), Domino("BBA", "AA"), Domino("ABA", "BAB") ] )
    print "Problem 4"
    Domino.dominosMatch( [ Domino("AB", "BB"), Domino("AA", "BA"), Domino("AB", "ABB"), Domino("BB", "BAB") ] )
    """
    print "Problem 5"
    Domino.dominosMatch( [ Domino("AB", "ABA"), Domino("BA", "ABB"), Domino("B", "AB"), Domino("ABB", "B"), Domino("A", "BAB") ] )
    """
    print "Test 1, [ A|A ]"
    Domino.dominosMatch( [ Domino("A", "A") ] )
    print "Test 2, [ A|B ]"
    Domino.dominosMatch( [ Domino("A", "B") ] )
    print "Test 3, [ A|BAA, AB|AA, BBA|BB ]"
    Domino.dominosMatch( [ Domino("A", "BAA"), Domino("AB", "AA"), Domino("BBA", "BB") ] )
    print "Test 4, [ BB|B, AB|BA, C|BC ]"
    Domino.dominosMatch( [ Domino("BB", "B"), Domino("AB", "BA"), Domino("C", "BC") ] )
    """

main()
