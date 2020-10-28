#ifndef _GCHF_H_
#define _GCHF_H_
#include "graphOutputHeader.hpp"

// creates a Node using new, with a val adaptive to alpha, and returns.
Node * Graph::createNode() {
    if (alpha) {
        Node * temp = new Node(everCount, intToAlphaID(everCount));
        everCount++;
        return temp; // notice, it increments everCount here,
    }
    else {
        Node * temp = new Node(everCount, to_string(everCount));
        everCount++;
        return temp; // so that the next node WILL have unique ID
    }
}

// creates an edge between two nodes. saves a line.
vector<int> Graph::createEdge(int start, int stop) {
    vector<int> nextEdge{start, stop, (rand() % weight + 1)};
    return nextEdge;
}

// returns an annotation value depending on adjacency to head node.
string Graph::getAnnotation(int ID) {
    string ret = "\"inf?\""; // assumed infinite if not special case
    // if this node IS the head
    if (ID == 0) {
        ret = "\"0\"";
        return ret;
    }
    for (int i = 0; i < allEdges.size(); i++) {
        // if this node is connected to head (ID = 0)
        if (allEdges[i][0] == 0 && allEdges[i][1] == ID) {
            ret = "\"";
            ret.append(to_string(ID));
            ret.append("\"");
        }
    }
    return ret; // will either return ID? or inf? at this point
}

// JSON output functions. you know, this would have been easier with printf(), but that's ok :)
void Graph::dataToJSON() {
    cout << "[{" << endl;
    cout << "\t\"type\": \"animation\"," << endl; // brain hurty

    // print all the nodes
    cout << "\t\"nodes\": {" << endl;
    for (int i = 0; i < allNodes.size(); i++) {
        int x = allNodes[i]->getPosition()[0];
        int y = allNodes[i]->getPosition()[1];

        cout << "\t\t\"" << allNodes[i]->getID() << "\": {" << endl;

        cout << "\t\t\t\"id\": "            << allNodes[i]->getID()                 << "," << endl;
        cout << "\t\t\t\"name\": \""        << allNodes[i]->getVal()                << "\"," << endl;
        cout << "\t\t\t\"position\": { "    << "\"x\": " << x << ", \"y\": " << y   << " }," << endl;
        cout << "\t\t\t\"color\": "         << vectorColor(i)                       << "," << endl;
        cout << "\t\t\t\"annotation\": "    << getAnnotation(i)                     << endl;

        if (i != allNodes.size() - 1) cout << "\t\t}," << endl;
        else cout << "\t\t}" << endl;
    }
    cout << "\t}," << endl;

    // print all the edges
    cout << "\t\"edges\": {" << endl;
    for (int i = 0; i < allEdges.size(); i++) {
        cout << "\t\t\"" << allEdges[i][0] << "-" << allEdges[i][1] << "\": {" << endl;

        cout << "\t\t\t\"start\": "     << allEdges[i][0]       << "," << endl;
        cout << "\t\t\t\"end\": "       << allEdges[i][1]       << "," << endl;
        cout << "\t\t\t\"weight\": "    << allEdges[i][2]       << endl;

        if (i != allEdges.size() - 1) cout << "\t\t}," << endl;
        else cout << "\t\t}" << endl;
    }
    cout << "\t}" << endl;

    // ending everything else
    cout << "},{\"type\": \"end\"}]" << endl;
}

// find indexes of nodes south, east, and southeast of allNodes[idx]. for usage in generating a grid.
vector<int> Graph::findForwardConns(int idx) {
    if (((idx+1) % gridWidth) == 0) { // it's a node in the rightmost column
        vector<int> ret {};
        int sideCandidate = idx + gridWidth; // one south of it
        if (sideCandidate < allNodes.size()) ret.push_back(sideCandidate);
        return ret;
    }
    else {
        vector<int> ret {};
        vector<int> candidates {idx+1, idx+gridWidth, idx+gridWidth+1}; // east, south and southeast respectively
        for (auto i: candidates) {
            if (i < allNodes.size()) ret.push_back(i);
        }
        return ret;
    }
}

// find the rest of the connections
vector<int> Graph::findBackwardsConns(int idx) {
    if ((idx % gridWidth) == 0) { // it's a node in the leftmost column
        vector<int> ret {};
        vector<int> sideCandidates {idx-gridWidth, idx-gridWidth+1}; // node north and northeast respectively
        for (auto i: sideCandidates) {
            if (i >= 0 && i < allNodes.size()) ret.push_back(i);
        }
        return ret;
    }
    else {
        vector<int> ret {};
        vector<int> candidates {idx-gridWidth-1, idx-gridWidth, idx-gridWidth+1, idx-1, idx+gridWidth-1}; // NW, N, NE, W, and SW respectively
        for (auto i: candidates) {
            if (i >= 0 && i < allNodes.size()) ret.push_back(i);
        }
        return ret;
    }
}

// returns a hex color, depending on whether or not i == 0.
string Graph::vectorColor(int idx) {
    if (allNodes[idx]->isVisited()) {
        string ret = "\"#0f0\"";
        return ret;
    } else {
        string ret = "\"#f00\"";
        return ret;
    }
}

// given index idx in allNodes, return all edges branching out from that node.
vector<vector<int>> Graph::getEdges(int idx) {
    vector<vector<int>> ret;
    for (auto edge: allEdges) {
        if (edge[0] == idx) ret.push_back(edge);
    }
    return ret;
}

#endif /* _GCHF_H_ */