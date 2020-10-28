#include <iostream>
#include <string>
#include <vector>
#ifndef _NC_H_
#define _NC_H_
using namespace std;

class Node {
private:
    string val;
    vector<int> position{};
    bool visited;
    int ID;

public:
    // constructor and destructor
    Node(int i, string v = "") {
        ID = i;
        val = v;
        if (i == 0) visited = true; // head always visited implicitly
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

    // setters
    void setVal(string s) {
        val = s;
    }
    void setPosition(int x, int y) {
        position = {x, y};
    }
    void visit() {
        visited = true;
    }
};

#endif /* _NC_H_ */