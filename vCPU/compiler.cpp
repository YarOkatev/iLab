//
// Created by Ярослав Окатьев  on 2019-03-05.
//

#include "compiler.h"

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

std::string* fileRead (FILE* file) {
	int i = 0;
	size_t size = fileSize (file);
	std::string* readBuf = new std::string {};
	readBuf->resize (size + 1);
	fread (readBuf->data(), sizeof (char), size, file);
	fclose (file);
	return readBuf;
}

struct UserCommand* readUserProgram (FILE* programFile, int* programLen) {
	std::string& rawInput = *fileRead (programFile);

	std::string name_ = {};
	std::string firstArg_ = {};
	std::string secondArg_ = {};

	auto userProgram = new class UserCommand [PROGRAM_SIZE] {};
	//auto userProgram = (class UserCommand*) calloc (PROGRAM_SIZE, sizeof (class UserCommand));
	int j = 0;
	for (int i = 0; i < rawInput.size(); j++) {
		skipSpaces (rawInput, &i);
		assignString (&name_, rawInput, &i); //command
		if (isspace (rawInput[i]) && rawInput[i] != '\n') {
			skipSpaces (rawInput, &i);
			assignString (&firstArg_, rawInput, &i);
		}
		skipSpaces (rawInput, &i);
		if (rawInput[i] == ',') {
			i++;
			skipSpaces (rawInput, &i);
			assignString (&secondArg_, rawInput, &i);
		}
		skipSpaces (rawInput, &i);
		if (rawInput[i] == ';') {
			while (rawInput[i] != '\n')
				i++;
		}

		if (name_.empty())
			break;

		userProgram[j].name = name_;
		userProgram[j].firstArg = firstArg_;
		userProgram[j].secondArg = secondArg_;

		name_.clear ();
		firstArg_.clear ();
		secondArg_.clear ();
	};

	*programLen = j;
	delete &rawInput;
	return userProgram;
}

//TODO realloc

void assignString (std::string* name_, std::string& rawInput, int* i) {
	while (isalnum (rawInput[*i]) || (ispunct (rawInput[*i]) &&
			rawInput[*i] != ',' && rawInput[*i] != ';')) {
		*name_ += rawInput[*i];
		*i += 1;
	}
}

void skipSpaces (std::string& str, int* i) {
	while (isspace (str[*i]))
		*i += 1;
}

class DefinedCommand* readCommandList (FILE* config, int* cmdAmount) {
	std::string& rawConfig = *fileRead (config);

	auto cmdList = new class DefinedCommand [PROGRAM_SIZE] {};
	int code_ = 0;
	int j = 0;
	std::string name_ = {};

	for (int i = 0; i < rawConfig.size (); i++, j++) {
		skipSpaces (rawConfig, &i);
		while (isalpha (rawConfig[i])) { // read name
			name_ += rawConfig[i];
			i++;
		}
		code_ = readCommandCode (rawConfig, &i); // read code
		if (rawConfig[i] == ':') { // if two codes
			setCommand (name_, code_, &(cmdList[j]));
			j++;
			code_ = readCommandCode (rawConfig, &i);
			setCommand (name_, code_, &(cmdList[j]));
		} else {
			setCommand (name_, code_, &(cmdList[j]));
		}

		name_.clear ();
	}

	*cmdAmount = j;
	delete &rawConfig;
	return cmdList;
} //TODO realloc

int readCommandCode (std::string &rawInput, int* i) {
	*i += 1;
	std::string codeString;
	while (isdigit (rawInput[*i])) {
		codeString += rawInput[*i];
		*i += 1;
	}
	int code = std::stoi (codeString);
	return code;
}

void setCommand (std::string name_, int code_, class DefinedCommand* command) {
	command->name = name_;
	command->code = code_;
	if (code_ < 100) {
		command->argOne = -1; // -1 no arg, 0 arg is reg, 1 arg is num
		command->argTwo = -1;
	} else if (code_ > 100 && code_ < 150) {
		command->argOne = 0;
		command->argTwo = -1;
	} else if (code_ > 150 && code_ < 200) {
		command->argOne = 1;
		command->argTwo = -1;
	} else if (code_ > 200 && code_ < 250) {
		command->argOne = 0;
		command->argTwo = 0;
	} else {
		command->argOne = 1;
		command->argTwo = 0;
	}
}

