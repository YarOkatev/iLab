#include "stack.h"

int main () {
	class Stack st(12), st2 (12);

	for (int i = 0; i < 10; i++) {
		st.push (i*i + 100);
	}
	for (int i = 0; i < 11; i++) {
		st2.push (i + 100);
	}
	printf ("%d", st.pop ());
	printf ("dd %d %d\n", st.size (), st2.size ());
	return 0;
}