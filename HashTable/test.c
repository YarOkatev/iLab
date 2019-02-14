//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef _TEST_C_INCLUDED_
#define _TEST_C_INCLUDED_

#include "test.h"

void plotting (char* name, double disp) {
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

double dispersion (List** table) {
	double sumdX = 0, sumX = 0;
	for (int i = 0; i < TABLE_SIZE; i++) sumX  += table[i]->size;
	for (int i = 0; i < TABLE_SIZE; i++) sumdX += (table[i]->size - (sumX / TABLE_SIZE)) * (table[i]->size - (sumX / TABLE_SIZE));
	return (sumdX / TABLE_SIZE);
}

struct Method* makeFuncArray () {
	struct Method* var = (struct Method*) calloc (4, sizeof (struct Method));
	strcpy (var[0].name, "Hash DigitSum");
	var[0].hfunc = Hash_DigitSum;
	strcpy (var[1].name, "Hash XOR");
	var[1].hfunc = Hash_XOR;
	strcpy (var[2].name, "Hash HalfXOR");
	var[2].hfunc = Hash_HalfXOR;
	strcpy (var[3].name, "Hash Mod");
	var[3].hfunc = Hash_Mod;
	return var;
}

#endif //_TEST_C_INCLUDED_
