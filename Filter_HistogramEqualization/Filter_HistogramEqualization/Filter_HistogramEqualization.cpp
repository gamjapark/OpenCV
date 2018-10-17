

#include "Filter_HistogramEqualization.h"


//Histogram Equlization
void HistogramEqualization()
{	//2���� �޸𸮿� �Է� ���� �о���� 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "input.raw");

	//������׷��� ������� 1���� �޸� �����Ҵ�
	int* Histogram = new int[256];
	memset(Histogram, 0, sizeof(int) * 256);

	//�Է� ������ ������׷� ���
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++) Histogram[Input2D[h][w]]++;

	//������׷� ���� ���� �Լ� ���ϱ�
	double Histogram_CDF[256] = { 0.0 };
	double out[256] = { 0 };

	Histogram_CDF[0] = (double)Histogram[0] / (HEIGHT * WIDTH);
	out[0] = round(Histogram_CDF[0] * 255);
	for (int i = 1; i < 256; i++) {
		Histogram_CDF[i] = (double)Histogram[i] / (HEIGHT * WIDTH) + Histogram_CDF[i - 1];
		out[i] = round(Histogram_CDF[i] * 255);
	}

	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Histogram Equalization�� �� ���� ����
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++)
			Output2D[h][w] = out[Input2D[h][w]];

	//��� ���� ����
	FileMake(Output2D, HEIGHT, WIDTH, "HistogramEqualization.raw");

	//1����, 2���� �����Ҵ� ���� 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, HEIGHT);
	delete Histogram;
}


//Average Filter
void SpatialFilter_Average(int nFilterSize) {
	//2���� �޸𸮿� �Է� ���� �о���� 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//�Է¿��� Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, nFilterSize);

	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter �����ϱ�
	AverageFilter(Pad, Output2D, HEIGHT, WIDTH, nFilterSize);

	//��� ���� ����
	char result[100];
	if (nFilterSize == 3)
		sprintf(result, "AverageFilter%d.raw", 1);
	else if (nFilterSize == 7) 	sprintf(result, "AverageFilter%d.raw", 2);

	FileMake(Output2D, HEIGHT, WIDTH, result);

	//2���� �����Ҵ� ���� 
	free2D(Pad, HEIGHT + (int)(nFilterSize / 2) * 2);
	free2D(Output2D, HEIGHT);
}

void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0;
			for (int h2 = h; h2 < h + FilterSize; h2++)
				for (int w2 = w; w2 < w + FilterSize; w2++)
					sum += In_Pad[h2][w2];
			Out[h][w] = sum / (FilterSize * FilterSize);
		}
	}
}


//Smooth Filter
void SpatialFilter_Smooth() {
	//2���� �޸𸮿� �Է� ���� �о���� 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//�Է¿��� Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, 3);

	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter �����ϱ�
	SmoothFilter(Pad, Output2D, HEIGHT, WIDTH, 3);

	//��� ���� ����
	FileMake(Output2D, HEIGHT, WIDTH, "SmoothFilter.raw");

	//2���� �����Ҵ� ���� 
	free2D(Pad, HEIGHT + 2);
	free2D(Output2D, HEIGHT);
}

void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0.0;
			for (int i = 0; i < FilterSize; i++) {
				for (int j = 0; j < FilterSize; j++) {
					if ((i == 0 || i == FilterSize - 1) && (j == 0 || j == FilterSize - 1))
						sum += (double)In_Pad[h + i][w + j] * 0.0625;
					else if (i == j && i == FilterSize / 2)
						sum += (double)In_Pad[h + i][w + j] * 0.25;
					else if (i != j && (i == FilterSize / 2 || j == FilterSize / 2))
						sum += (double)In_Pad[h + i][w + j] * 0.125;
				}
			}
			if (sum > 255) sum = 255;
			Out[h][w] = sum;
		}
	}
}

// Sharpening Filter
void SpatialFilter_Sharpen() {
	//2���� �޸𸮿� �Է� ���� �о���� 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "AverageFilter1.raw");

	//�Է¿��� Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, 3);

	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter �����ϱ�
	SharpenFilter(Pad, Output2D, HEIGHT, WIDTH, 3);

	//��� ���� ����
	FileMake(Output2D, HEIGHT, WIDTH, "SharpenFilter.raw");

	//2���� �����Ҵ� ���� 
	free2D(Pad, HEIGHT + 2);
	free2D(Output2D, HEIGHT);
}

void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0;
			for (int i = 0; i < FilterSize; i++) {
				for (int j = 0; j < FilterSize; j++) {
					if ((i == 0 || i == FilterSize - 1) && (j == 0 || j == FilterSize - 1))
						sum += In_Pad[h + i][w + j] * 0;
					else if (i == j && i == FilterSize / 2)
						sum += In_Pad[h + i][w + j] * 5;
					else if (i != j && (i == FilterSize / 2 || j == FilterSize / 2))
						sum += In_Pad[h + i][w + j] * -1;
				}
			}
			if (sum < 0) sum = 0;
			else if (sum > 255) sum = 255;
			Out[h][w] = sum;
		}
	}
}


//Median Filter
void SpatialFilter_Median(int FilterSize, const char *FileName) {
	if (FilterSize >= 3) {
		if (FilterSize % 2 == 0) FilterSize++;
		//2���� �޸𸮿� �Է� ���� �о���� 
		unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, FileName);

		//�Է¿��� Padding
		unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, FilterSize);

		//��� ������ ��� ���� ������ �޸� �����Ҵ�
		unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

		//Average Filter �����ϱ�
		MedianFilter(Pad, Output2D, HEIGHT, WIDTH, FilterSize);

		//��� ���� ����
		string BUF = FileName;
		string buf = BUF.substr(0, BUF.size() - 4);
		stringstream ss;
		ss << buf << "MedianFilter" << FilterSize << ".raw";
		buf = ss.str();
		char * filename = new char[buf.size() + 1];
		copy(buf.begin(), buf.end(), filename);
		filename[buf.size()] = '\0';
		FileMake(Output2D, HEIGHT, WIDTH, filename);
		//2���� �����Ҵ� ���� 
		free2D(Pad, HEIGHT + (int)(FilterSize / 2) * 2);
		free2D(Output2D, HEIGHT);
		delete[] filename;
	}
}
void MedianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			unsigned char* list = new unsigned char[FilterSize * FilterSize];
			memset(list, 0, sizeof(unsigned char) * (FilterSize * FilterSize));
			//Filter ������ ������ŭ ���� �ȼ������� 1���� �迭�� ����
			int n = 0;
			for (int h2 = h; h2 < h + FilterSize; h2++) {
				for (int w2 = w; w2 < w + FilterSize; w2++) {
					list[n] = In_Pad[h2][w2];
					n++;
				}
			}
			//Insertion sort => �ش� ���� �˰����� ����Ͽ� 1���� �迭 ����
			for (int i = 1; i < FilterSize * FilterSize; i++) {
				int key = list[i], j;
				for (j = i - 1; j >= 0; j--) {
					if (list[j] > key) list[j + 1] = list[j];
					else break;
				}
				list[j + 1] = key;
			}
			//Median value selection => ���ĵ� �迭���� �߰� �� ����
			Out[h][w] = list[(FilterSize * FilterSize) / 2];
			//1���� �迭 �����Ҵ� ����
			delete[] list;
		}
	}

}

