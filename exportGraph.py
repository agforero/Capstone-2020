#!/usr/bin/env python3

# this program exports, as a JSON file, the structure of whatever graph is used as 
# argv[1]. it will export data such as whether the edges are directed, what nodes are
# connected and where, edge distance and which node is the default head.

# ideally, according to some of the research I read for the background paper, we shouldn't
# use too many nodes at a time; maybe something like 8 or 10 that they can interact with.
# with some of the more involved graph traversal algorithms, we can increase that number
# to challenge students to accurately predict what the algo will do next.

# written by Agustin

import sys
from textwrap import dedent

class Graph:
    def __init__(self, selectedType=1, nodes=8, head=0, directed=False, weighted=False):
        self.graphTypes = {     # dictionary holding types of graphs; can add more if need be
            1: "NONE",          # no type yet; just disjointed nodes
            2: "LINKEDLIST",    # a simple linked list (directed)
            3: "BINTREE",       # binary tree
            4: "UNDIR",         # undirected graph
            5: "DAG",           # directed acyclic graph
            6: "WEIGHTED",      # weighted graph (aka, edges have distance)
            7: "WEIGHTED_DIR",  # weighted graph, with directional edges
        } # this dict exists so we can know, explicitly, what graph this is...not just ID number

        self.type = selectedType    # what type of graph this is. default 1, which is none
        self.nodeCount = nodes      # how many nodes this graph contains. default 8
        self.head = head            # which node is the head. default node ID 0
        self.directed = directed    # whether or not the edges are directed. default False
        self.weighted = weighted    # whether or not the edges are weighted. default False
        self.allEdges = []          # all edges; stored as an adjacency list. connects IDs, not values!!
        self.allNodes = []          # all Nodes; stored as a list of Node objects

        # now: for each type of graph, we dictate how it is generated.
        # 1. NONE: no type yet. just a bunch of disconnected nodes.
        if self.type == 1:
            for i in range(self.nodeCount):
                self.allNodes.append(Node(i)) # add node of ID i, val i, unexplored

        # 2. LINKEDLIST: a linked list. just a line of directed nodes.
        elif self.type == 2:
            self.directed = True
            for i in range(self.nodeCount):
                self.allNodes.append(Node(i))
                if i != 0: # if we're not at the first node,
                    self.allEdges.append([i-1, i, 1]) # from previous node to this node, with distance of 1 each time.

        # 3. BINTREE: a binary tree. each node has two children. directed.
        elif self.type == 3:
            pass

    def printGraph(self):
        print(f"DIRECTED: {self.directed}\tWEIGHTED: {self.weighted}")

        print("\nNODES:")
        print("ID\tVAL\tEXPLORED")
        for node in self.allNodes:
            node.printNode()

        print("\nEDGES:")
        print("START\tEND\tDIST")
        for edge in self.allEdges:
            print(f"{edge[0]}\t{edge[1]}\t{edge[2]}")

class Node: 
    def __init__(self, ID, val=None, explored=False):
        # ID = ID of node. does not change. used to identify this node in particular. 
        # val = value of node. could be the same as ID, or a string, or some random integer...you name it.
        # explored = whether or not this node has been explored. default False; important for DFS and the like
        # edges themselves are stored in the graph.

        self.ID = ID
        if val == None: self.val = ID # if specific value specified, use that value
        else: self.val = val # otherwise default to value of ID
        self.explored = explored

    def printNode(self):
        print(f"{self.ID}\t{self.val}\t{self.explored}")

def printUsage():
    print(dedent("""\
    usage: ./exportGraph.py graphType (nodes) (headNodeID)

    graphType values:
    1: none (a bunch of disconnected nodes)
    2: linked list, directed
    3: binary tree
    4: undirected graph
    5: directed acyclic graph
    6: weighted graph, undirected
    7: weighted graph, directed

    use the -h or --help flags as the first argument to display this help menu.\
    """))

def main():
    if len(sys.argv) > 4: # if too many arguments used
        print("too many arguments.", end=' ')
        printUsage()
        sys.exit(1)

    elif sys.argv[1] == "-h" or sys.argv[1] == "--help":
        printUsage()
        sys.exit(0)

    try:
        if len(sys.argv) == 1: g1 = Graph() # no argument; defaults to NONE type (disconnected nodes)
        elif len(sys.argv) == 2: g1 = Graph(int(sys.argv[1])) # one argument; uses whatever structure they choose
        elif len(sys.argv) == 3: g1 = Graph(int(sys.argv[1]), int(sys.argv[2])) # two arguments; creates graph of their choice using argv[2] nodes
        else: g1 = Graph(int(sys.argv[1]), int(sys.argv[2]), int(sys.argv[3])) # everything from before but user also defines head node

    except TypeError: # if they use any arguments that can't be converted to ints
        print("invalid argument(s).", end=' ')
        printUsage()
        sys.exit(2)

    g1.printGraph()

if __name__ == "__main__":
    main()
