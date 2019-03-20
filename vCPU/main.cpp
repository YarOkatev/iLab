#include "vCPU.h"
#include "compiler.h"

int main () {
	std::string exeName = "a.vcpu";
	compilation ("prog1", "cmdlist.txt", exeName);
	startCPU (exeName);
}