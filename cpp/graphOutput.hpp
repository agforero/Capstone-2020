#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

int itest = 0;
#define ITEST cout << "TEST #" << itest++ << endl;

// TYPE ENUMERATOR ========================================================================================================================
enum TYPE {
    NONE,       // no type yet; just disjointed nodes
    DAG,        // directed acyclic graph
    LINKEDLIST, // a linked list. duh
};

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

    bool directed;                  // whether or not the edges are directed
    bool alpha = true;              // whether or not Node vals should be alphabetical. default true.

public:
    // constructor
    Graph(TYPE t=NONE, int s=8, bool a=true) {
        srand(time(NULL)); 
        startCount = s; 
        alpha = a; 

        switch(t) {
            case NONE:
                generateNONE();
                break;
            case DAG:
                generateDAG(); 
                break;
            case LINKEDLIST:
                generateLINKEDLIST();
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

        // then, for every Node, have it randomly point to a node further down the line
        for (int i = 0; i < startCount-1; i++) { // -1 so we DON'T mess with the last Node
            // neighbor down the line
            allEdges.push_back(createEdge(i, i+1));

            // 1 to 3 random Nodes up ahead
            int randoms = rand() % 3 + 1;
            vector<int> newConns;
            for (int k = 0; k < randoms; k++) {
                // start by generating how many numbers out this connection will go. 
                // range is i + 2 to last node......so how does that translate?
                int range1 = ((allNodes.size() - i - 2) != 0) ? (allNodes.size() - i - 2) : 1;
                int range2 = rand() % range1; // this is to prevent floating point exception (0 % 0)!!!!

                // then we start nextCandidate at i+2 (since we don't wanna just connect
                // to the node in front randomly. we already did that). this should
                // correctly create edges going forward (albeit with some repeats -- we 
                // can add more logic here if we wanna get rid of said repition, or just
                // brute force some way to remove all of them)
                int nextCandidate = i + 2 + range2;

                newConns.push_back(nextCandidate); // add to newConns
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

    // GRAPH HELPER METHODS ========================================================================================================================
    // creates a Node using new, with a val adaptive to alpha, and returns.
    Node * createNode() {
        if (alpha) {
            Node * temp = new Node(everCount, intToAlphaID(everCount++));
            return temp; // notice, it increments everCount here,
        }
        else {
            Node * temp = new Node(everCount, to_string(everCount++));
            return temp; // so that the next node WILL have unique ID
        }
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
        cout << "let Frames = [" << endl;
        cout << "\t{" << endl;
        cout << "\t\t\"type\": \"animation\"," << endl; // brain hurty

        // print all the nodes
        cout << "\t\t\"nodes\": {" << endl;
        for (int i = 0; i < allNodes.size(); i++) {
            cout << "\t\t\t\"" << allNodes[i]->getID() << "\": {" << endl;
            cout << "\t\t\t\t\"id\": "           << allNodes[i]->getID()                             << "," << endl;
            cout << "\t\t\t\t\"name\": \""       << allNodes[i]->getVal()                            << "\"," << endl;
            cout << "\t\t\t\t\"position\": { "   << "\"x\": " << 50*(i+1) << ", \"y\": " << 50*(i+1) << " }," << endl;
            cout << "\t\t\t\t\"color\": "        << vectorColor(i)                                   << "," << endl;
            cout << "\t\t\t\t\"annotation\": "   << getAnnotation(i)                                 << "," << endl;
            cout << "\t\t\t}," << endl;
        }
        cout << "\t\t}," << endl;

        // print all the edges
        cout << "\t\t\"edges\": {" << endl;
        for (int i = 0; i < allEdges.size(); i++) {
            cout << "\t\t\t\"" << allEdges[i][0] << "-" << allEdges[i][1] << "\": {" << endl;
            cout << "\t\t\t\t\"start\": "    << allEdges[i][0]       << "," << endl;
            cout << "\t\t\t\t\"end\": "      << allEdges[i][1]       << "," << endl;
            cout << "\t\t\t\t\"weight\": "   << allEdges[i][2]       << "," << endl;
            cout << "\t\t\t}," << endl;
        }
        cout << "\t\t}," << endl;

        // ending everything else
        cout << "\t}," << endl;
        cout << "]" << endl;
    }
};

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

// creates an edge between two nodes. saves a line.
vector<int> createEdge(int start, int stop, int weight=1) {
    vector<int> nextEdge {start, stop, weight};
    return nextEdge;
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
