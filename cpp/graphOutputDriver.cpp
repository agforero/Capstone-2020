#include "graphOutput.hpp"

int main(int argc, char ** argv) {
    Graph granny(argc, argv);
    granny.dataToJSON();
    return 0;
}