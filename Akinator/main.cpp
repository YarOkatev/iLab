#include "akinator.h"

int main () {
    FILE* file = fopen ("data.db", "r");
    Node* tree = readDB (file);
    fclose (file);
    play (tree);
    file = fopen ("data.db", "w");
    writeDB (file, tree);
    fclose (file);
    removeTree (tree);
    return 0;
}
