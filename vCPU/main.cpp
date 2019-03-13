
#include "compiler.h"

int main () {
	FILE* f = fopen ("prog1", "r");
	int a = 0, b = 0;
	FILE* g = fopen ("cmdlist.txt", "r");
	class DefinedCommand* cl = readCommandList (g, &b);
	class UserCommand* uc = readUserProgram (f, &a);
	compilationCode (cl, uc, b, a);
	fclose (f);
	fclose (g);
}