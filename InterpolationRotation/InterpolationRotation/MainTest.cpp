#include <iostream>
#include "InterpolationRotation.h"

using namespace std;

int main() {

	while (true) {
		cout << "1. Bilinear Interpoloation" << endl;
		cout << "2. Rotation" << endl;

		cout << "What = ";
		int n;
		cin >> n;
		system("cls");
		if (n == 1) {
			cout << "1. x 2(default)" << endl;
			cout << "2. x 1.703125" << endl;
			cout << "What = ";
			int n;
			int num;
			cin >> num;
			switch (num)
			{
			case 2:
				BilinearInterpolation(1.703125);
				break;
			default:
				BilinearInterpolation(2);
				break;
			}
			system("cls");
			cout << "Complete Bilinear Interpolation" << endl;
		}
		else if (n == 2)
		{
			cout << "1. 30(default)" << endl;
			cout << "2. 45" << endl;
			cout << "3. 60" << endl;
			cout << "What = ";
			int n;
			int num;
			cin >> num;
			switch (num)
			{
			case 2:
				Rotation(45);
				break;
			case 3:
				Rotation(60);
				break;
			default:
				Rotation(30);
				break;
			}
			system("cls");
			cout << "Complete Bilinear Rotation" << endl;
		}
		else {
			cout << "Fail" << endl;
		}

		cout << "RESTART?" << endl;
		cout << "Yes(1) No(2)" << endl;
		cout << "What = ";
		int me;
		cin >> me;
		if (me == 2) break;
		system("cls");
	}
	return 0;
}




