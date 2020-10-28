#include "graphOutputHeader.hpp"

// a bunch of disconnected nodes. none of them have edges connecting each other. pretty straightforward.
void Graph::generateNONE() {
    for (int i = 0; i < startCount; i++) {
        allNodes.push_back(createNode());
        allNodes[i]->setPosition(((i + 1) * 50), ((i + 1) * 50));
    }
}

/* a directed acyclic graph. this is a graph that cannot be cycled infinitely using any given path, starting 
at any given node. thus, it has startCount nodes each with, let's say, 0 to 3 edges: but these edges cannot
point backwards. they can point to a node further down the line, but they cannot point backwards, as this 
might allow a cycle. can't have that. bad for business. it should also point to its next neighbor. */
void Graph::generateDAG() {
    for (int i = 0; i < startCount; i++) {
        allNodes.push_back(createNode());
    }

    // last two nodes are special cases here:
    // second-to-last only points to last node, and nothing else.
    // last node finishes the graph, so it points to nothing.

    // first, make it basically generate a linked list
    for (int i = 0; i < startCount - 1; i++) {
        allEdges.push_back(createEdge(i, i + 1));
        allNodes[i]->setPosition(((i + 1) * 50), ((i + 1) * 50)); // we'll need curved arrows or something for this
    }

    // then, for every Node, have it randomly point to a node further down the line
    for (int i = 0; i < startCount - 2; i++) { // -2 so we DON'T mess with the last two Nodes
        int randoms = rand() % 3 + 1;
        vector<int> newConns;

        for (int k = 0; k < randoms; k++) {
            bool unique = true;
            int range1 = allNodes.size() - i - 2;
            int range2 = (range1 != 0) ? rand() % range1 : 0;
            int nextCandidate = i + 2 + range2;

            for (auto con : newConns) {
                if (con == nextCandidate || nextCandidate == allNodes.size())
                    unique = false;
            }
            if (unique)
                newConns.push_back(nextCandidate); // add to newConns
        }

        for (int j = 0; j < newConns.size(); j++) {
            allEdges.push_back(createEdge(i, newConns[j])); // add these edges to the DAG
        }
    }
}

// a string of nodes, each pointing to the next one in line.
void Graph::generateLINKEDLIST() {
    for (int i = 0; i < startCount; i++) {
        allNodes.push_back(createNode());
    }
    for (int i = 0; i < startCount - 1; i++) {
        allEdges.push_back(createEdge(i, i + 1));
    }

    // setting positions
    for (int i = 0; i < startCount; i++) {
        allNodes[i]->setPosition((i + 1) * 50, (i + 1) * 50);
    }
}

/* a tree with anywhere between 1 to variability connections per node per layer before continuing.
this works a lot better with longer node counts, like 16. */
void Graph::generateTREE() {
    for (int i = 0; i < startCount; i++) {
        allNodes.push_back(createNode());
    }

    vector<int> currentLayer {0};
    vector<int> nextLayer;
    int depth = 1;
    int width = 500;
    int nodesPlaced = 0; // keeps track of how many nodes we've positioned so far

    queue<int> availableNodes;
    for (int i = 1; i < startCount; i++) {
        availableNodes.push(i);
    }

    while (!currentLayer.empty()) {
        // set positions for each node in current layer
        // we will assume width of div is 500 for now; we can tune this later if need be. see above for width
        int spacing = width / (currentLayer.size() + 1); // there will be this many px between each node.
        for (int i = 0; i < currentLayer.size(); i++) {
            allNodes[nodesPlaced++]->setPosition(((i + 1) * spacing), (depth * 50));
        }
        depth++;

        for (int i = 0; i < currentLayer.size(); i++) {
            int children = rand() % variability + 2;
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

// generates a grid of nodes.
void Graph::generateGRID() {
    int currentColumn = 1; 
    for (int i = 0; i < startCount; i++) {
        allNodes.push_back(createNode());
        allNodes[i]->setPosition(50 * currentColumn++, ((i / gridWidth) + 1) * 50);
        if (currentColumn > gridWidth) currentColumn = 1;
    }

    // draw edges. this is a bit tricky.
    // for each node, all neighbors east, south and southeast should have a 3/4 chance of having an edge forward.
    /* example:
    A - B
    | \
    C   D
    where all three edges have a 3/4 chance of existing.
    */

    for (int i = 0; i < allNodes.size(); i++) {
        vector<int> forwardCandidates = findForwardConns(i);
        vector<int> backwardsCandidates = findBackwardsConns(i);

        for (int j = 0; j < forwardCandidates.size(); j++) {
            bool wontFire = ((rand() % 4) == 0); // 1 in 4 chance not to create an edge in this case.
            if (!wontFire) {
                allEdges.push_back(createEdge(allNodes[i]->getID(), forwardCandidates[j]));
            }
        }
        
        for (int j = 0; j < backwardsCandidates.size(); j++) {
            bool willFire = ((rand() % 8) == 0); // 1 in 8 chance to cycle back to a previous neighbor.
            if (willFire) {
                allEdges.push_back(createEdge(allNodes[i]->getID(), backwardsCandidates[j]));
            }
        }
    }
}