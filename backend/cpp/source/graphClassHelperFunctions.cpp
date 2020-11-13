#ifndef _GCHF_H_
#define _GCHF_H_
#include "graphMainHeader.hpp"

string boolToString(bool b){
    if(b){
        return "true";
    }else{
        return "false";
    }
}

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
/*vector<int> Graph::createEdge(int start, int stop) {
    vector<int> nextEdge{start, stop, (rand() % weight + 1)};
    return nextEdge;
}*/
Edge* Graph::createEdge(int start, int stop) {
    Edge *ret = new Edge;
    ret->start = start;
    ret->stop = stop;
    ret->weight = rand() % weight+1;
    ret->highlight = false;
    return ret;
}

// gets the "ID" for an edge in string form.
string Graph::getEdgeID(Edge* edge) {
    string ret = to_string(edge->start); ret.append("-"); ret.append(to_string(edge->stop));
    return ret;
}

vector<int> Graph::remainingToPrint(vector<bool> printed) {
    vector<int> ret;
    for (int i = 0; i < printed.size(); i++) {
        if (!printed[i]) ret.push_back(i);
    }
    return ret;
}

// prints the currentStack.
void Graph::printStack() {
    cout << "\t\t\"stack\": [" << endl;
    
    for (int i = 0; i < currentStack.size(); i++) {
        cout << "\t\t\t\"" << getNode(currentStack[i][0])->getVal() << ":" << currentStack[i][1] << "\"";
        if (i < currentStack.size()-1) cout << ","; // don't print comma for last entry
        cout << endl;
    }

    cout << "\t\t]" << endl;
}

// JSON output functions. you know, this would have been easier with printf(), but that's ok :)
void Graph::dataToJSON() {
    cout << "{" << endl;
    cout << "\t\"type\": \"animation\"," << endl;
    cout << "\t\"currentLine\": " << currentLine << "," << endl;

    // print all the nodes
    cout << "\t\"nodes\": {" << endl;
    for (int i = 0; i < allNodes.size(); i++) {
        int x = allNodes[i]->getPosition()[0];
        int y = allNodes[i]->getPosition()[1];

        cout << "\t\t\"" << allNodes[i]->getID() << "\": {" << endl;

        cout << "\t\t\t\"id\": "            << allNodes[i]->getID()                 << "," << endl;
        cout << "\t\t\t\"name\": \""        << allNodes[i]->getVal()                << "\"," << endl;

        cout << "\t\t\t\"position\": {"                                             << endl;
        cout << "\t\t\t\t\"x\": "           << x << ","                             << endl;
        cout << "\t\t\t\t\"y\": "           << y                                    << endl;
        cout << "\t\t\t},"                                                          << endl;

        cout << "\t\t\t\"color\": "         << vectorColor(i)                       << "," << endl;
        cout << "\t\t\t\"annotation\": \""  << allNodes[i]->getAnnotation()         << "\"," << endl;
        cout << "\t\t\t\"bold\": "          << isBold(i)                            << endl;

        if (i != allNodes.size() - 1) cout << "\t\t}," << endl;
        else cout << "\t\t}" << endl;
    }
    cout << "\t}," << endl;

    // print all the edges
    cout << "\t\"edges\": {" << endl;
    for (int i = 0; i < allEdges.size(); i++) {
        cout << "\t\t\"" << allEdges[i]->start << "-" << allEdges[i]->stop << "\": {" << endl;

        cout << "\t\t\t\"id:\": \""     << getEdgeID(allEdges[i])   << "\"," << endl;
        cout << "\t\t\t\"start\": "     << allEdges[i]->start           << "," << endl;
        cout << "\t\t\t\"end\": "       << allEdges[i]->stop           << "," << endl;
        cout << "\t\t\t\"weight\": "    << allEdges[i]->weight           << "," << endl;

        cout << "\t\t\t\"highlight\": " << boolToString(allEdges[i]->highlight) << endl;
        
        if(i==allEdges.size()-1){
            cout << "\t\t}" << endl;
        }else{
            cout << "\t\t}," << endl;
        }
    }

    cout << "\t}," << endl;

    printStack();

    // ending everything else
    cout << "}," << endl;
}

