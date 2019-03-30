#include "vCPU.h"
#include "compiler.h"

int main () {
	std::string programName = "sqreq"; //source code file
	compilation (programName, "cmdlist.txt"); //command config file
	startCPU (programName);
}