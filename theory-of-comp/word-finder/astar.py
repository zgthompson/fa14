from priority_queue import PriorityQueue

class AStar(object):
    """
    Given a starting node and a goal node, finds the shortest cost path
    from the starting node to the goal node.
    """

    def __init__(self, root_node, goal_node):
        """
        Initializes the AStar object and sets function that determines the
        order in which the nodes are evaluated.

        Precondition: nodes can be any type of object, but must be able to
        be compared by str(node1) == str(node2)
        """
        self.goal_node = goal_node

        # min heap for the fringe
        self.fringe = PriorityQueue()
        self.fringe.add_node( root_node, self.h_score(root_node) )

        # min cost to node
        self.min_node_score = {}
        self.min_node_score[ str(root_node) ] = 0

        # path to start
        self.came_from = {}

        self.nodes_visited = 0

    def search(self):
        """
        Returns a list that contains the nodes of the shortest path to the goal,
        starting with the root node and ending with the goal node. Will return
        an empty list if there is no solution
        """
        while self.fringe:
            # grab the best candidate so far
            current_node = self.fringe.pop_node()

            self.nodes_visited += 1

            if self.is_goal(current_node): return self.path_to_root(current_node)
            else:
                self.add_children_to_fringe(current_node)
        # no path was found
        return []

    def h_score(self, node):
        """
        Returns the estimated cost from node to goal_node
        """
        raise NotImplementedError

    def children(self, node):
        """
        Returns a list of tuples with the first tuple value being the child
        and the second value being the cost to move to that child
        """
        raise NotImplementedError

    def path_to_root(self, node):
        """
        Return a list of nodes from root_node to node
        """
        path = [ node ]
        current_key = str(node)

        while current_key in self.came_from:
            next_node = self.came_from[current_key]
            path.append(next_node)
            current_key = str(next_node)

        # reverse order so root node is first
        path.reverse()

        for i, node in enumerate(path):
            if i == 0:
                print "Input  : %s" % node
            else:
                print "Move %2s: %s" % (i, node)

        return path


    def add_children_to_fringe(self, node):
        """
        Adds all the children of node to fringe and updates min_node_score and
        came_from dictionaries
        """
        node_key = str(node)
        cost_to_node = self.min_node_score[ node_key ]

        for child, cost_to_move in self.children(node):
            cost_to_child = cost_to_node + cost_to_move
            child_key = str(child)
            if child_key not in self.min_node_score or self.min_node_score[child_key] > cost_to_child:
                self.fringe.add_node(child, cost_to_child + self.h_score(child))
                self.min_node_score[child_key] = cost_to_child
                self.came_from[child_key] = node

    def is_goal(self, node):
        """
        Returns True if node is the same as goal_node, False otherwise
        """
        return str(node) == str(self.goal_node)

