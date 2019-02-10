#include "test.h"

int main () {
	struct Method* var = makeFuncArray ();

	FILE* database = NULL;
	List** table = NULL;
	char file[25];
	double disp = 0;
	FILE* stat = NULL;

	//system ("./phonegen.exe");

	for (int i = 0; i < 4; i++) {
		table = tableInit ();
		disp = 0;
		database = fopen ("contacts.dat", "r");
		fileRead (database, table, var[i].hfunc);
		disp = dispersion (table);
		strcat (file, var[i].name);
		strcat (file, ".csv");
		stat = fopen (file, "w");
		for (int j = 0; j < TABLE_SIZE; j++) {
			fprintf(stat, "%d %d\n", j, table[j]->size);
		}
		fclose (stat);
		plotting (var[i].name, disp);
		memset (file, '\0', 25);
		database = fopen ("contacts.dat", "w");
		fileWrite (database, table);
		tableDelete (table);
	}
	return 0;
}
