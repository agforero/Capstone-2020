#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

int itest = 0;
#define ITEST cout << "TEST #" << itest++ << endl;

// HELPER FUNCTIONS ========================================================================================================================
// helps create unique alphabetical IDs for each Node.
string intToAlphaID(int n) {
    if (n == 0) {
        string ret = "A";
        return ret;
    }
    vector<char> letterVals;
    for (int i = 0; i < 26; i++) {
        char thisChar = i + 65;
        letterVals.push_back(thisChar);
    }

    string ret = "";
    while (n > 0) {
        ret += letterVals[(n % 26)];
        n /= 26; // should round down and be ok.
    }
    return ret;
}

// returns a hex color, depending on whether or not i == 0.
string vectorColor(int idx) {
    if (idx == 0) {
        string ret = "\"#0f0\"";
        return ret;
    }
    else {
        string ret = "\"#f00\"";
        return ret;
    }
}

// just...you know. prints an error message.
void printInvalidArgs() {
    cout << "invalid argument(s)." << endl;
}

// gets next command line arg from this flag; or, if it doesn't exist, throws an error 
int getFlagArg(int argc, char ** argv, string flag) {
    for (int i = 1; i < argc-1; i++) { // skip executable and last arg
        if (flag.compare(argv[i]) == 0) {
            return atoi(argv[i+1]); // shouldn't be a flag.
        }
    }
    // at this point, we've encountered an error. we can exit(0) immediately
    printInvalidArgs(); exit(0); return -1; // added a return to silence g++
}

void printHelp() { // by God, is there a better way to do this? perhaps.
    cout << "usage: ./graphOutputDriver graphType (flags)" << endl;
    cout << endl;
    cout << "graphType values:" << endl;
    cout << "0: none" << endl;
    cout << "1: directed acyclic graph" << endl;
    cout << "2: linked list" << endl;
    cout << "3: tree" << endl;
    cout << endl;
    cout << "optional flags available:" << endl;
    cout << "-nodes <n>\tforce the creation of n nodes in the graph. default 8." << endl;
    cout << "-v <n>\t\trandomize the node vals to be between 1 and n. default False." << endl;
    cout << "-w <n>\t\trandomize the edge weights to be integers between 1 and n. by default, n is 1, implying weightlessness." << endl;
    cout << "-v <n>\t\tedit variability; nodes will now randomly have between 1 and n edges stemming from them. works best with high node counts." << endl;
    cout << endl;
    cout << "run ./graphOutputDriver -h or ./graphOutputDriver --help to display this help menu." << endl;
}

// NODE CLASS DECLARATION ========================================================================================================================
class Node {
private:
    string val;
    int ID;

public:
    // constructor and destructor
    Node(int i, string v="") {
        ID = i;
        val = v;
    }
    ~Node() {
        delete this;
    }

    // getters
    string getVal() {
        return val;
    }
    int getID() {
        return ID;
    }

    // setters
    void setVal(string s) {
        val = s;
    }
};

// GRAPH CLASS DECLARATION ========================================================================================================================
class Graph {
private:
    vector<Node *> allNodes;        // vector of all nodes
    vector<vector<int>> allEdges;   // vector of vectors of all edges; stored as {thisID, thatID, weight=1}

    int everCount = 0;              // how many Nodes have ever existed; helps with creating unique IDs
    int startCount = 8;             // how many Nodes to start; default 8 as per research
    int variability = 3;            // in a graph with random edge counts per node, this is highest possible edge count.
    int weight = 1;                 // default weight to put on edges. starts at 1, which basically means weightless
    int t = 0;                      // what type of graph this is. default none, aka 0

