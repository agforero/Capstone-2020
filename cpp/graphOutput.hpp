#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;

// the thing Richey once said was a bad idea but here I go anyways
#define CC cout <<
#define EE << endl

int itest = 0;
#define ITEST cout << "TEST #" << itest++ << endl;

// TYPE enumerator
enum TYPE {
    NONE,   // no type yet; just disjointed nodes
    DAG     // directed acyclic graph
};

// Node class declaration
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

// Helper functions
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

// creates a Node using New, with a val adaptive to alpha, and returns.
Node * createNode(int ID, bool alpha) {
    if (alpha) {
        Node * temp = new Node(ID, intToAlphaID(ID));
        return temp;
    }
    else {
        Node * temp = new Node(ID, to_string(ID));
        return temp;
    }
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

// Graph class declaration
class Graph {
private:
    vector<Node *> allNodes;        // vector of all nodes
    vector<vector<int>> allEdges;   // vector of vectors of all edges; stored as {thisID, thatID, weight=1}

    int everCount = 0;              // how many Nodes have ever existed; helps with creating unique IDs
    int startCount = 8;             // how many Nodes to start; default 8 as per research

    bool directed;                  // whether or not the edges are directed
    bool alpha = true;              // whether or not Node vals should be alphabetical, default true.

public:
    // constructor
    Graph(TYPE t=NONE, int s=8, bool a=true) {
        srand(time(NULL)); // start random seed; imperative for random generation later
        startCount = s;
        alpha = a;

        switch(t) {
            case NONE:
                generateNONE();
                break;
            case DAG:
                generateDAG();
                break;
        }
    }

    // generation functions
    void generateNONE() {
        // a bunch of disconnected nodes. none of them have edges connecting each other. pretty straightforward.

        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode(everCount++, alpha));
        }
    }

    void generateDAG() {
        /*
        a directed acyclic graph. this is a graph that cannot be cycled infinitely using any given path, starting 
        at any given node. thus, it has startCount nodes each with, let's say, 0 to 3 edges: but these edges cannot
        point backwards. they can point to a node further down the line, and sometimes to themselves, but they cannot 
        point backwards, as this might allow a cycle. can't have that. bad for business.

        it should also point to its next neighbor.
        */

        // start by generating all Nodes
        for (int i = 0; i < startCount; i++) {
            allNodes.push_back(createNode(everCount++, alpha));
        }

        // then, for every Node, have it randomly point to a node further down the line, and rarely to itself
        for (int i = 0; i < allNodes.size()-1; i++) { // -1 so we DON'T mess with the last Node
            // neighbor down the line
            allEdges.push_back(createEdge(i, i+1));

            // 1 to 3 random Nodes up ahead
            int randoms = rand() % 3 + 1;
            vector<int> newConns;
            for (int i = 0; i < randoms; i++) {
                int nextCandidate = (rand() % (allNodes.size() - i - 1)) + (i + 1); // gens rand number i+1 to end
                newConns.push_back(nextCandidate); // add to newConns
            }
            for (int j = 0; j < newConns.size(); j++) {
                allEdges.push_back(createEdge(i, newConns[j])); // add these edges to the DAG
            }

            // rarely, itself
            if (rand() % 20) { // 1 in 20 chance(?)
                allEdges.push_back(createEdge(i, i));
            }
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
        CC "let Frames = [" EE;
        CC "\t{" EE;
        CC "\t\t\"type\": \"animation\"," EE; // brain hurty

        // print all the nodes
        CC "\t\t\"nodes\": {" EE;
        for (int i = 0; i < allNodes.size(); i++) {
            CC "\t\t\t\"" << allNodes[i]->getID() << "\": {" EE;
            CC "\t\t\t\t\"id\": "           << allNodes[i]->getID()                             << "," EE;
            CC "\t\t\t\t\"name\": \""       << allNodes[i]->getVal()                            << "\"," EE;
            CC "\t\t\t\t\"position\": { "   << "\"x\": " << 50*(i+1) << ", \"y\": " << 50*(i+1) << " }," EE;
            CC "\t\t\t\t\"color\": "        << vectorColor(i)                                   << "," EE;
            CC "\t\t\t\t\"annotation\": "   << getAnnotation(i)                                 << "," EE;
            CC "\t\t\t}," EE;
        }
        CC "\t\t}," EE;

        // print all the edges
        CC "\t\t\"edges\": {" EE;
        for (int i = 0; i < allEdges.size(); i++) {
            CC "\t\t\t\"" << allEdges[i][0] << "-" << allEdges[i][1] << "\": {" EE;
            CC "\t\t\t\t\"start\": "    << allEdges[i][0]       << "," EE;
            CC "\t\t\t\t\"end\": "      << allEdges[i][1]       << "," EE;
            CC "\t\t\t\t\"weight\": "   << allEdges[i][2]       << "," EE;
            CC "\t\t\t}," EE;
        }
        CC "\t\t}," EE;

        // ending everything else
        CC "\t}," EE;
        CC "]" EE;
    }
};