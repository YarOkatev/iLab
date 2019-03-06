
#include "compiler.h"

int main () {
	FILE* f = fopen ("prog1", "r");

	readUserProgram (f);
	FILE* g = fopen ("cmdlist.txt", "r");
	generateCommandList (g);
}