    bool directed = false;          // whether or not the edges are directed
    bool alpha = true;              // whether or not Node vals should be alphabetical. default true.

public:
    // constructor
    Graph(int argc, char ** argv) {
        // reroll random rolls so the rolls are random
        srand(time(NULL)); 
        
        // sift through args; see if we need to override defaults
        // start by checking for -h or --help
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printHelp(); exit(0);
            }
        }

        // see if we can't extrapolate graph type
        if (argc >= 2) {
            t = atoi(argv[1]); // should return 0 if nothing else...bug proof??
        }

        // process other arguments
        for (int i = 2; i < argc-1; i++) { // skip executable name and graph type
            //string argToString = argv[i];
            if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
                printHelp(); exit(0); // terminate 
            }
            else if (strcmp(argv[i], "-nodes") == 0) {
                startCount = getFlagArg(argc, argv, "-nodes");
            }
            else if (strcmp(argv[i], "-w") == 0) {
                weight = getFlagArg(argc, argv, "-w");
            }
            else if (strcmp(argv[i], "-v") == 0) {
                variability = getFlagArg(argc, argv, "-v");
            }
        }

        switch (t) {
            case 0:
                generateNONE();
                break;
            case 1:
                generateDAG(); 
                break;
            case 2:
                generateLINKEDLIST();
                break;
            case 3:
                generateTREE();
                break;
        }
    }

    // GENERATE FUNCTIONS ========================================================================================================================
    // a bunch of disconnected nodes. none of them have edges connecting each other. pretty straightforward.
    void generateNONE() {
        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode());
        }
    }

    /* a directed acyclic graph. this is a graph that cannot be cycled infinitely using any given path, starting 
    at any given node. thus, it has startCount nodes each with, let's say, 0 to 3 edges: but these edges cannot
    point backwards. they can point to a node further down the line, but they cannot point backwards, as this 
    might allow a cycle. can't have that. bad for business.

    it should also point to its next neighbor. */
    void generateDAG() {
        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode());
        }

        // last two nodes are special cases here:
        // second-to-last only points to last node, and nothing else.
        // last node finishes the graph, so it points to nothing.

        // first, make it basically generate a linked list
        for (int i = 0; i < startCount-1; i++) {
            allEdges.push_back(createEdge(i, i+1));
        }

        // then, for every Node, have it randomly point to a node further down the line
        for (int i = 0; i < startCount-2; i++) { // -2 so we DON'T mess with the last two Nodes
            int randoms = rand() % 3 + 1;
            vector<int> newConns;

            for (int k = 0; k < randoms; k++) {
                bool unique = true;
                int range1 = allNodes.size() - i - 2;
                int range2 = (range1 != 0) ? rand() % range1 : 0;
                int nextCandidate = i + 2 + range2;

                for (auto con : newConns) {
                    if (con == nextCandidate || nextCandidate == allNodes.size()) unique = false;
                }
                if (unique) newConns.push_back(nextCandidate); // add to newConns
            } 

            for (int j = 0; j < newConns.size(); j++) {
                allEdges.push_back(createEdge(i, newConns[j])); // add these edges to the DAG
            }
        }
    }

    // a string of nodes, each pointing to the next one in line.
    void generateLINKEDLIST() {
        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode());
        }
        for (int i = 0; i < startCount-1; i++) {
            allEdges.push_back(createEdge(i, i+1));
        }
    }

    /* a tree with anywhere between 1 to variability connections per node per layer before continuing.
    this works a lot better with longer node counts, like 16. */
    void generateTREE() {
        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode());
        }

        vector<int> currentLayer { 0 };
        vector<int> nextLayer;

        queue<int> availableNodes;
        for (int i = 1; i < startCount; i++) {
            availableNodes.push(i);
        }

        while (!currentLayer.empty()) {
            for (int i = 0; i < currentLayer.size(); i++) {
                int children = rand() % variability + 1;
                for (int j = 0; j < children; j++) {
                    if (availableNodes.empty()) break;
                    allEdges.push_back(createEdge(currentLayer[i], availableNodes.front()));
                    nextLayer.push_back(availableNodes.front());
                    availableNodes.pop();
                }
            }
            vector<int> emptyVector;
            currentLayer = nextLayer;
            nextLayer = emptyVector;
        }
    }

    // GRAPH HELPER METHODS ========================================================================================================================
    // creates a Node using new, with a val adaptive to alpha, and returns.
    Node * createNode() {
        if (alpha) {
            Node * temp = new Node(everCount, intToAlphaID(everCount)); everCount++;
            return temp; // notice, it increments everCount here,
        }
        else {
            Node * temp = new Node(everCount, to_string(everCount)); everCount++;
            return temp; // so that the next node WILL have unique ID
        }
    }

    // creates an edge between two nodes. saves a line.
    vector<int> createEdge(int start, int stop) {
        vector<int> nextEdge {start, stop, (rand() % weight + 1)};
        return nextEdge;
    }

    // returns an annotation value depending on adjacency to head node.
    string getAnnotation(int ID) {
        string ret = "\"inf?\""; // assumed infinite if not special case
        // if this node IS the head
        if (ID == 0) { 
            ret = "\"0\"";
            return ret;
        }
        for (int i = 0; i < allEdges.size(); i++) {
            // if this node is connected to head (ID = 0)
            if (allEdges[i][0] == 0 && allEdges[i][1] == ID) {
                ret = "\""; ret.append(to_string(ID)); ret.append("\"");
            }
        }
        return ret; // will either return ID? or inf? at this point
    }

    // JSON output functions. you know, this would have been easier with printf(), but that's ok :)
    void dataToJSON() {
        cout << "{" << endl;
        cout << "\t\"type\": \"animation\"," << endl; // brain hurty

        // print all the nodes
        cout << "\t\"nodes\": {" << endl;
        for (int i = 0; i < allNodes.size(); i++) {
            cout << "\t\t\"" << allNodes[i]->getID() << "\": {" << endl;

            cout << "\t\t\t\"id\": "           << allNodes[i]->getID()                             << "," << endl;
            cout << "\t\t\t\"name\": \""       << allNodes[i]->getVal()                            << "\"," << endl;
            cout << "\t\t\t\"position\": { "   << "\"x\": " << 50*(i+1) << ", \"y\": " << 50*(i+1) << " }," << endl;
            cout << "\t\t\t\"color\": "        << vectorColor(i)                                   << "," << endl;
            cout << "\t\t\t\"annotation\": "   << getAnnotation(i)                                 << endl;

            if (i != allNodes.size()-1) cout << "\t\t}," << endl;
            else cout << "\t\t}" << endl;
        }
        cout << "\t}," << endl;

        // print all the edges
        cout << "\t\"edges\": {" << endl;
        for (int i = 0; i < allEdges.size(); i++) {
            cout << "\t\t\"" << allEdges[i][0] << "-" << allEdges[i][1] << "\": {" << endl;

            cout << "\t\t\t\"start\": "    << allEdges[i][0]       << "," << endl;
            cout << "\t\t\t\"end\": "      << allEdges[i][1]       << "," << endl;
            cout << "\t\t\t\"weight\": "   << allEdges[i][2]       << endl;

            if (i != allEdges.size()-1) cout << "\t\t}," << endl;
            else cout << "\t\t}" << endl;
        }
        cout << "\t}" << endl;

        // ending everything else
        cout << "}" << endl;
    }
};