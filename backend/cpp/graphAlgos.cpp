#include "graphOutputHeader.hpp"

// Prim's Algorithm. See https://www.geeksforgeeks.org/prims-minimum-spanning-tree-mst-greedy-algo-5/ for reference
void Graph::prims() {
    if (allNodes.size() == 0) return; // please no segfaulterino

    dataToJSON(); // print initial state
    vector<int> visited {0}; // indexes of visited Nodes; starts only as head node
    for (int i = 0; i < allNodes.size(); i++) { // this will run a MAXIMUM of allNodes.size() times.
        // get all possible traversals from our current visited
        vector<vector<int>> possibilities;
        for (auto idx: visited) {
            for (auto e: getEdges(idx)) {
                if (!allNodes[e[1]]->isVisited()) possibilities.push_back(e); // if THAT node isn't visited, add this edge to possibilities.
            }
        }

        // then, from pool of edges, find closest neighbor
        if (possibilities.size() == 0) break; // if no possible traversals, break immediately
        int minWeight = possibilities[0][2]; 
        int closestNeighbor = possibilities[0][1];
        for (auto e: possibilities) {
            if (e[2] < minWeight) {
                minWeight = e[2];
                closestNeighbor = e[1];
            }
        }

        // finally, visit that neighbor and add it to visited[] from above
        allNodes[closestNeighbor]->visit();
        visited.push_back(closestNeighbor);
        dataToJSON(); // print updated state from this step.
    }
}