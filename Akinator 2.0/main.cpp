#include "akinator.h"

int main () {
    char* fileName = const_cast<char*>("data.db");
    FILE* file = fopen (fileName, "r");
    Node* tree = readDB (file);
    play (tree);
    file = fopen (fileName, "w");
    writeDB (file, tree);
    fclose (file);
    removeTree (tree);
    return 0;
}