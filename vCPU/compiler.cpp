//
// Created by Ярослав Окатьев  on 2019-03-05.
//

#include "compiler.h"

Compiler::Compiler (FILE* programFile_, FILE* configFile_):
		readBuffer (*(new std::string {})),
		userProgram (*(new std::vector<UserCommand> (DEFAULT_SIZE))),
		cmdList (*(new std::vector<DefinedCommand> (DEFAULT_SIZE))),
		labelList (*(new std::vector<Label> (DEFAULT_SIZE / 2))),
		machineCode (*(new std::string {})),
		programFile (programFile_),
		configFile (configFile_),
		programLen (0),
		cmdAmount (0),
		labelCount (0),
		errorCount (0)
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

	for (int i = 0; i < readBuffer.size (); i++) {
		for (int j = 0; ; j++) {
			skipSpaces (&i);
			if (userProgram[programLen].arg.capacity () <= j)
				userProgram[programLen].arg.resize (userProgram[programLen].arg.capacity () + 3);
			assignString (&(userProgram[programLen].arg[j]), &i);
			if (readBuffer[i] == '\n' || i + 1 >= readBuffer.size ())
				break;
		}
		programLen++;
		if (programLen + 1 > userProgram.capacity ()) //resize vector
			userProgram.resize (userProgram.capacity () * 2);
	}
}

void Compiler::assignString (std::string* name_, int* i) {
	while ((isalnum (readBuffer[*i]) || readBuffer[*i] == '-' || readBuffer[*i] == '+' || readBuffer[*i] == ':') \
			&& *i <= readBuffer.size ()) {
		*name_ += readBuffer[*i];
		*i += 1;
	}
}

void Compiler::skipSpaces (int* i) {
	while (!(isalnum (readBuffer[*i]) || readBuffer[*i] == '+' || readBuffer[*i] == '-' || readBuffer[*i] == ':') \
			&& *i <= readBuffer.size () && readBuffer[*i] != '\n')
		if (readBuffer[*i] == ';') { //skip comments
			while (readBuffer[*i] != '\n') {
				*i += 1;
			}
		} else {
			*i += 1;
		}
}

void Compiler::readCommandList () {
	fileRead (configFile);

	int code_ = 0;
	std::string name_ {};

	for (int i = 0; i < readBuffer.size (); i++) {
		while (isspace (readBuffer[i]))
			i++;
		while (isalpha (readBuffer[i])) { // read name
			name_ += readBuffer[i];
			i++;
		}

		do {
			code_ = readCommandCode (&i);
			setCommand (name_, code_);
			cmdAmount++;
		} while (readBuffer[i] == ':');

		if (cmdAmount + 1 > cmdList.capacity ()) //resize vector
			cmdList.resize (cmdList.capacity () * 2);

		name_.clear ();
	}
}

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
	int cmdNum = 0;

	for (int i = 0; i < programLen; i++) {
		if (userProgram[i].arg[0].empty ())
			continue;

		if (userProgram[i].arg[0][0] == ':') { //labels check
			if (labelCount + 1 > labelList.capacity ())
				labelList.resize (labelList.capacity () * 2);
			labelList[labelCount].name = userProgram[i].arg[0];
			machineCode += userProgram[i].arg[0] + ' ';
			labelCount++;
			continue;
		}
		if (userProgram[i].arg[0] == "call") {
			callProcessing (i);
			continue;
		} else if (userProgram[i].arg[0] == "mov") {
			movProcessing (i);
			continue;
		} else {
			cmdNum = searchCommand (i); //search command through the defined commands
		}

		if (cmdNum < 0) {
			errorCount++;
		} else {
			machineCode += std::to_string (cmdList[cmdNum].code) + ' ';
			if (userProgram[i].arg[0] == "je" || userProgram[i].arg[0] == "jmp" || userProgram[i].arg[0] == "jne" ||
				userProgram[i].arg[0] == "ja" || userProgram[i].arg[0] == "jb") { //jump processing
				if (!isDigit (userProgram[i].arg[1]))
					machineCode += userProgram[i].arg[1] + ' ';
				continue;
			}
			if (cmdList[cmdNum].argOne == 0) {
				setRegister (userProgram[i].arg[1], i);
			} else if (cmdList[cmdNum].argOne == 1) {
				machineCode += userProgram[i].arg[1] + ' ';
			}
			if (cmdList[cmdNum].argTwo == 0) {
				setRegister (userProgram[i].arg[2], i);
			} else if (cmdList[cmdNum].argTwo == 1) {
				machineCode += userProgram[i].arg[2] + ' ';
			}
		}
	}

	labelAnalysis (); //label processing

	compilationResult (exeName);
}

