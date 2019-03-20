#include "vCPU.h"
#include "compiler.h"

int main () {
	std::string programName = "fib";
	compilation (programName, "cmdlist.txt");
	startCPU (programName);
}