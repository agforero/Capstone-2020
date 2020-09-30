export class NodeData {
    constructor(ID, edges=[], val=null) {
        // the ID of this node; a unique value that helps identify this node in particular.
        this.ID = ID;

        // the value (actual contents) of this node; could be a number, a string, a color(?), whatever.
        if (val) this.val = val; 
        else this.val = ID;

        // the edges branching out of this node.
        // an edge is stored as [toNodeID, weight], i.e. [2, 1] in an unweighted graph.
        this.edges = edges; 
    }
    addEdge(ID, weight=1) { // if they use separate vals / just the ID 
        this.edges.push([ID, weight]);
    }
}

export class GraphData {
    constructor(type=1, nodeCount=8) {
        /* type values:
        1) no type yet; just disjointed nodes
        2) a simple linked list (directed)
        3) binary tree
        4) weighted binary tree
        5) undirected graph
        6) directed acyclic graph
        7) weighted graph (aka, edges have distance)
        8) weighted graph, with directional edges
        */

        // attributes
        this.directed = false;      // whether or not the edges are directed. could change depending on the graph. default false
        this.weighted = false;      // whether or not the edges are weighted. could change depending on the graph. default false
        this.allNodes = [];         // all NodeDatas; stored as a list of NodeData objects
        this.head = null;           // the head node; starts unspecified

        // this var is so that all IDs are unique, and makes deletion easier
        // it keeps track of how many nodes have ever existed. when a new node is created,
        // it uses this count to assign its ID, so that all IDs will be unique.
        this.allNodesEver = 0; 

        // now, for each graph, we dictate how it is generated, and how the attributes are changed
        switch (type) {
            case 1:
                this.generate_NONE(nodeCount);
                break;
            case 2:
                this.generate_LINKEDLIST(nodeCount);
                break;
            case 3:
                this.generate_BINTREE(nodeCount);
                break;
        }
    }

    // generation methods. decided to separate them out to here so things are more legible
    generate_NONE(nodeCount) {
        for (let i = 0; i < nodeCount; i++) {
            this.data_addNode();
        }
    }

    generate_LINKEDLIST(nodeCount) {
        for (let i = 0; i < nodeCount; i++) {
            this.data_addNode([i+1, 1]);
        }
    }

    generate_BINTREE(nodeCount) {
        for (let i = 0; i < nodeCount; i++) {
            this.data_addNode();
        }

        // adding edges here is a meme but here we go
        let power = 2; 
        let layer = []; let allLayers = [];
        for (let i = 0; i < self.allNodes.length; i++) {
            layer.push(self.allNodes[i].ID)
            if (i+1 === power-1) {                  // if i+1 is a power of 2
                allLayers.push(layer);              // add this layer to allLayers
                layer = [];                         // reset this layer
                power *= 2;                         // double current power
            }
        } // at this point allLayers holds the IDs of all the nodes, with subarrays of doubling length (1, 2, 4, 8...)
        if (i+1 !== power-1) allLayers.push(layer); // if the last layer was still in progress but not added, add it

        // now we add the edges. we use [i+1] to refer to the next layer, and [(j*2)] and [(j*2)+1] to access the 
        // left and right nodes beneath the node being processed. 
        for (let i = 0; i < allLayers.length; i++) {
            for (let j = 0; j < allLayers[i].length; j++) {
                try {
                    allLayers[i][j].addEdge(allLayers[i+1][(j*2)]);
                    allLayers[i][j].addEdge(allLayers[i+1][(j*2)+1]);
                }
                catch(err) { break; } // lazy way of getting it to break once we hit inevitable index error
            }
        }
    }

    // should put these in a new file, since data_addNode() changes depending on the graph type.

    // add and deletion methods. prepended with "data_" to indicate this is part of graphData.js, 
    // not graphDisplay.js, which must instead add and delete JSX elements themselves
    data_addNode(edges=[], val=null) { 
        this.allNodes.push(new NodeData(this.allNodesEver++, edges, val)); 
    }

    data_deleteNode(ID) { // remove node idx from allNodes
        // scan IDs to make sure its ok first
        let found = false; let idx = 0; // idx holds index of where ID is located in this.allNodes
        for (let i = 0; i < self.allNodes.length; i++) {
            if (self.allNodes[i] === ID) { 
                found = true;
                idx = i;
            }
        }
        if (!found) {
            console.log("ID " + toString(ID) + " not in graph.");
            return -1;
        }

        // using .splice() to surgically remove element
        this.allNodes.splice(idx, 1);
        
        // removing all edges that lead to this element
        for (let i = 0; i < this.allNodes.length; i++) {                // for every node,
            for (let j = 0; j < this.allNodes[i].edges.length; j++) {   // for every edge in current node,
                if (this.allNodes[i].edges[j][0] === ID) {              // if toNodeID is the ID being removed,
                    this.allNodes[i].edges.splice(j, 1); break;         // splice out that edge and break to next node
                }
            }
        }
    }
}