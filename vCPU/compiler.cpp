//
// Created by Ярослав Окатьев  on 2019-03-05.
//

#include "compiler.h"

size_t fileSize (FILE* file) {
	size_t size = 0;
	fseek (file, 0, SEEK_END);
	size = (size_t)ftell (file);
	if (size == 0) {
		printf ("File is empty\n");
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

struct userCommand* readUserProgram (FILE* programFile, int* programLen) {
	std::string& rawInput = *fileRead (programFile);

	std::string name_ = {};
	std::string firstArg_ = {};
	std::string secondArg_ = {};

	auto userProgram = new class userCommand [PROGRAM_SIZE] {};
	//auto userProgram = (class userCommand*) calloc (PROGRAM_SIZE, sizeof (class userCommand));
	int j = 0;
	for (int i = 0; i < rawInput.size(); i++, j++) {
		skipSpaces (rawInput, &i);
		if (rawInput[i] == ':') { //tags
			skipSpaces (rawInput, &i);
			name_ += ':';
			i++;
			assignString (&name_, rawInput, &i);
		} else {
			assignString (&name_, rawInput, &i); //functions
		}
		if (isspace (rawInput[i]) && rawInput[i] != '\n') {
			skipSpaces (rawInput, &i);
			assignString (&firstArg_, rawInput, &i);
		}
		if (rawInput[i] == ',') {
			i++;
			skipSpaces (rawInput, &i);
			assignString (&secondArg_, rawInput, &i);
		}
		if (rawInput[i] == ';') {
			while (rawInput[i] != '\n')
				i++;
		}

		if (name_.empty())
			break;

		userProgram[j].name = name_;
		userProgram[j].firstArg = firstArg_;
		userProgram[j].secondArg = secondArg_;

		name_.erase ();
		firstArg_.erase ();
		secondArg_.erase ();
	};

	*programLen = j;
	delete &rawInput;
	return userProgram;
}

//TODO realloc

void assignString (std::string* name_, std::string& rawInput, int* i) {
	while (isalnum (rawInput[*i])) {
		*name_ += rawInput[*i];
		*i += 1;
	}
}

void skipSpaces (std::string& str, int* i) {
	while (isspace (str[*i]))
		*i += 1;
}

class definedCommand* generateCommandList (FILE* config, int* cmdAmount) {
	std::string& rawConfig = *fileRead (config);

	auto cmdList = new class definedCommand [PROGRAM_SIZE] {};
	int code_ = 0;
	int j = 0;
	std::string name_ = {};

	for (int i = 0; i < rawConfig.size (); i++, j++) {
		skipSpaces (rawConfig, &i);
		while (isalpha (rawConfig[i])) { // read name
			name_ += rawConfig[i];
			i++;
		}
		code_ = readCode (rawConfig, &i); // read code
		if (rawConfig[i] == ':') { // if two codes
			setCommand (name_, code_, &(cmdList[j]));
			j++;
			code_ = readCode (rawConfig, &i);
			setCommand (name_, code_, &(cmdList[j]));
		} else {
			setCommand (name_, code_, &(cmdList[j]));
		}

		name_.erase ();
	}

	*cmdAmount = j;
	delete &rawConfig;
	return cmdList;
} //TODO realloc

int readCode (std::string& rawInput, int* i) {
	*i += 1;
	std::string codeString;
	while (isdigit (rawInput[*i])) {
		codeString += rawInput[*i];
		*i += 1;
	}
	int code = std::stoi (codeString);
	return code;
}

void setCommand (std::string name_, int code_, class definedCommand* command) {
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

void generateMachineCode (class definedCommand* cmdList, struct userCommand* userProgram, int cmdAmount, int programLen) {
	std::string* machineCodeStr = new std::string {};
	int j = 0;
	int errorNum = 0;

	for (int i = 0; i < programLen; i++) {
		j = 0;
		for (; j < cmdAmount; j++) {
			if (userProgram[i].name == cmdList[j].name || userProgram[i].name[0] == ':') {
				break;
			}
		}
		if (j == cmdAmount) {
			printf ("Command %s in line %d is not defined \n", userProgram[i].name.data (), i + 1);
			printf ("        ^\n");
			errorNum++;
		} else {
			if (userProgram[i].firstArg == "ax" || userProgram[i].firstArg == "bx" ||
				userProgram[i].firstArg == "cx" || userProgram[i].firstArg == "dx" ||
				isDigit (userProgram[i].firstArg)) {
				// do
			} else {

			}
		}

	}
}

bool isDigit (std::string str) {
	int i = 0;
	while (isdigit (str[i]))
		i++;
	if (i == str.size ())
		return true;
	return false;
}

