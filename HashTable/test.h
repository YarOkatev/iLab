//
// Created by Ярослав Окатьев  on 2019-02-10.
//

#ifndef HASHTABLE_TEST_H
#define HASHTABLE_TEST_H

#include "hashtable.h"

struct Method {
	char name[20];
	long long int (*hfunc) (long long int);
};

void plotting (char* name, double disp);
double dispersion (List** table);
struct Method* makeFuncArray ();

#include "test.c"

#endif //HASHTABLE_TEST_H
