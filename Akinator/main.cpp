#include "akinator.h"

int main () {
    FILE* file = fopen ("2.db", "r");
    Node* tree = readDB (file);
    fclose (file);
    play (tree);
    file = fopen ("2.db", "w");
    writeDB (file, tree);
    fclose (file);
    removeTree (tree);
    return 0;
}