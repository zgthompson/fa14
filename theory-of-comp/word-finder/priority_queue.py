from heapq import *
import itertools

class PriorityQueue(object):

    
    REMOVED = '<removed-node>'      # placeholder for a removed node

    def __init__(self):
        self.pq = []                         # list of entries arranged in a heap
        self.entry_finder = {}               # mapping of nodes to entries
        self.counter = itertools.count()     # unique sequence count

    def add_node(self, node, priority=0):
        'Add a new node or update the priority of an existing node'

        key = str(node)
        if key in self.entry_finder:
            self.remove_node(node)
        count = next(self.counter)
        entry = [priority, count, node]
        self.entry_finder[key] = entry
        heappush(self.pq, entry)

    def remove_node(self, node):
        'Mark an existing node as self.REMOVED.  Raise KeyError if not found.'
        entry = self.entry_finder.pop(str(node))
        entry[-1] = PriorityQueue.REMOVED

    def pop_node(self):
        'Remove and return the lowest priority node. Raise KeyError if empty.'
        while self.pq:
            priority, count, node = heappop(self.pq)
            if node is not PriorityQueue.REMOVED:
                del self.entry_finder[str(node)]
                return node
        raise KeyError('pop from an empty priority queue')

    def __len__(self):
        return len(self.entry_finder)

