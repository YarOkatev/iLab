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

struct userCommand* readUserProgram (FILE* programFile) {
	std::string& rawInput = *fileRead (programFile);
	struct userCommand* userProgram = (struct userCommand*) calloc (PROGRAM_SIZE, sizeof (struct userCommand));
	for (int i = 0, j = 0; i < rawInput.size(); i++) {
		std::string* name_ = new std::string {};
		std::string* firstArg_ = new std::string {};
		std::string* secondArg_ = new std::string {};

		skipSpaces (rawInput, &i);
		if (rawInput[i] == ':') { //tags
			skipSpaces (rawInput, &i);
			*name_ += ':';
			i++;
			assignString (name_, rawInput, &i);
		} else {
			assignString (name_, rawInput, &i); //functions
		}
		if (isspace (rawInput[i]) && rawInput[i] != '\n') {
			skipSpaces (rawInput, &i);
			assignString (firstArg_, rawInput, &i);
		}
		if (rawInput[i] == ',') {
			i++;
			skipSpaces (rawInput, &i);
			assignString (secondArg_, rawInput, &i);
		}
		if (rawInput[i] == ';') {
			while (rawInput[i] != '\n')
				i++;
		}

		if (name_->empty()) {
			delete name_;
			delete firstArg_;
			delete secondArg_;
			break;
		}
		userProgram[j].name = name_;
		if (firstArg_->empty ()) {
			delete firstArg_;
			userProgram[j].firstArg = nullptr;
		} else { userProgram[j].firstArg = firstArg_; }
		if (secondArg_->empty ()) {
			delete secondArg_;
			userProgram[j].secondArg = nullptr;
		} else { userProgram[j].secondArg = secondArg_; }

		j++;
	};
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

std::string* generateCommandList (FILE* config) {
	std::string& rawConfig = *fileRead (config);
	struct definedCommand* cmdList = (struct definedCommand*) calloc (PROGRAM_SIZE, sizeof (struct definedCommand));
	int code_ = 0;

	for (int i = 0, j = 0; i < rawConfig.size (); i++) {
		std::string* name_ = new std::string {};

		skipSpaces (rawConfig, &i);
		while (isalpha (rawConfig[i])) { // read name
			*name_ += rawConfig[i];
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
		j++;
	}
	delete &rawConfig;
	return nullptr;
}

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

void setCommand (std::string* name_, int code_, struct definedCommand* command) {
	command->name = name_;
	command->code = code_;
	if (code_ < 100) {
		command->argOne = -1;
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

