#include <iostream>
using namespace std;

void main() {
	bool taster = 1, diagn = 1, status = 0;
	int motor;

	if ((taster == 0) || (diagn == 0)) {
		if (status == 1){
			motor = 2;
		}
		else {motor = 0;}
	}
	else if (status == 0) {
		motor = 1;
	}
	else { motor = 0; }
	cout << motor << endl;
}
