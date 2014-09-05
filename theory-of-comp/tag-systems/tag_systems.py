def tags(startTag):

    ZERO_SUFFIX = "00"
    ONE_SUFFIX = "1101"
    REMOVE_COUNT = 3

    statesVisited = set()

    current = startTag

    while current not in statesVisited and len(current) >= 3:

        yield current

        statesVisited.add(current)

        if current[0] == "1":
            current = current[REMOVE_COUNT:] + ONE_SUFFIX
        else:
            current = current[REMOVE_COUNT:] + ZERO_SUFFIX

class TagSystem(object):

    ZERO_SUFFIX = "00"
    ONE_SUFFIX = "1101"
    REMOVE_COUNT = 3

    def __init__(self, startingPoint):
        self.startState = startingPoint
        self.state = startingPoint

    def nextState(self):

        if len(self.state) < TagSystem.REMOVE_COUNT:
            return

        firstChar = self.state[0]

        self.state = self.state[TagSystem.REMOVE_COUNT:]
        if firstChar == "1":
            self.state += TagSystem.ONE_SUFFIX
        else:
            self.state += TagSystem.ZERO_SUFFIX


    def showNextStates(self, statesToShow):
        for state in range(statesToShow):
            print self.state
            self.nextState()

        self.resetToStartState()


    def doesTerminate(self):

        for state in range(1000):

            if len(self.state) < 3:

                self.resetToStartState()
                return True

            self.nextState()

        self.resetToStartState()
        return False



    def doesLoop(self):

        visitedStates = set()

        for state in range(1000):
            if self.state in visitedStates:
                self.resetToStartState()
                return True
            else:
                visitedStates.add( self.state )
                self.nextState()

        self.resetToStartState()
        return False


    def uniqueStateCount(self):

        visitedStates = set()

        for state in range(1000):

            if self.state in visitedStates:
                self.resetToStartState()
                return len(visitedStates)
            else:
                visitedStates.add( self.state )
                self.nextState()

        self.resetToStartState()
        return -1


    def resetToStartState(self):
        self.state = self.startState
