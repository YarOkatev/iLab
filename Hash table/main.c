#include "hashtable.h"

struct Method {
    char name[20];
    long long int (*hfunc) (long long int);
};

void Plotting (char* name, double disp);
double Dispersion (List** table);

int main () {
    struct Method var[4];
    strcpy (var[0].name, "Hash DigitSum");
    var[0].hfunc = Hash_DigitSum;
    strcpy (var[1].name, "Hash XOR");
    var[1].hfunc = Hash_XOR;
    strcpy (var[2].name, "Hash HalfXOR");
    var[2].hfunc = Hash_HalfXOR;
    strcpy (var[3].name, "Hash Mod");
    var[3].hfunc = Hash_Mod;

    FILE* database = NULL;
    List** table = NULL;
    char file[25];
    double disp = 0;
    FILE* stat = NULL;

    system ("./phonegen.exe");

    for (int i = 0; ; i++) {
        table = TableInit ();
        disp = 0;
        database = fopen ("contacts.dat", "r");
        FileRead (database, table, var[i].hfunc);
        disp =  Dispersion (table);
        strcat (file, var[i].name);
        strcat (file, ".csv");
        stat = fopen (file, "w");
        for (int j = 0; j < TABLE_SIZE; j++) {
            fprintf(stat, "%d %d\n", j, table[j]->size);
        }
        fclose (stat);
        Plotting (var[i].name, disp);
        memset (file, '\0', 25);
        TableDelete (table);
    }
    return 0;
}

void Plotting (char* name, double disp) {
    FILE* plot = fopen ("plot.p", "w");
    fprintf (plot, "set terminal png\n");
    fprintf (plot, "set xrange [0:*]\n");
    fprintf (plot, "set yrange [0:*]\n");
    fprintf (plot, "set style data boxes\n");
    fprintf (plot, "set nokey\n");
    fprintf (plot, "set style fill solid 1\n");
    fprintf (plot, "set boxwidth 1 absolute\n");
    fprintf (plot, "set title \"%s\"\n", name);
	fprintf (plot, "set output \'%s.png\'\n", name);
	fprintf (plot, "set label \"Dispersion: %lg\" at graph 0.65, 1.05 font \",15\"\n", disp);
	fprintf (plot, "plot \'%s.csv\' u 2\n", name);
    fclose (plot);
    system ("gnuplot plot.p");
    remove ("plot.p");
}

double Dispersion (List** table) {
    double sumdX = 0, sumX = 0;
    for (int i = 0; i < TABLE_SIZE; i++) sumX  += table[i]->size;
    for (int i = 0; i < TABLE_SIZE; i++) sumdX += (table[i]->size - (sumX / TABLE_SIZE)) * (table[i]->size - (sumX / TABLE_SIZE));
    return (sumdX / TABLE_SIZE);
}
