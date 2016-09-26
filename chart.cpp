#include <iostream>
using namespace std;

void chart1(int in_1, int in_2, int in_3, int &out_1, int &out_2);
void chart2(int in_1, int in_2, int in_3, int &out_1, int &out_2);
void chart3(int in_1, int in_2, int in_3, int &out_1, int &out_2);


void main() {
	int in_1, in_2, in_3, out_1=0, out_2=0;
	in_1 = 1;
	in_2 = 0;
	in_3 = 0;
	chart1(in_1, in_2, in_3, out_1, out_2);
	in_1 = 0;
	chart2(in_1, in_2, in_3, out_1, out_2);
	in_1 = 4;
	chart3(in_1, in_2, in_3, out_1, out_2);

}

void chart1(int in_1, int in_2, int in_3, int &out_1, int &out_2) {
	if (in_1 || in_2) {
		if (in_1 == 4) {
			out_1 = 3;
			out_2 = 1;
		}
		else{
			out_1 = 1;
			out_2 = 1;
		}
	}
	else{
		out_1 = 0;
		out_2 = 0;
	}
}

void chart2(int in_1, int in_2, int in_3, int &out_1, int &out_2) {
	static int local_1 = 0;
	local_1 = 0;
	if (in_1 || in_2 || local_1==1) {
		if (in_1 == 4) {
			out_1 = 3;
			out_2 = 1;
		}
		else{
			out_1 = 1;
			out_2 = 1;
			local_1++;
		}
	}
	else{
		out_1 = 0;
		out_2 = 0;
	}
}

void chart3(int in_1, int in_2, int in_3, int &out_1, int &out_2) {
	static int local_1 = 0;
	if (in_1 || in_2) {
		if (in_1 == 4) {
			out_1 = 3;
			out_2 = 1;
			local_1 = 1;
		}
		else{
			out_1 = 1;
			out_2 = 1;
			local_1++;
		}
	}
	else{
		out_1 = 0;
		out_2 = 0;
	}
}
