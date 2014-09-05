from game_of_life import GameOfLife

life = GameOfLife()

for generation in range(5):
    print "Generation %d" % generation
    life.prettyPrint()
    life.tick()
