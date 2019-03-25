#include "vCPU.h"
#include "compiler.h"

int main () {
	std::string programName = "sqreq";
	compilation (programName, "cmdlist.txt");
	startCPU (programName);
}