#ifndef _GOD_H_
#define _GOD_H_
#include "globalHelperFunctions.hpp"
#include "nodeClass.hpp"

class Graph {
private:
    vector<Node *> allNodes;      // vector of all nodes
    vector<vector<int>> allEdges; // vector of vectors of all edges; stored as {thisID, thatID, weight=1}

    int everCount = 0;            // how many Nodes have ever existed; helps with creating unique IDs
    int startCount = 8;           // how many Nodes to start; default 8 as per research
    int variability = 4;          // in a graph with random edge counts per node, this is highest possible edge count.
    int weight = 1;               // default weight to put on edges. starts at 1, which basically means weightless
    int gridWidth = 4;            // grid is default 4 nodes wide.
    int t = 0;                    // what type of graph this is. default none, aka 0

    bool directed = false;        // whether or not the edges are directed
    bool alpha = true;            // whether or not Node vals should be alphabetical. default true.

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
    vector<int> createEdge(int start, int stop);

    // helpers for grid
    vector<int> findForwardConns(int idx);
    vector<int> findBackwardsConns(int idx);

    // helpers for algos
    vector<vector<int>> getEdges(int idx);

    // couting data
    string getAnnotation(int ID);
    string vectorColor(int idx);
    void dataToJSON();

    // the graph traversal algorithms (where the money's at)
    void prims();
};

#endif /* _GOD_H_ */