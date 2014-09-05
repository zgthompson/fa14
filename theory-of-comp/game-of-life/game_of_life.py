

class GameOfLife(object):

    BOARD_SIZE = 3
    CELL_DEAD = " "
    CELL_LIVE = "o"

    def __init__(self):
        self.board = [[ " ", "o", " "], [" ", "o", " "], [" ", "o", " "]]


    def prettyPrint(self):
        print "-" * (GameOfLife.BOARD_SIZE + 2)
        for row in self.board:
            curRow = "|"
            for col in row:
                curRow += col
            curRow += "|"
            print curRow
        print "-" * (GameOfLife.BOARD_SIZE + 2)



    @staticmethod
    def generateNewBoard():
        return [ [GameOfLife.CELL_DEAD for col in range(GameOfLife.BOARD_SIZE)] for row in range(GameOfLife.BOARD_SIZE) ]

    def tick(self):

        nextBoard = GameOfLife.generateNewBoard()

        for rowIndex in range(GameOfLife.BOARD_SIZE):
            for colIndex in range(GameOfLife.BOARD_SIZE):

                neighborCount = self.countNeighborsAt(rowIndex, colIndex)

                if self.positionIsAlive(rowIndex, colIndex):

                    if neighborCount < 2 or neighborCount > 3:
                        nextBoard[rowIndex][colIndex] = GameOfLife.CELL_DEAD
                    else:
                        nextBoard[rowIndex][colIndex] = GameOfLife.CELL_LIVE

                # dead cell
                else:

                    if neighborCount == 3:
                        nextBoard[rowIndex][colIndex] = GameOfLife.CELL_LIVE
                    else:
                        nextBoard[rowIndex][colIndex] = GameOfLife.CELL_DEAD

        self.board = nextBoard


    def countNeighborsAt(self, row, col):

        neighborCount = 0

        for neighborRow in range(row - 1, row + 2):
            for neighborCol in range(col - 1, col + 2):

                isInputRow = neighborRow == row and neighborCol == col

                if not isInputRow and self.isValidPosition(neighborRow, neighborCol) and self.positionIsAlive(neighborRow, neighborCol):
                    neighborCount += 1

        return neighborCount



    def isValidPosition(self, row, col):

        rowIsValid = row >=0 and row < GameOfLife.BOARD_SIZE
        colIsValid = col >=0 and col < GameOfLife.BOARD_SIZE

        return rowIsValid and colIsValid

    def positionIsAlive(self, row, col):
        return self.board[row][col] == GameOfLife.CELL_LIVE



