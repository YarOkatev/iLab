//
// Created by Ярослав Окатьев  on 2019-03-17.
//

#include "vCPU.h"

CpuMemory::CpuMemory ():
	regArray (new int[5] {}),
	memArray (new int[MEM_SIZE] {}),
	currentPtr (0)
	{};

void CpuMemory::flashMem (FILE* bootFile) {
	int i = 0;
	while (i < MEM_SIZE && fscanf (bootFile, "%d", &memArray[i]) != EOF)
		i++;
	if (fscanf (bootFile, "%d") != EOF && i == MEM_SIZE) {
		std::cout << "Program is too large\n It should include less than " << MEM_SIZE << " codes\n";
	}
	fclose (bootFile);
}

int CpuMemory::getWord () {
	currentPtr++;
	return memArray[currentPtr - 1];
}

void Processor::halt () {
	std::cout << "vCPU stopped\n";
	exit (0);
}

Processor::Processor (FILE* bootFile):
	stack (STACK_SIZE)
{
	memory.flashMem (bootFile);
	std::cout << "vCPU started\n";
}

void Processor::print () {
	std::cout << memory.regArray[memory.getWord ()] << "\n";
}

void Processor::loop () {
	while (true)
		;
}

void Processor::pop () {
	stack.pop ();
}

void Processor::scmp () {
	memory.regArray[LX] = stack.cmp ();
}

void Processor::top () {
	memory.regArray[memory.getWord ()] = stack.top ();
}

void Processor::rpush () {
	stack.push (memory.regArray[memory.getWord ()]);
}

void Processor::rpop () {
	memory.regArray[memory.getWord ()] = stack.pop ();
}

void Processor::jmp () {
	memory.currentPtr = memory.getWord ();
}

void Processor::je () {
	if (memory.regArray[LX] == 0) {
		memory.currentPtr = memory.getWord ();
		return;
	}
	memory.currentPtr++;
}

void Processor::jne () {
	if (memory.regArray[LX] != 0) {
		memory.currentPtr = memory.getWord ();
		return;
	}
	memory.currentPtr++;
}

void Processor::ja () {
	if (memory.regArray[LX] == 1) {
		memory.currentPtr = memory.getWord ();
		return;
	}
	memory.currentPtr++;
}

void Processor::jb () {
	if (memory.regArray[LX] == 2) {
		memory.currentPtr = memory.getWord ();
		return;
	}
	memory.currentPtr++;
}

void Processor::npush () {
	stack.push (memory.getWord ());
}

void Processor::rcmp () {
	int first = memory.regArray[memory.getWord ()];
	int second = memory.regArray[memory.getWord ()];
	if (first == second)
		memory.regArray[LX] = 0;
	else if (first > second)
		memory.regArray[LX] = 1;
	else
		memory.regArray[LX] = 2;
}

void Processor::reg () {
	int num = memory.getWord ();
	memory.regArray[memory.getWord ()] = num;
}

void Processor::tact () {
	int code = 0;
	int a;
	while (true) {
		a = memory.memArray[memory.currentPtr];
		code = memory.getWord ();
		switch (code) {
			case 0: {
				halt ();
				break;
			}
			case 1: {
				dump ();
				break;
			}
			case 51: {
				pop ();
				break;
			}
			case 52: {
				scmp ();
				break;
			}
			case 53: {
				stack.add ();
				break;
			}
			case 54: {
				stack.sub ();
				break;
			}
			case 55: {
				stack.div ();
				break;
			}
			case 56: {
				stack.mul ();
				break;
			}
			case 101: {
				top ();
				break;
			}
			case 103: {
				rpush ();
				break;
			}
			case 104: {
				rpop ();
				break;
			}
			case 105: {
				jmp ();
				break;
			}
			case 106: {
				je ();
				break;
			}
			case 107: {
				jne ();
				break;
			}
			case 108: {
				ja ();
				break;
			}
			case 109: {
				jb ();
				break;
			}
			case 110: {
				print ();
				break;
			}
			case 111: {
				write ();
				break;
			}
			case 151: {
				npush ();
				break;
			}
			case 201: {
				rcmp ();
				break;
			}
			case 252: {
				reg ();
				break;
			}
			default: {
				std::cout << "afaf";
				loop ();
			}
		}
	}
}

void Processor::write () {
	std::cin >> memory.regArray[memory.getWord ()];
}

void Processor::dump () {
	std::cout << "\n------------ DUMP ------------\n";
	stack.dump ();
	std::cout << "Current pointer: " << memory.currentPtr << "\n";
	std::cout << "AX: " << memory.regArray[AX] << "\n";
	std::cout << "BX: " << memory.regArray[BX] << "\n";
	std::cout << "CX: " << memory.regArray[CX] << "\n";
	std::cout << "DX: " << memory.regArray[DX] << "\n";
	std::cout << "LX: " << memory.regArray[LX] << "\n";
	std::cout << "------------------------------\n\n";
}

void startCPU (std::string fileName) {
	FILE* bootFile = fopen (fileName.data (), "r");
	Processor myCpu (bootFile);
	myCpu.tact ();
}