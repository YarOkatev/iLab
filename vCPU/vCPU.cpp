//
// Created by Ярослав Окатьев  on 2019-03-17.
//

#include "vCPU.h"

CpuMemory::CpuMemory ():
	regArray (new int[REG_NUM] {}),
	memArray (new int[MEM_SIZE] {}),
	currentPtr (0)
	{};

void CpuMemory::flashMem (FILE* bootFile) {
	int i = 0;
	while (i < MEM_SIZE && fscanf (bootFile, "%d", &memArray[i]) != EOF)
		i++;
	if (fscanf (bootFile, "%d", memArray) != EOF && i == MEM_SIZE) {
		std::cout << "Program is too large. It should include less than " << MEM_SIZE << " codes\n";
		exit (1);
	}
	fclose (bootFile);
}

int CpuMemory::getWord () {
	currentPtr++;
	return memArray[currentPtr - 1];
}

CpuMemory::~CpuMemory () {
	delete[] regArray;
	delete[] memArray;
}

void Processor::halt () {
	std::cout << "\nvCPU stopped\n";
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

void Processor::movN () {
	int regNumber =  memory.getWord ();
	memory.regArray[regNumber] = memory.getWord ();
}

void Processor::tact () {
	int code = 0;
	while (true) {
		code = memory.getWord ();
		memory.regArray[SP] = stack.size () - 1;
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
			case 57: {
				stack.ssqrt ();
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
			case 112: {
				retR ();
				break;
			}
			case 151: {
				npush ();
				break;
			}
			case 152: {
				retN ();
				break;
			}
			case 201: {
				rcmp ();
				break;
			}
			case 252: {
				movN ();
				break;
			}
			case 301: {
				movSN ();
				break;
			}
			case 302: {
				movSR ();
				break;
			}
			case 303: {
				call ();
				break;
			}
			default: {
				std::cout << "CPU looped :)\n";
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
	std::cout << "Current pointer: " << memory.currentPtr << " (" << memory.memArray[memory.currentPtr] << ")\n";
	std::cout << "AX: " << memory.regArray[AX] << "\n";
	std::cout << "BX: " << memory.regArray[BX] << "\n";
	std::cout << "CX: " << memory.regArray[CX] << "\n";
	std::cout << "DX: " << memory.regArray[DX] << "\n";
	std::cout << "LX: " << memory.regArray[LX] << "\n";
	std::cout << "SP: " << memory.regArray[SP] << "\n";
	std::cout << "FP: " << memory.regArray[FP] << "\n";
	std::cout << "RX: " << memory.regArray[RX] << "\n";
	std::cout << "------------------------------\n";
}

void Processor::movSN () {
	int* currReg = &memory.regArray[memory.getWord ()];
	if (memory.getWord () == 1)
		*currReg = stack.data ()[memory.regArray[memory.getWord ()] + memory.getWord ()];
	else
		*currReg = stack.data ()[memory.regArray[memory.getWord ()] - memory.getWord ()];
}

void Processor::movSR () {
	int* currReg = &memory.regArray[memory.getWord ()];
	if (memory.getWord () == 1)
		*currReg = stack.data ()[memory.regArray[memory.getWord ()] + memory.regArray[memory.getWord ()]];
	else
		*currReg = stack.data ()[memory.regArray[memory.getWord ()] - memory.regArray[memory.getWord ()]];
}

void Processor::retR () {
	memory.regArray[RX] = memory.regArray[memory.getWord ()];
	unstacking ();
}

void Processor::retN () {
	memory.regArray[RX] = memory.getWord ();
	unstacking ();
}

void Processor::call () {
	int argNum = memory.getWord ();
	stacking (argNum);
	for (; argNum > 0; argNum--) {
		if (memory.getWord () == 1)
			stack.push (memory.getWord ());
		else
			stack.push (memory.regArray[memory.getWord ()]);
	}
	memory.currentPtr = memory.getWord ();
}

void Processor::stacking (int argNum) {
	stack.push (memory.regArray[AX]);
	stack.push (memory.regArray[BX]);
	stack.push (memory.regArray[CX]);
	stack.push (memory.regArray[DX]);
	stack.push (memory.regArray[SP]);
	stack.push (memory.regArray[FP]);
	stack.push (memory.currentPtr + argNum * 2 + 1);
	memory.regArray[FP] = stack.size ();
}

void Processor::unstacking () {
	while (stack.size () != memory.regArray[FP]) { //cleaning stack
		stack.pop ();
	}
	memory.currentPtr = stack.pop ();
	memory.regArray[FP] = stack.pop ();
	memory.regArray[SP] = stack.pop ();
	memory.regArray[DX] = stack.pop ();
	memory.regArray[CX] = stack.pop ();
	memory.regArray[BX] = stack.pop ();
	memory.regArray[AX] = stack.pop ();
}

void startCPU (std::string fileName) {
	fileName += ".vcpu";
	FILE* bootFile = fopen (fileName.data (), "r");
	if (!bootFile) {
		std::cout << "File opening error";
		exit (1);
	}
	Processor myCpu (bootFile);
	myCpu.tact ();
}