void compilationCode (class DefinedCommand* cmdList, struct UserCommand* userProgram, int cmdAmount, int programLen) {
	auto machineCode = new std::string {};
	auto labelList = new class Label [PROGRAM_SIZE] {}; //TODO айайай
	int cmdNum = 0;
	int errorCount = 0;
	int labelCount = 0;

	for (int i = 0; i < programLen; i++) {
		if (userProgram[i].name[0] == ':') { // проверка на метки
			labelList[labelCount].name = userProgram[i].name;
			*machineCode += userProgram[i].name + ' ';
			labelCount++;
			continue;
		}

		cmdNum = searchCommand (userProgram[i], programLen, cmdList, cmdAmount, i);

		if (cmdNum < 0) {
			errorCount++;
		} else {
			*machineCode += std::to_string (cmdList[cmdNum].code) + ' ';
			if (userProgram[i].name == "je" || userProgram[i].name == "jmp" || userProgram[i].name == "jne" ||
				userProgram[i].name == "ja" || userProgram[i].name == "jb") { // обработка jump отдельно
				*machineCode += userProgram[i].firstArg + ' ';
				continue;
			}
			if (cmdList[cmdNum].argOne == 0) {
				if (setRegister (machineCode, userProgram[i].firstArg, userProgram[i], i))
					errorCount++;
			} else if (cmdList[cmdNum].argOne == 1) {
				*machineCode += userProgram[i].firstArg + ' ';
			}
			if (cmdList[cmdNum].argTwo == 0) {
				if (setRegister (machineCode, userProgram[i].secondArg, userProgram[i], i))
					errorCount++;
			} else if (cmdList[cmdNum].argTwo == 1) {
				*machineCode += userProgram[i].secondArg + ' ';
			}
		}
	}

	errorCount += labelAnalysis (*machineCode, labelList, labelCount);
	if (errorCount == 0) {
		std::cout << "Compilation successful\n";
	} else {
		std::cout << "Compilation unsuccessful. " << errorCount << " errors generated\n";
	}

	std::cout << "\nCODE: " << *machineCode << '\n';
}

bool setRegister (std::string* machineCodeStr, std::string argument, class UserCommand userProgram, int line) {
	if (argument == "ax") {
		*machineCodeStr += "1 ";
	} else if (argument == "bx") {
		*machineCodeStr += "2 ";
	} else if (argument == "cx") {
		*machineCodeStr += "3 ";
	} else if (argument == "dx") {
		*machineCodeStr += "4 ";
	} else {
		std::string errorMsg = userProgram.name + ' ' + userProgram.firstArg;
		if (!userProgram.secondArg.empty ())
			errorMsg += ", " + userProgram.secondArg;
		std::cout << "Line " << line + 1 << ": no matching register for \"" << argument << "\"\n";
		std::cout << errorMsg << '\n';
		std::cout << "    ^^^^\n\n";
		return true;
	}
	return false;
}

bool isDigit (std::string str) {
	int i = 0;
	while (isdigit (str[i]))
		i++;
	if (i == str.size ())
		return true;
	return false;
}

int searchCommand (class UserCommand userProgram, int programLen,
					class DefinedCommand* cmdList, int cmdAmount, int line) {
	int j = 0;
	int firstArg = 0, secondArg = 0;

	if (userProgram.firstArg.empty ()) {
		firstArg = -1;
	} else if (isDigit (userProgram.firstArg)) {
		firstArg = 1;
	} else {
		firstArg = 0;
	}
	if (userProgram.secondArg.empty ()) {
		secondArg = -1;
	} else if (isDigit (userProgram.secondArg)) {
		secondArg = 1;
	} else {
		secondArg = 0;
	}

	for (; j < cmdAmount; j++) { // поиск команды в списке определенных
		if (userProgram.name == cmdList[j].name && cmdList[j].argOne == firstArg && cmdList[j].argTwo == secondArg) {
			break;
		}
	}

	if (j == cmdAmount) { // проверка, что команда найдена
		std::string errorMsg = userProgram.name + ' ' + userProgram.firstArg;
		if (!userProgram.secondArg.empty ())
			errorMsg += ", " + userProgram.secondArg;
		std::cout << "Line " << line + 1 << ": no matching command for \n";
		std::cout << errorMsg << '\n';
		std::cout << "^^^^\n\n";
		return -1;
	}

	return j;
}

int labelAnalysis (std::string &codeStr, class Label* labelList, int labelCount) {
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
			codeStr.erase (i, currentLabel.size() + 1);
			for ( ; j < labelCount; j++) {
				if (labelList[j].name == currentLabel)
					if (labelList[j].address != 0) {
						std::cout << "Label \"" << labelList[j].name.data () + 1 << "\" declared twice\n\n";
						errorCount++;
					} else {
						labelList[j].address = addrCount;
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
			codeStr.erase (i, currentLabel.size() - 1);
			for ( ; j < labelCount; j++) {
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