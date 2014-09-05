from collections import deque

def mergesort(array, low, high):
    if low < high:
        middle = (low + high) / 2
        mergesort(array, low, middle)
        mergesort(array, middle+1, high)
        merge(array, low, middle, high)

def merge(array, low, middle, high):

    bufferLow = deque(array[low:middle+1])
    bufferHigh = deque(array[middle+1:high+1])

    i = 0

    while bufferLow and bufferHigh:
        if bufferLow[0] < bufferHigh[0]:
            array[i] = bufferLow.popleft()
        else:
            array[i] = bufferHigh.popleft()

        i += 1

    while bufferLow:
        array[i] = bufferLow.popleft()
        i += 1

    while bufferHigh:
        array[i] = bufferHigh.popleft()
        i+= 1

def main():
    array = [43, 251, 763, 1324, 5, 27, 721, 62, 346]
    mergesort(array, 0, len(array) - 1)
    print array

main()


