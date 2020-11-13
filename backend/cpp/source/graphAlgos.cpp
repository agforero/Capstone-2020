#include "graphMainHeader.hpp"

// Prim's Algorithm. See https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/ for reference
void Graph::prims() {
    if (allNodes.size() == 0) return; // please no segfaulterino

    cout << "[" << endl;
    dataToJSON(); // print initial state

    vector<int> visited {0}; // indexes of visited Nodes; starts only as head node
    for (int i = 0; i < allNodes.size(); i++) { // this will run a MAXIMUM of allNodes.size() times.
        // get all possible traversals from our current visited
        vector<Edge *> possibilities;
        for (auto idx: visited) {
            for (auto e: getEdges(idx)) {
                if (!getNode(e->stop)->isVisited()) possibilities.push_back(e); // if THAT node isn't visited, add this edge to possibilities.
            }
        }

        // then, from pool of edges, find closest neighbor
        if (possibilities.size() == 0) break; // if no possible traversals, break immediately
        int minWeight = possibilities[0]->weight; 
        int closestNeighbor = possibilities[0]->stop;
        Edge* used = possibilities[0];
        for (auto e: possibilities) {
            if (e->weight < minWeight) {
                minWeight = e->weight;
                closestNeighbor = e->stop;
                used = e;
            }
        }

        // finally, visit that neighbor and add it to visited[] from above
        visit(closestNeighbor);
        visited.push_back(closestNeighbor);
        usedEdges.push_back(used);
        dataToJSON(); // print updated state from this step.
    }

    dataToJSON_FILTERED();
    cout << "{\"type\": \"end\"}]" << endl;
}

// depth first search where we attempt to find a specific node. t is ID of target node.
void Graph::DFS(int current) {
    visit(current);

    if (current == 0) cout << "[" << endl; // initial bracket
    dataToJSON(); // print current state; inclusive of when we've found it

    if (getNode(current)->getID() == target) {
        cout << "{\"type\": \"end\"}]" << endl;
        exit(0); // if target found, end the program immediately. please work.  
    } 

    vector<Edge *> possibilities = getEdges(current);
    for (auto e: possibilities) {
        if (!getNode(e->stop)->isVisited()) DFS(e->stop);
    }

    if (current == 0) cout << "{\"type\": \"end\"}]" << endl; // it should only do this if we never found the target.
}

void Graph::BFS(int current) {
    cout << "[" << endl;

    vector<int> currentLayer {0}; 
    vector<int> nextLayer {};

    while (!currentLayer.empty()) {
        for (int i = 0; i < currentLayer.size(); i++) {
            for (auto e: getEdges(currentLayer[i])) {
                if (!getNode(e->stop)->isVisited()) nextLayer.push_back(e->stop);
            }
        }

        // we've had our first buffer established, we visit each in order, outputting data to JSON every time.
        // if we ever encounter the searched for node, we stop immediately, just like in DFS() above.
        for (int i = 0; i < currentLayer.size(); i++) {
            visit(currentLayer[i]);
            dataToJSON();
            if (getNode(currentLayer[i])->getID() == target) { // if target found, exit immediately
                cout << "{\"type\": \"end\"}]" << endl;
                exit(0);
            }
        }

        // finally, shift our current layer down, and redeclare nextLayer as empty.
        currentLayer = nextLayer;
        nextLayer.clear();
    }

    // will only hit this point if it never finds the target.
    cout << "{\"type\": \"end\"}]" << endl;
}

void Graph::dijkstras(){
    cout << "[" << endl;
    map<int, int> dist;         // map of all distances
    map<int, bool> vertices;    // ID --> explored?
    dist[0] = 0;

    for (auto n : allNodes){
        if (n->getID() != 0){               // if not source
            dist[n->getID()] = INT32_MAX;   // couldn't ever be this high, so this is "infinity"
        }
        vertices[n->getID()] = false;
    }

    while (true){
        // first, find Node in vertices with minimum distance from source. cannot be a Node with -1 dist.
        int minDist = INT32_MAX;
        int minID; // ID of closest Node

        // Find minimum distance
        for (auto it = vertices.begin(); it != vertices.end(); it++){
            if (!it->second && dist[it->first] < minDist){
                minDist = dist[it->first];
                minID = it->first;
            }
        }

        if (minDist == INT32_MAX){
            // Didn't find any new nodes, done running algo
            break;
        }

        getNode(minID)->setAnnotation(minDist);
        visit(minID);
        vertices[minID] = true;
        removeFromStack(minID);
        currentLine = 7;

        // Highlight path to parent
        Edge* currentPath = getNode(minID)->getParentPath();
        while(currentPath != NULL){
            currentPath->highlight=true;
            Node* parent = getNode(currentPath->start);
            if(parent==NULL){
                break;
            }
            currentPath = parent->getParentPath();
        }

        dataToJSON();

        // Unhighlight path to parent
        currentPath = getNode(minID)->getParentPath();
        while(currentPath != NULL){
            currentPath->highlight=false;
            Node* parent = getNode(currentPath->start);
            if(parent==NULL){
                break;
            }
            currentPath = parent->getParentPath();
        }

        // then, for every neighbor of minID not yet removed from vertices,
        vector<Edge *> neighs = getEdges(minID);
        for (auto e : neighs){
            if (vertices[e->stop]){
                continue; // already explored, don't do it
            }

            consider(e->stop);
            currentLine = 9;
            dataToJSON();

            // we are now referring to a neighbor that is still in vertices.
            int alt = dist[minID] + e->weight;
            if (alt < dist[e->stop]){
                // a shorter path to e[1] has been found; update.
                dist[e->stop] = alt;
                getNode(e->stop)->setAnnotation(to_string(alt) + "?");
                getNode(e->stop)->setParentPath(e);
                currentLine = 12;
                addToStack(e->stop, dist[e->stop]);
                dataToJSON();
            }
        }

        unbold();
        currentLine = 6;
        dataToJSON();

        // TODO: iterate through vertices, if all explored then break
        // alternatively, if we didn't explore anything new this time, assume we're done.
    }

    cout << "{\"type\": \"end\"}]" << endl;
}
