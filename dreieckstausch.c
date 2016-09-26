#include <iostream>
using namespace std;

void tausch(int &a, int &b);
void tausch2(int *a, int *b);

void main() {
	int a = 5;
	int b = 1;
	int *p1=&a, *p2=&b;
	tausch(a, b);
	cout << a << '\t' << b << '\n';
	tausch2(p1, p2);
	cout << a << '\t' << b << '\n';
}

void tausch(int &a, int &b) {
	int c = a;
	a = b;
	b = c;
}

void tausch2(int *a, int *b) { // kann nicht gehen
	int *c = a;
	a = b;
	b = c;
}
