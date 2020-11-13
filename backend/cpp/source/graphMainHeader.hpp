#ifndef _GOD_H_
#define _GOD_H_
#include "globalHelperFunctions.hpp"
#include "nodeClass.hpp"

class Graph {
private:
    vector<Node *> allNodes;            // vector of all nodes
    vector<Edge *> allEdges;            // vector storing all edges
    vector<Edge *> usedEdges;           // vector of whichever edges were actually used. used for dataToJSON_FILTERED().
    vector<vector<int>> currentStack;   // IDs and distances of whichever Nodes are currently candidates in the stack. updates dynamically.

    int everCount = 0;            // how many Nodes have ever existed; helps with creating unique IDs
    int startCount = 8;           // how many Nodes to start; default 8 as per research
    int variability = 4;          // in a graph with random edge counts per node, this is highest possible edge count
    int weight = 1;               // default max weight to put on edges. starts at 1, which basically means weightless
    int gridWidth = 4;            // grid is default 4 nodes wide.
    int t = 0;                    // what type of graph this is. default none, aka 0
    int bolded = 0;               // the current bolded node. this changes to embolden whichever node was visited most recently
    int target = 0;               // target node in search algorithms. applicable to something like DFS.
    int currentLine = 0;          // which line in the pseudocode is currently highlighted.

    bool directed = false;        // whether or not the edges are directed
    bool alpha = true;            // whether or not Node vals should be alphabetical. default true.
    bool cyclesOK = false;        // allow edges to go back and forth between the same two nodes.

public:
    // constructor
    Graph(int argc, char **argv) {
        // reroll random rolls so the rolls are random
        srand(time(NULL));

        // sift through args; see if we need to override defaults
        // start by checking for -h or --help
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printHelp();
                exit(0);
            }
        }

        // see if we can't extrapolate graph type
        if (argc >= 2) {
            t = atoi(argv[1]); // should return 0 if nothing else...bug proof??
        }

        // process other arguments
        for (int i = 2; i < argc - 1; i++) { // skip executable name and graph type
            //string argToString = argv[i];
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printHelp();
                exit(0); // terminate
            } else if (strcmp(argv[i], "-nodes") == 0) {
                startCount = getFlagArg(argc, argv, "-nodes");
            } else if (strcmp(argv[i], "-w") == 0) {
                weight = getFlagArg(argc, argv, "-w");
            } else if (strcmp(argv[i], "-v") == 0) {
                variability = getFlagArg(argc, argv, "-v");
            } else if (strcmp(argv[i], "-i") == 0) {
                alpha = false;
            } else if (strcmp(argv[i], "-g") == 0) {
                gridWidth = getFlagArg(argc, argv, "-g");
            } else if (strcmp(argv[i], "-t") == 0) {
                target = getFlagArg(argc, argv, "-t");
            } else if (strcmp(argv[i], "-c") == 0) {
                cyclesOK = true;
            }
        }

        switch (t) {
            case 0: generateNONE(); break;
            case 1: generateDAG(); break;
            case 2: generateLINKEDLIST(); break;
            case 3: generateTREE(); break;
            case 4: generateGRID(); break; 
        }
    }

    // generation functions
    void generateNONE();
    void generateDAG();
    void generateLINKEDLIST();
    void generateTREE();
    void generateGRID();


    // graph class helper functions
    // element creation
    Node * createNode();
    Edge * createEdge(int start, int stop);

    // helpers for grid
    vector<int> findForwardConns(int idx);
    vector<int> findBackwardsConns(int idx);
    void cleanupEdges();

    // helpers for algos
    vector<Edge *> getEdges(int ID);
    vector<Edge *> getUnvisitedEdges(int ID);
    Node * getNode(int ID);
    vector<int> getVisited();
    void visit(int ID);
    void consider(int ID);
    void unbold();
    void addToStack(int ID, int distance);
    void removeFromStack(int ID);
    int idxToID(int idx);

    // couting data
    vector<int> remainingToPrint(vector<bool> printed);
    string getEdgeID(Edge* edge);
    string vectorColor(int ID);
    string isBold(int ID);
    void dataToJSON();
    void dataToJSON_FILTERED();
    void printStack();

    // the graph traversal algorithms (where the money's at)
    void prims();
    void DFS(int current=0);
    void BFS(int current=0);
    void dijkstras();
};

#endif /* _GOD_H_ */