void Compiler::compilationResult (std::string &exeName) {
	if (errorCount == 0) {
		std::cout << "Compilation successful\n";
		exeName += ".vcpu";
		FILE* exeFile = fopen (exeName.data (), "w");
		fwrite (machineCode.data (), sizeof (char), machineCode.size (), exeFile);
		fclose (exeFile);
		return;
	} else {
		std::cout << "Compilation unsuccessful. " << errorCount << " errors generated\n";
		exit (1);
	}
}

void Compiler::setRegister (std::string argument, int line) {
	if (argument == "ax") {
		machineCode += "0 ";
	} else if (argument == "bx") {
		machineCode+= "1 ";
	} else if (argument == "cx") {
		machineCode += "2 ";
	} else if (argument == "dx") {
		machineCode += "3 ";
	} else if (argument == "lx") {
		machineCode += "4 ";
	} else if (argument == "sp") {
		machineCode += "5 ";
	} else if (argument == "fp") {
		machineCode += "6 ";
	} else if (argument == "rx") {
		machineCode += "7 ";
	} else {
		std::string errorMsg = userProgram[line].arg[0] + ' ' + userProgram[line].arg[1];
		if (!userProgram[line].arg[2].empty ())
			errorMsg += ", " + userProgram[line].arg[2];
		std::cout << "Line " << line + 1 << ": no matching register for \"" << argument << "\"\n";
		std::cout << errorMsg << '\n';
		std::cout << "    ^^^^\n\n";
		errorCount++;
	}
}

int Compiler::searchCommand (int line) {
	int j = 0;
	int firstArg = 0, secondArg = 0;

	if (userProgram[line].arg[1].empty ()) {
		firstArg = -1;
	} else if (isDigit (userProgram[line].arg[1])) {
		firstArg = 1;
	} else {
		firstArg = 0;
	}
	if (userProgram[line].arg[2].empty ()) {
		secondArg = -1;
	} else if (isDigit (userProgram[line].arg[2])) {
		secondArg = 1;
	} else {
		secondArg = 0;
	}

	for (; j < cmdAmount; j++) { //search command through the defined commands
		if (userProgram[line].arg[0] == cmdList[j].name && cmdList[j].argOne == firstArg && cmdList[j].argTwo == secondArg) {
			break;
		}
	}

	if (j == cmdAmount) { //is command found?
		std::string errorMsg = userProgram[line].arg[0] + ' ' + userProgram[line].arg[1];
		if (!userProgram[line].arg[2].empty ())
			errorMsg += ", " + userProgram[line].arg[2];
		std::cout << "Line " << line + 1 << ": no matching command for \n";
		std::cout << errorMsg << '\n';
		std::cout << "^^^^\n\n";
		return -1;
	}

	return j;
}

