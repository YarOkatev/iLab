
#include "compiler.h"

int main () {
	FILE* f = fopen ("prog1", "r");
	int a, b;

	FILE* g = fopen ("cmdlist.txt", "r");
	struct definedCommand* cl = generateCommandList (g, &b);
	struct userCommand* uc = readUserProgram (f, &a);
	generateMachineCode (cl, uc, b, a);
}