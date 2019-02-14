//
// Created by Ярослав Окатьев  on 02/02/2019.
//

#include "akinator.h"

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

Node* readDB (FILE* file) {
	int i = 0;
	size_t size = fileSize (file);
	char* readBuf = new char[size] {};
	fread (readBuf, sizeof (char), size, file);
	Node* root = new Node;
	parsing (&i, size, readBuf, root);
	delete[] readBuf;
	fclose (file);
	return root;
}

void parsing (int* i, size_t size, char* readBuf, Node* cur) {
	char* tempBuf = new char[BUF_SIZE] {};
	int j = 0;
	for (; *i < size; *i += 1) {
		if (readBuf[*i] == '?') {
			while (readBuf[*i] != ':' && *i < size) {
				tempBuf[j] = readBuf[*i];
				*i += 1;
				j++;
			}
			cur->setValue (tempBuf);
			cur->setLeft (new Node);
			cur->setRight (new Node);
			parsing (i, size, readBuf, cur->getLeft ());
			parsing (i, size, readBuf, cur->getRight ());
			return;
		}
		else if (readBuf[*i] == '@') {
			*i += 1;
			while (readBuf[*i] != '}' && *i < size) {
				tempBuf[j] = readBuf[*i];
				*i += 1;
				j++;
			}
			cur->setValue (tempBuf);
			return;
		}
	}
}

void writeDB (FILE* file, Node* tree) {
	if (tree != nullptr) {
		if (tree->getValue () == nullptr || tree->getValue ()[0] == '\0')
			fprintf (file, "{@}");
		else if (tree->getValue ()[0] == '?') {
			fprintf (file, "{%s:", tree->getValue ());
			writeDB (file, tree->getLeft ());
			writeDB (file, tree->getRight ());
			fprintf (file, "}");
		} else {
			fprintf (file, "{@%s}", tree->getValue ());
			writeDB (file, tree->getLeft ());
			writeDB (file, tree->getRight ());
		}
	}
}

void play (Node* cur) {
	if (cur->getValue () == nullptr || cur->getValue ()[0] == '\0') {
		printf ("Unfortunately, I don't know the answer.\n");
		printf ("Would you like to help me? (y/n)\n");
		if (chooseVariant ('y', 'n') == 1)
			study (cur);
		return;
	} else if (cur->getValue ()[0] == '?') {
		printf ("Is it %s? (y/n)\n", cur->getValue () + 1);
		if (chooseVariant ('y', 'n') == 1) {
			play (cur->getLeft ());
		} else {
			play (cur->getRight ());
		}
		return;
	} else {
		printf ("My answer is %s\n", cur->getValue ());
		printf ("Am I right? (y/n)\n");
		if (chooseVariant ('y', 'n') == 2)
			changeAnswer (cur);
		return;
	}
}

void changeAnswer (Node* cur) {
	printf ("Please, type a property that differs your answer:\n");
	char* newQuestion = new char[BUF_SIZE] {'?'};
	strcat (newQuestion, readAnswer ());
	char* oldAnswer = new char[BUF_SIZE] {};
	strcpy (oldAnswer, cur->getValue ());
	delete[] cur->getValue ();
	cur->setValue (newQuestion);
	cur->setLeft (new Node);
	cur->setRight (new Node);
	printf ("Is your answer for 'yes' or 'no'? (y/n)\n");
	if (chooseVariant ('y', 'n') == 1) {
		printf ("Type your answer:\n");
		cur->getLeft ()->setValue (readAnswer ());
		cur->getRight ()->setValue (oldAnswer);
	} else {
		printf ("Type your answer:\n");
		cur->getRight ()->setValue (readAnswer ());
		cur->getLeft ()->setValue (oldAnswer);
	}
}


void study (Node* cur) {
	printf ("What do you want to add here?\n 1) characteristic\n 2) answer\n");
	if (chooseVariant ('1', '2') == 1) {
		printf ("Type characteristic in one-two word\n");
		delete[] cur->getValue ();
		cur->setValue (new char[BUF_SIZE] {'?'});
		char* answer = readAnswer ();
		strcat (cur->getValue (), answer);
		delete[] answer;
		printf ("You must add at least one answer\n");
		printf ("Will you add answer for 'yes' or for 'no'? (y/n)\n");
		int choice = chooseVariant ('y', 'n');
		cur->setLeft (new Node);
		cur->setRight (new Node);
		if (choice == 1) {
			study (cur->getLeft ());
		} else {
			study (cur->getRight ());
		}
		printf ("Will you add answer for '%s'? (y/n)\n", choice == 1 ? "no" : "yes");
		if (chooseVariant ('y', 'n') == 1) {
			if (choice == 1) {
				study (cur->getRight ());
			} else {
				study (cur->getLeft ());
			}
		}
	}
	else {
		printf ("Type answer in one-two word\n");
		cur->setValue (readAnswer ());
	}
}

int chooseVariant (char first, char second) {
	int ans = '\0';
	int flag = 0;
	while (true) {
		ans = getchar ();
		while (getchar () != '\n')
			flag++;
		if (ans == first && flag == 0) {
			return 1;
		} else if (ans == second && flag == 0) {
			return 2;
		} else {
			printf ("Please, type '%c' or '%c'\n", first, second);
			flag = 0;
			continue;
		}
	}
}

char* readAnswer () {
	int flag = 0;
	char* input = new char[BUF_SIZE] {};
	while (true) {
		fgets (input, BUF_SIZE, stdin);
		int len = strlen (input) - 1;
		while (!(isalpha (input[len]))) {
			input[len] = '\0';
			len--;
			if (len < 1) {
				flag = 1;
				break;
			}
		}
		for (int i = 0; i < len && flag != 1; i++) {
			if (!(isalpha (input[i]) && input[i] != ' '))
				flag = 1;
		}
		if (flag != 0) {
			printf ("Incorrect input, please, retype:\n");
			flag = 0;
			bzero (input, BUF_SIZE);
		} else {
			return input;
		}
	}
}

void removeTree (Node* tree) {
	if (tree != nullptr) {
		removeTree (tree->getLeft ());
		removeTree (tree->getRight ());
		delete tree;
	}
}