// displays the graph, but only the visited vertices, and the used edges.
void Graph::dataToJSON_FILTERED() {
    cout << "{" << endl;
    cout << "\t\"type\": \"animation\"," << endl;
    cout << "\t\"currentLine\": " << currentLine << "," << endl;

    // print all the nodes
    cout << "\t\"nodes\": {" << endl;

    for (auto i: getVisited()) {
        int x = allNodes[i]->getPosition()[0];
        int y = allNodes[i]->getPosition()[1];

        cout << "\t\t\"" << allNodes[i]->getID() << "\": {" << endl;

        cout << "\t\t\t\"id\": "            << allNodes[i]->getID()                 << "," << endl;
        cout << "\t\t\t\"name\": \""        << allNodes[i]->getVal()                << "\"," << endl;

        cout << "\t\t\t\"position\": {"                                             << endl;
        cout << "\t\t\t\t\"x\": "           << x << ","                             << endl;
        cout << "\t\t\t\t\"y\": "           << y                                    << endl;
        cout << "\t\t\t},"                                                          << endl;

        cout << "\t\t\t\"color\": "         << vectorColor(i)                       << "," << endl;
        cout << "\t\t\t\"annotation\": \""  << allNodes[i]->getAnnotation()         << "\"," << endl;
        cout << "\t\t\t\"bold\": "          << isBold(i)                            << endl;

        if (i != allNodes.size() - 1) cout << "\t\t}," << endl;
        else cout << "\t\t}" << endl;
    }
    cout << "\t}," << endl;

    // print all the edges
    cout << "\t\"edges\": {" << endl;
    for (int i = 0; i < usedEdges.size(); i++) {
        cout << "\t\t\"" << usedEdges[i]->start << "-" << usedEdges[i]->stop << "\": {" << endl;

        cout << "\t\t\t\"id:\": \""     << getEdgeID(usedEdges[i])  << "\"," << endl;
        cout << "\t\t\t\"start\": "     << usedEdges[i]->start          << "," << endl;
        cout << "\t\t\t\"end\": "       << usedEdges[i]->stop          << "," << endl;
        cout << "\t\t\t\"weight\": "    << usedEdges[i]->weight          << endl;

        cout << "\t\t}," << endl;cout << "\t\t}" << endl;
    }
    
    printStack();
    cout << "\t}" << endl;

    // ending everything else
    cout << "}," << endl;
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
    else if ((idx+1 % gridWidth) == 0) { // it's in the rightmost column
        vector<int> ret {};
        vector<int> candidates {idx-gridWidth-1, idx-gridWidth, idx-1, idx+gridWidth-1}; // NW, N, W, and SW respectively. no NE for rightmost column, duh
        for (auto i: candidates) {
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

void Graph::cleanupEdges() {
    vector<Edge *> cleaned;
    for (auto e1: allEdges) {
        bool alreadyIn = false;
        for (auto e2: cleaned) {
            if (e2->start == e1->stop && e2->stop == e1->start) alreadyIn = true;
        }
        if (!alreadyIn) cleaned.push_back(e1);
    }
    allEdges = cleaned;
}

// returns a hex color, depending on whether or not idx has been visited.
string Graph::vectorColor(int ID) {
    if (getNode(ID)->isVisited()) return "\"#00a600\"";
    else return "\"#f00\"";
}

// given index ID in allNodes, return all edges branching out from that node.
vector<Edge *> Graph::getEdges(int ID) {
    vector<Edge *> ret;
    for (auto edge: allEdges) {
        if (edge->start == ID) ret.push_back(edge);
    }
    return ret;
}

vector<Edge *> Graph::getUnvisitedEdges(int ID) {
    vector<Edge *> ret;
    for (auto edge: allEdges) {
        if ((edge->start == ID) && (!getNode(edge->stop)->isVisited())) ret.push_back(edge);
    }
    return ret;
}

// takes in an ID and returns a pointer to that exact Node.
Node * Graph::getNode(int ID) {
    for (auto n : allNodes) {
        if (n->getID() == ID) return n;
    }
    return nullptr;
}

// visit and embolden this node.
void Graph::visit(int ID) {
    getNode(ID)->setVisited();
    bolded = ID;
}

// embolden this node, but don't visit it quite yet
void Graph::consider(int ID){
    bolded = ID;
}

void Graph::unbold(){
    bolded = INT32_MAX;
}

void Graph::addToStack(int ID, int distance) {
    // first check if the ID is already in the currentStack. if so, update instead, and return.
    for (auto e: currentStack) {
        if (e[0] == ID) {
            removeFromStack(ID);
            addToStack(ID, distance);
            return;
        }
    }

    // we keep cycling through until we either reach the end, or find something with a distance GREATER OR EQUAL TO than the one we're inserting.
    // at this point, we insert one before this encountered element.
    for (int i = 0; i < currentStack.size(); i++) {
        if (currentStack[i][1] > distance) {
            currentStack.insert(currentStack.begin() + i, {ID, distance}); 
            return;
        }

        /* if we run into a distance EQUAL to the argument passed, place the argument ID at the end of that group of distances.
        example: argument distance = 4, distances = (1, 2, 4, 4, 4, 5) -> place new ID before the 5.
        this doesn't seem to fix the issue of equidistant Nodes not displaying in the stack relative to which will be chosen.
        we could fix this by displaying something like ID:DISTANCE, where the user can see that the nodes are equidistant,
        and are thus arbitrarily chosen. */
        else if (currentStack[i][1] == distance) {
            int j = i;
            while (true) {
                // we get to the end of the stack
                if (j >= currentStack.size()) {
                    currentStack.push_back({ID, distance});
                    return;
                }

                // we still have the group to get to the end of
                else if (currentStack[j][1] == distance) {
                    j++;
                    continue;
                }

                // we finally find the end of this group of distances
                else if (currentStack[j][1] > distance) {
                    currentStack.insert(currentStack.begin() + j, {ID, distance}); 
                    return;
                }
            }
        }
    }
    currentStack.push_back({ID, distance}); // only reaches here if it never finds something with greater distance than itself.
}

// removes a given ID from the currentStack.
void Graph::removeFromStack(int ID) {
    for (int i = 0; i < currentStack.size(); i++) {
        if (currentStack[i][0] == ID) {
            currentStack.erase(currentStack.begin() + i);
            return;
        }
    } // should never reach this point.
}

// get all indeces of visited nodes within allNodes.
vector<int> Graph::getVisited() {
    vector<int> ret;
    for (int i = 0; i < allNodes.size(); i++) {
        if (allNodes[i]->isVisited()) ret.push_back(i);
    }
    return ret;
}

// convert idx within allNodes to its actual ID
int Graph::idxToID(int idx) {
    return allNodes[idx]->getID();
}

// checks if this node is the currently bolded node.
string Graph::isBold(int ID) {
    if (bolded == ID) return "true";
    else return "false";
}

#endif /* _GCHF_H_ */