void Compiler::labelAnalysis () {
	std::string currentLabel {};
	int k = 0, j = 0;
	int addrCount = 0;
	for (int i = 0; i < machineCode.size (); i++) {
		if (machineCode[i] == ':') {
			currentLabel.clear ();
			k = i; j = 0;
			while (machineCode[k] != ' ') {
				currentLabel += machineCode[k];
				k++;
			}
			machineCode.erase (i, currentLabel.size () + 1);
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
		if (machineCode[i] == ' ')
			addrCount++;
	}

	for (int i = 0; i < machineCode.size (); i++) {
		if (isalpha (machineCode[i])) {
			currentLabel.clear ();
			currentLabel += ':';
			k = i; j = 0;
			while (machineCode[k] != ' ') {
				currentLabel += machineCode[k];
				k++;
			}
			machineCode.erase (i, currentLabel.size () - 1);
			for (; j < labelCount; j++) {
				if (labelList[j].name == currentLabel) {
					machineCode.insert (i, std::to_string (labelList[j].address));
					break;
				}
			}
			if (j == labelCount) {
				std::cout << "Label \"" << currentLabel << "\" does not exist\n\n";
				errorCount++;
			}
		}
	}
}

void compilation (std::string programName, std::string config) {
	FILE* programFile = fopen (programName.data (), "r");
	FILE* configFile = fopen (config.data (), "r");
	Compiler MyCompiler (programFile, configFile);
	MyCompiler.readUserProgram ();
	MyCompiler.readCommandList ();
	MyCompiler.generateMachineCode (programName);
}

bool isDigit (std::string str) {
	int i = 0;
	while (isdigit (str[i]) || str[i] == '-' || str[i] == '+')
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

Compiler::~Compiler () {
	delete &readBuffer;
	delete &labelList;
	delete &userProgram;
	delete &cmdList;
	delete &machineCode;
}

void Compiler::callProcessing (int line) { // [CMD] [ARG_NUM] [ARG_TYPE 1/0 - number/reg] [ARG] ... [ADDRESS]
	for (int j = 0; j < cmdAmount; j++) { //search command through the defined commands
		if (userProgram[line].arg[0] == cmdList[j].name) {
			machineCode += std::to_string (cmdList[j].code) + " ";
			break;
		}
	}
	int strPos = machineCode.size ();
	int k = 2;
	while (!userProgram[line].arg[k].empty ()) {
		if (isDigit (userProgram[line].arg[k])) {
			machineCode += "1 ";
			machineCode += userProgram[line].arg[k] + ' ';
		} else {
			machineCode += "0 ";
			setRegister (userProgram[line].arg[k], line);
		}
		k++;
	}
	if (!isDigit (userProgram[line].arg[1]))
		machineCode += userProgram[line].arg[1] + ' ';
	machineCode.insert (strPos, std::to_string (k - 2) + " ");
}

void Compiler::movProcessing (int line) {
	int i = 1;
	int errorBefore = errorCount;
	int mask[5] {};
	while (!userProgram[line].arg[i].empty ()) {
		mask[i - 1] = isDigit (userProgram[line].arg[i]);
		i++;
	}
	i--;
	if (i == 2 && mask[0] == 0 && mask[1] == 1) { // [CMD] [DESTINATION] [NUMBER]
		machineCode += "252 ";
		setRegister (userProgram[line].arg[1], line);
		machineCode += userProgram[line].arg[2] + " ";
	} else if (i == 4 && mask[0] == 0 && mask[1] == 0 && mask[3] == 1 \
				&& (userProgram[line].arg[3] == "+" || userProgram[line].arg[3] == "-")) {
		machineCode += "301 ";							// [CMD] [DESTINATION] [ADD/SUB 1/0] [FIRST] [SECOND]
		setRegister (userProgram[line].arg[1], line);
		if (userProgram[line].arg[3] == "+")
			machineCode += "1 ";
		else if (userProgram[line].arg[3] == "-")
			machineCode += "0 ";
		else
			errorCount++;
		setRegister (userProgram[line].arg[2], line);
		machineCode += userProgram[line].arg[4] + " ";
	} else if (i == 4 && mask[0] == 0 && mask[1] == 0 && mask[3] == 0 \
				&& (userProgram[line].arg[3] == "+" || userProgram[line].arg[3] == "-")) {
		machineCode += "302 ";
		setRegister (userProgram[line].arg[1], line);
		if (userProgram[line].arg[3] == "+")
			machineCode += "1 ";
		else if (userProgram[line].arg[3] == "-")
			machineCode += "0 ";
		else
			errorCount++;
		setRegister (userProgram[line].arg[2], line);
		setRegister (userProgram[line].arg[4], line);
	} else {
		errorCount++;
	}
	if (errorBefore < errorCount) {
		std::string errorMsg = userProgram[line].arg[0] + ' ' + userProgram[line].arg[1];
		if (!userProgram[line].arg[2].empty ())
			errorMsg += ", " + userProgram[line].arg[2];
		std::cout << "Line " << line + 1 << ": no matching command for \n";
		std::cout << errorMsg << '\n';
		std::cout << "^^^^\n\n";
	}
};
