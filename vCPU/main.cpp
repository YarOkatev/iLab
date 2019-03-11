
#include "compiler.h"

int main () {
	std::string as = "asd";
	as.clear ();
	std::cout << as;
	FILE* f = fopen ("prog1", "r");
	int a = 0, b = 0;
	FILE* g = fopen ("cmdlist.txt", "r");
	class DefinedCommand* cl = readCommandList (g, &b);
	class UserCommand* uc = readUserProgram (f, &a);
	precompilationCode (cl, uc, b, a);
}