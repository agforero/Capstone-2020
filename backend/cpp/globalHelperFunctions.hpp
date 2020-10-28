#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <queue>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
using namespace std;
// these include statements carry over to .cpp files feeding into gOH.hpp

// helps create unique alphabetical IDs for each Node.
static string intToAlphaID(int n) {
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

// just...you know. prints an error message.
static void printInvalidArgs() {
    cout << "invalid argument(s)." << endl;
}

// gets next command line arg from this flag; or, if it doesn't exist, throws an error
static int getFlagArg(int argc, char **argv, string flag) {
    for (int i = 1; i < argc - 1; i++) { // skip executable and last arg
        if (flag.compare(argv[i]) == 0) {
            return atoi(argv[i + 1]); // shouldn't be a flag.
        }
    }
    // at this point, we've encountered an error. we can exit(0) immediately
    printInvalidArgs();
    exit(0);
    return -1; // added a return to silence g++
}

static void printHelp() { // by God, is there a better way to do this? perhaps.
    cout << "usage: ./graphOutputDriver graphType (flags)" << endl;
    cout << endl;
    cout << "graphType values:" << endl;
    cout << "0: none" << endl;
    cout << "1: directed acyclic graph" << endl;
    cout << "2: linked list" << endl;
    cout << "3: tree" << endl;
    cout << "4: grid" << endl;
    cout << endl;
    cout << "optional flags available:" << endl;
    cout << "-i\t\tname nodes using integers rather than letters." << endl;
    cout << "-nodes <n>\tforce the creation of n nodes in the graph. default 8." << endl;
    cout << "-w <n>\t\trandomize the edge weights to be integers between 1 and n. by default, n is 1, implying weightlessness." << endl;
    cout << "-v <n>\t\tedit variability; nodes have between 2 and n edges stemming from them. default n is 4. works best with high node counts." << endl;
    cout << "-g <n>\t\tgrid width; in a grid, how many nodes wide it is." << endl;
    cout << endl;
    cout << "run ./graphOutputDriver -h or ./graphOutputDriver --help to display this help menu." << endl;
}