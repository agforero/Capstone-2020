#include <iostream>
#include <string>
#include <vector>
#ifndef _NC_H_
#define _NC_H_
using namespace std;

struct Edge {
    int start;  // stored as ID
    int stop;   // stored as ID
    int weight = 1;
    bool highlight = false;
};

class Node {
private:
    string val;
    string annotation = "inf?";
    vector<int> position {};
    bool visited = false;
    int ID;
    Edge * parentPath = NULL;

public:
    // constructor and destructor
    Node(int i, string v = "") {
        ID = i;
        val = v;
        if (i == 0) {
            visited = true; // head always visited implicitly
            annotation = "0";
        }
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
    vector<int> getPosition() {
        return position;
    }
    bool isVisited() {
        return visited;
    }
    string getAnnotation() {
        return annotation;
    }
    int annoToInt() {
        if (annotation == "inf?") {
            return -1;
        }
        else {
            return atoi(annotation.c_str());
        }
    }
    Edge * getParentPath() {
        return parentPath;
    }

    // setters
    void setVal(string s) {
        val = s;
    }
    void setPosition(int x, int y) {
        position = {x, y};
    }
    void setVisited() {
        visited = true;
    }
    void setAnnotation(int dist) {
        annotation = to_string(dist);
    }
    void setAnnotation(string ann){
        annotation = ann;
    }
    void setParentPath(Edge * pp) {
        parentPath = pp;
    }
};

#endif /* _NC_H_ */