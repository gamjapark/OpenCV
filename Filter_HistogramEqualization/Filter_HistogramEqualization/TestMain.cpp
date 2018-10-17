#include <iostream>
#include "Filter_HistogramEqualization.h"

using namespace std;

int main() {

	while (true) {
		cout << "1. HistogramEqualization" << endl;
		cout << "2. SpatialFilter_Average" << endl;
		cout << "3. SpatialFilter_Smooth" << endl;
		cout << "4. SpatialFilter_Sharpen" << endl;
		cout << "5. SpatialFilter_Median" << endl;

		cout << "What = ";
		int n;
		cin >> n;
		system("cls");

		int size;
		switch (n)
		{
		case 1:
			HistogramEqualization();
			system("cls");
			cout << "Complete Histogram Equalization" << endl;
			break;
		case 2:
			system("cls");
			cout << "Input your Filter Size" << endl;
			cout << "What = ";
			cin >> size;
			SpatialFilter_Average(size);

			system("cls");
			cout << "Complete SpatialFilter Average" << endl;
			break;
		case 3:
			SpatialFilter_Smooth();
			system("cls");
			cout << "Complete SpatialFilter Smooth" << endl;
			break;
		case 4:
			SpatialFilter_Average(3);
			SpatialFilter_Sharpen();
			system("cls");
			cout << "Complete SpatialFilter Sharpen" << endl;
			break;
		case 5:
			system("cls");
			cout << "1. lena256_n5.raw" << endl;
			cout << "2. lena256_n10.raw" << endl;
			cout << "3. lena256_n25.raw" << endl;
			cout << "What = ";
			int input;
			cin >> input;

			system("cls");
			cout << "Input your Filter Size" << endl;
			cout << "What = ";
			cin >> size;

			switch (input)
			{
			case 1:
				SpatialFilter_Median(size, "lena256_n5.raw");
				break;
			case 2:
				SpatialFilter_Median(size, "lena256_n10.raw");
				break;
			case 3:
				SpatialFilter_Median(size, "lena256_n25.raw");
				break;
			}

			system("cls");
			cout << "Complete SpatialFilter Median" << endl;
			break;
		default:
			cout << "Fail" << endl;
			break;
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