from astar import AStar
from word_finder import WordFinder
class WordAStar(AStar):

    def __init__(self, start_node, goal_node):
        super(WordAStar, self).__init__(start_node, goal_node)
        self.finder = WordFinder()

    def h_score(self, node):
        score = 0
        for left, right in zip(node, self.goal_node):
            if (left == right):
                score += 1
        return score


    def children(self, node):
        return [(word, 1) for word in self.finder.wordsSimilarTo(node)]
