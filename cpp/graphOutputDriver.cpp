#include "graphOutput.hpp"

int main(int argc, char ** argv) {
    Graph granny(DAG);
    granny.dataToJSON(); 
    return 0;
}