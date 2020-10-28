#include "graphOutputHeader.hpp"

int main(int argc, char ** argv) {
    Graph granny(argc, argv);
    granny.prims();
    return 0;
}