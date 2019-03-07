
#include "compiler.h"

int main () {
	FILE* f = fopen ("prog1", "r");
	int a = 0, b = 0;
	FILE* g = fopen ("cmdlist.txt", "r");
	class definedCommand* cl = generateCommandList (g, &b);
	class userCommand* uc = readUserProgram (f, &a);
	generateMachineCode (cl, uc, b, a);
}