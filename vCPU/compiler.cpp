//
// Created by Ярослав Окатьев  on 2019-03-05.
//

#include "compiler.h"

Compiler::Compiler (FILE* programFile_, FILE* configFile_):
		programFile (programFile_),
		configFile (configFile_),
		programLen (0),
		cmdAmount (0),
		labelCount (0)
{};

void Compiler::fileRead (FILE* file) {
	if (!file) {
		std::cout << "File opening error";
		exit (1);
	}
	size_t size = fileSize (file);
	readBuffer.clear ();
	readBuffer.resize (size + 1);
	fread (readBuffer.data (), sizeof (char), size, file);
	fclose (file);
}

void Compiler::readUserProgram () {
	fileRead (programFile);

	std::string name_ {};
	std::string firstArg_ {};
	std::string secondArg_ {};

	//std::vector<class UserCommand> userProgram(PROGRAM_SIZE);

	userProgram = new UserCommand [PROGRAM_SIZE] {};

	for (int i = 0; i < readBuffer.size (); programLen++) {
		skipSpaces (&i);
		assignString (&name_, &i); //command
		if (isspace (readBuffer[i]) && readBuffer[i] != '\n') {
			skipSpaces (&i);
			assignString (&firstArg_, &i);
		}
		skipSpaces (&i);
		if (readBuffer[i] == ',') {
			i++;
			skipSpaces (&i);
			assignString (&secondArg_, &i);
		}
		skipSpaces (&i);
		if (readBuffer[i] == ';') {
			while (readBuffer[i] != '\n')
				i++;
		}

		if (name_.empty ())
			break;

//		if (j > userProgram.capacity ())
//			userProgram.reserve (userProgram.capacity () * 1.5);

		userProgram[programLen].name = name_;
		userProgram[programLen].firstArg = firstArg_;
		userProgram[programLen].secondArg = secondArg_;

		name_.clear ();
		firstArg_.clear ();
		secondArg_.clear ();
	};
} //TODO realloc

void Compiler::assignString (std::string* name_, int* i) {
	while (isalnum (readBuffer[*i]) || (ispunct (readBuffer[*i]) && readBuffer[*i] != ',' && readBuffer[*i] != ';')) {
		*name_ += readBuffer[*i];
		*i += 1;
	}
}

void Compiler::skipSpaces (int* i) {
	while (isspace (readBuffer[*i]))
		*i += 1;
}

void Compiler::readCommandList () {
	fileRead (configFile);

	cmdList = new DefinedCommand[PROGRAM_SIZE] {};
	int code_ = 0;
	std::string name_ {};

	for (int i = 0; i < readBuffer.size (); i++, cmdAmount++) {
		skipSpaces (&i);
		while (isalpha (readBuffer[i])) { // read name
			name_ += readBuffer[i];
			i++;
		}
		code_ = readCommandCode (&i); // read code
		if (readBuffer[i] == ':') { // if two codes
			setCommand (name_, code_);
			cmdAmount++;
			code_ = readCommandCode (&i);
			setCommand (name_, code_);
		} else {
			setCommand (name_, code_);
		}

		name_.clear ();
	}
} //TODO realloc

int Compiler::readCommandCode (int* i) {
	*i += 1;
	std::string codeString {};
	while (isdigit (readBuffer[*i])) {
		codeString += readBuffer[*i];
		*i += 1;
	}
	int code = std::stoi (codeString);
	return code;
}

void Compiler::setCommand (std::string name_, int code_) {
	cmdList[cmdAmount].name = name_;
	cmdList[cmdAmount].code = code_;
	if (code_ < 100) {
		cmdList[cmdAmount].argOne = -1; // -1 no arg, 0 arg is reg, 1 arg is num
		cmdList[cmdAmount].argTwo = -1;
	} else if (code_ > 100 && code_ < 150) {
		cmdList[cmdAmount].argOne = 0;
		cmdList[cmdAmount].argTwo = -1;
	} else if (code_ > 150 && code_ < 200) {
		cmdList[cmdAmount].argOne = 1;
		cmdList[cmdAmount].argTwo = -1;
	} else if (code_ > 200 && code_ < 250) {
		cmdList[cmdAmount].argOne = 0;
		cmdList[cmdAmount].argTwo = 0;
	} else {
		cmdList[cmdAmount].argOne = 1;
		cmdList[cmdAmount].argTwo = 0;
	}
}

void Compiler::generateMachineCode (std::string exeName) {
	std::string machineCode {};
	int cmdNum = 0;
	int errorCount = 0;

	labelList = new Label[PROGRAM_SIZE] {}; //TODO айайай

	for (int i = 0; i < programLen; i++) {
		if (userProgram[i].name[0] == ':') { // проверка на метки
			labelList[labelCount].name = userProgram[i].name;
			machineCode += userProgram[i].name + ' ';
			labelCount++;
			continue;
		}

		cmdNum = searchCommand (i);

		if (cmdNum < 0) {
			errorCount++;
		} else {
			machineCode += std::to_string (cmdList[cmdNum].code) + ' ';
			if (userProgram[i].name == "je" || userProgram[i].name == "jmp" || userProgram[i].name == "jne" ||
				userProgram[i].name == "ja" || userProgram[i].name == "jb") { // обработка jump отдельно
				machineCode += userProgram[i].firstArg + ' ';
				continue;
			}
			if (cmdList[cmdNum].argOne == 0) {
				if (setRegister (&machineCode, userProgram[i].firstArg, i))
					errorCount++;
			} else if (cmdList[cmdNum].argOne == 1) {
				machineCode += userProgram[i].firstArg + ' ';
			}
			if (cmdList[cmdNum].argTwo == 0) {
				if (setRegister (&machineCode, userProgram[i].secondArg, i))
					errorCount++;
			} else if (cmdList[cmdNum].argTwo == 1) {
				machineCode += userProgram[i].secondArg + ' ';
			}
		}
	}

	errorCount += labelAnalysis (machineCode);
	if (errorCount == 0) {
		std::cout << "Compilation successful\n";
		exeName += ".vcpu";
		FILE* exeFile = fopen (exeName.data (), "w");
		fwrite (machineCode.data (), sizeof(char), machineCode.size (), exeFile);
		fclose (exeFile);
		return;
	} else {
		std::cout << "Compilation unsuccessful. " << errorCount << " errors generated\n";
		exit (1);
	}
}

