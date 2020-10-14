#include "graphOutput.hpp"

int main(int argc, char ** argv) {
    if (argc == 2) {
        Graph granny(DAG, atoi(argv[1]));
        granny.dataToJSON(); 
    }
    else {
        Graph granny(DAG);
        granny.dataToJSON(); 
    }
    return 0;
}