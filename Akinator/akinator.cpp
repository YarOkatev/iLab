//
// Created by Ярослав Окатьев  on 02/02/2019.
//

//#include "akinator.h"

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
	int i = 1;
	size_t size = fileSize (file);
	char* readbuf = (char*)calloc (size, sizeof (char));
	fread (readbuf, sizeof (char), size, file);
	Node* root = new Node;
	parsing (&i, size, readbuf, root);
	free (readbuf);
	return root;
}

void parsing (int* i, size_t size, char* readbuf, Node* cur) {
	char* tempbuf = (char*)calloc (BUF_SIZE, sizeof (char));
	int j = 0;
	for (; *i < size; *i += 1) {
		if (readbuf[*i] == '?') {
			while (readbuf[*i] != ':' && *i < size) {
				tempbuf[j] = readbuf[*i];
				*i += 1;
				j++;
			}
			cur->val = tempbuf;
			cur->left = new Node;
			cur->right = new Node;
			parsing (i, size, readbuf, cur->left);
			parsing (i, size, readbuf, cur->right);
			return;
		}
		if (readbuf[*i] == '@') {
			*i += 1;
			while (readbuf[*i] != '}' && *i < size) {
				tempbuf[j] = readbuf[*i];
				*i += 1;
				j++;
			}
			cur->val = tempbuf;
			return;
		}
	}
}

void writeDB (FILE* file, Node* tree) {
	if (tree != NULL) {
		if (tree->val == NULL)
			fprintf (file, "{@}");
		else if (tree->val[0] == '?') {
			fprintf (file, "{%s:", tree->val);
			writeDB (file, tree->left);
			writeDB (file, tree->right);
			fprintf (file, "}");
		} else {
			fprintf (file, "{@%s}", tree->val);
			writeDB (file, tree->left);
			writeDB (file, tree->right);
		}
	}
}

void play (Node* cur) {
	if (cur->val[0] == '?') {
		printf ("Is it %s? (y/n)\n", cur->val + 1);
		if (chooseVariant ('y', 'n') == 1) {
			play (cur->left);
			return;
		} else {
			play (cur->right);
			return;
		}
	} else if (cur->val[0] != '\0') {
		printf ("My answer is %s\n", cur->val);
	} else {
		printf ("Unfortunately, I don't know the answer.\n");
		printf ("Would you like to help me? (y/n)\n");
		if (chooseVariant ('y', 'n') == 1)
			study (cur);
	}
}

void study (Node* cur) {
	printf ("What do you want to add here?\n 1) characteristic\n 2) answer\n");
	if (chooseVariant ('1', '2') == 1) {
		printf ("Type characteristic in one-two word\n");
		if (cur->val == NULL)
			cur->val = (char*)calloc (BUF_SIZE, sizeof (char));
		cur->val[0] = '?';
		char* answer = inputAnswer ();
		strcat (cur->val, answer);
		free (answer);
		printf ("You must add at least one answer\n");
		printf ("Will you add answer for 'yes' or for 'no'? (y/n)\n");
		int choice = chooseVariant ('y', 'n');
		cur->left = new Node;
		cur->right = new Node;
		if (choice == 1) {
			study (cur->left);
		} else {
			study (cur->right);
		}
		printf ("Will you add answer for '%s'? (y/n)\n", choice == 1 ? "no" : "yes");
		if (chooseVariant ('y', 'n') == 1) {
			if (choice == 1) {
				study (cur->right);
			} else {
				study (cur->left);
			}
		}
	} else {
		printf ("Type answer in one-two word\n");
		cur->val = inputAnswer ();
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

char* inputAnswer () {
	int flag = 0;
	char* input = (char*)calloc (BUF_SIZE, sizeof (char));
	while (true) {
		fgets (input, BUF_SIZE, stdin);
		int len = strlen (input) - 1;
		while (!(isalpha (input[len]))) {
			input[len] = '\0';
			len--;
		}
		for (int i = 0; i < len; i++) {
			if (!(isalpha (input[i]) || input[i] == ' '))
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
	if (tree != NULL) {
		removeTree (tree->left);
		removeTree (tree->right);
		delete tree;
	}
}