bool Compiler::setRegister (std::string* machineCodeStr, std::string argument, int line) {
	if (argument == "ax") {
		*machineCodeStr += "0 ";
	} else if (argument == "bx") {
		*machineCodeStr += "1 ";
	} else if (argument == "cx") {
		*machineCodeStr += "2 ";
	} else if (argument == "dx") {
		*machineCodeStr += "3 ";
	} else {
		std::string errorMsg = userProgram[line].name + ' ' + userProgram[line].firstArg;
		if (!userProgram[line].secondArg.empty ())
			errorMsg += ", " + userProgram[line].secondArg;
		std::cout << "Line " << line + 1 << ": no matching register for \"" << argument << "\"\n";
		std::cout << errorMsg << '\n';
		std::cout << "    ^^^^\n\n";
		return true;
	}
	return false;
}

int Compiler::searchCommand (int line) {
	int j = 0;
	int firstArg = 0, secondArg = 0;

	if (userProgram[line].firstArg.empty ()) {
		firstArg = -1;
	} else if (isDigit (userProgram[line].firstArg)) {
		firstArg = 1;
	} else {
		firstArg = 0;
	}
	if (userProgram[line].secondArg.empty ()) {
		secondArg = -1;
	} else if (isDigit (userProgram[line].secondArg)) {
		secondArg = 1;
	} else {
		secondArg = 0;
	}

	for (; j < cmdAmount; j++) { // поиск команды в списке определенных
		if (userProgram[line].name == cmdList[j].name && cmdList[j].argOne == firstArg && cmdList[j].argTwo == secondArg) {
			break;
		}
	}

	if (j == cmdAmount) { // проверка, что команда найдена
		std::string errorMsg = userProgram[line].name + ' ' + userProgram[line].firstArg;
		if (!userProgram[line].secondArg.empty ())
			errorMsg += ", " + userProgram[line].secondArg;
		std::cout << "Line " << line + 1 << ": no matching command for \n";
		std::cout << errorMsg << '\n';
		std::cout << "^^^^\n\n";
		return -1;
	}

	return j;
}

int Compiler::labelAnalysis (std::string &codeStr) {
	std::string currentLabel {};
	int k = 0, j = 0;
	int addrCount = 0;
	int errorCount = 0;
	for (int i = 0; i < codeStr.size (); i++) {
		if (codeStr[i] == ':') {
			currentLabel.clear ();
			k = i; j = 0;
			while (codeStr[k] != ' ') {
				currentLabel += codeStr[k];
				k++;
			}
			codeStr.erase (i, currentLabel.size () + 1);
			for (; j < labelCount; j++) {
				if (labelList[j].name == currentLabel) {
					if (labelList[j].address != 0) {
						std::cout << "Label \"" << labelList[j].name.data () + 1 << "\" declared twice\n\n";
						errorCount++;
					} else {
						labelList[j].address = addrCount;
					}
				}
			}
		}
		if (codeStr[i] == ' ')
			addrCount++;
	}

	for (int i = 0; i < codeStr.size (); i++) {
		if (isalpha (codeStr[i])) {
			currentLabel.clear ();
			currentLabel += ':';
			k = i; j = 0;
			while (codeStr[k] != ' ') {
				currentLabel += codeStr[k];
				k++;
			}
			codeStr.erase (i, currentLabel.size () - 1);
			for (; j < labelCount; j++) {
				if (labelList[j].name == currentLabel) {
					codeStr.insert (i, std::to_string (labelList[j].address));
					break;
				}
			}
			if (j == labelCount) {
				std::cout << "Label \"" << currentLabel << "\" does not exist\n\n";
				errorCount++;
			}
		}
	}

	return errorCount;
}

void compilation (std::string programName, std::string config) {
	FILE* programFile = fopen (programName.data (), "r");
	FILE* configFile = fopen (config.data (), "r");
	Compiler MyCompiler (programFile, configFile);
	MyCompiler.readUserProgram ();
	MyCompiler.readCommandList ();
	MyCompiler.generateMachineCode (programName);
	MyCompiler.memoryClean ();
}

bool isDigit (std::string str) {
	int i = 0;
	while (isdigit (str[i]) || str[i] == '-')
		i++;
	if (i == str.size ())
		return true;
	return false;
}

size_t fileSize (FILE* file) {
	size_t size = 0;
	fseek (file, 0, SEEK_END);
	size = (size_t)ftell (file);
	if (size == 0) {
		std::cout << "File is empty\n";
		exit (1);
	}
	fseek (file, 0, SEEK_SET);
	return size;
}

void Compiler::memoryClean () {
	readBuffer.clear ();
	readBuffer.shrink_to_fit ();
	delete[] userProgram;
	delete[] cmdList;
	delete[] labelList;
};
