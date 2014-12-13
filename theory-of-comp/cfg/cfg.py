import sys
from collections import defaultdict

def grammarFromFile(filename):
    grammar = defaultdict(list)
    with open(filename) as f:
        for line in f:
            rule = line.strip()
            left, right = rule.split(" -> ")
            if right == "{}":
                right = ""
            grammar[left].append(right)
    return grammar

def productionsWithMaxLength(grammar, startState, maxLength):
    productions = set()
    for state in grammar[startState]:
        productionHelper(grammar, state, maxLength, productions)
    return productions



def productionHelper(grammar, curState, maxLength, productions):
    if len(curState) > maxLength:
        return
    elif not any([ch.isupper() for ch in curState]):
        productions.add(curState)
    else:
        for index, ch in enumerate(curState):
            if ch.isupper():
                for state in grammar[ch]:
                    newState = curState[:index] + state + curState[index+1:]
                    productionHelper(grammar, newState, maxLength, productions)


    
def main():
    if len(sys.argv) != 2:
        print "Use: %s filename" % sys.argv[0]
        return

    grammar = grammarFromFile(sys.argv[1])
    productions = productionsWithMaxLength(grammar, "S", 10)
    print sorted(productions, key=len)

main()
