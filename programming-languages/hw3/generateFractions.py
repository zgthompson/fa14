import sys
import random

def randomFractionString():
    wholeNumber = random.randint(-256, 255)

    if wholeNumber < 0:
        numerator = random.randint(1, 256)
    else:
        numerator = random.randint(-256, 255)

    denominator = random.randint(1, 256)

    return "%d %d/%d\n" % ( wholeNumber, numerator, denominator )

def main():

    if len(sys.argv) != 2:
        print "Format: generationFractions.py amount"
        print "Example: generationFractions.py 1000"
        return

    amount = int(sys.argv[1])

    random.seed()

    fractionStrings = [randomFractionString() for i in range(amount)]
    
    fractionText = ''.join(fractionStrings)

    with open("fractions.txt", 'w') as f:
        f.write(fractionText)

main()

