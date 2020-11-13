#include "../source/graphMainHeader.hpp"

int main(int argc, char ** argv) {
    Graph granny(argc, argv);
    granny.dijkstras();
    return 0;
}