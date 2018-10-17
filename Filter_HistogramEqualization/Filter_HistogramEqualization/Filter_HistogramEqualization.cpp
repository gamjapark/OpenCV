

#include "Filter_HistogramEqualization.h"


//Histogram Equlization
void HistogramEqualization()
{	//2차원 메모리에 입력 영상 읽어오기 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "input.raw");

	//히스토그램을 얻기위한 1차원 메모리 공간할당
	int* Histogram = new int[256];
	memset(Histogram, 0, sizeof(int) * 256);

	//입력 영상의 히스토그램 얻기
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++) Histogram[Input2D[h][w]]++;

	//히스토그램 누적 분포 함수 구하기
	double Histogram_CDF[256] = { 0.0 };
	double out[256] = { 0 };

	Histogram_CDF[0] = (double)Histogram[0] / (HEIGHT * WIDTH);
	out[0] = round(Histogram_CDF[0] * 255);
	for (int i = 1; i < 256; i++) {
		Histogram_CDF[i] = (double)Histogram[i] / (HEIGHT * WIDTH) + Histogram_CDF[i - 1];
		out[i] = round(Histogram_CDF[i] * 255);
	}

	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Histogram Equalization을 한 영상 생성
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++)
			Output2D[h][w] = out[Input2D[h][w]];

	//출력 영상 생성
	FileMake(Output2D, HEIGHT, WIDTH, "HistogramEqualization.raw");

	//1차원, 2차원 동적할당 해제 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, HEIGHT);
	delete Histogram;
}


//Average Filter
void SpatialFilter_Average(int nFilterSize) {
	//2차원 메모리에 입력 영상 읽어오기 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//입력영상 Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, nFilterSize);

	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter 적용하기
	AverageFilter(Pad, Output2D, HEIGHT, WIDTH, nFilterSize);

	//출력 영상 생성
	char result[100];
	if (nFilterSize == 3)
		sprintf(result, "AverageFilter%d.raw", 1);
	else if (nFilterSize == 7) 	sprintf(result, "AverageFilter%d.raw", 2);

	FileMake(Output2D, HEIGHT, WIDTH, result);

	//2차원 동적할당 해제 
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
	//2차원 메모리에 입력 영상 읽어오기 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//입력영상 Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, 3);

	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter 적용하기
	SmoothFilter(Pad, Output2D, HEIGHT, WIDTH, 3);

	//출력 영상 생성
	FileMake(Output2D, HEIGHT, WIDTH, "SmoothFilter.raw");

	//2차원 동적할당 해제 
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
	//2차원 메모리에 입력 영상 읽어오기 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "AverageFilter1.raw");

	//입력영상 Padding
	unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, 3);

	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//Average Filter 적용하기
	SharpenFilter(Pad, Output2D, HEIGHT, WIDTH, 3);

	//출력 영상 생성
	FileMake(Output2D, HEIGHT, WIDTH, "SharpenFilter.raw");

	//2차원 동적할당 해제 
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
		//2차원 메모리에 입력 영상 읽어오기 
		unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, FileName);

		//입력영상 Padding
		unsigned char** Pad = Padding(Input2D, HEIGHT, WIDTH, FilterSize);

		//결과 영상을 담기 위한 이차원 메모리 공간할당
		unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

		//Average Filter 적용하기
		MedianFilter(Pad, Output2D, HEIGHT, WIDTH, FilterSize);

		//출력 영상 생성
		string BUF = FileName;
		string buf = BUF.substr(0, BUF.size() - 4);
		stringstream ss;
		ss << buf << "MedianFilter" << FilterSize << ".raw";
		buf = ss.str();
		char * filename = new char[buf.size() + 1];
		copy(buf.begin(), buf.end(), filename);
		filename[buf.size()] = '\0';
		FileMake(Output2D, HEIGHT, WIDTH, filename);
		//2차원 동적할당 해제 
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
			//Filter 내부의 갯수만큼 주의 픽셀값들을 1차원 배열에 저장
			int n = 0;
			for (int h2 = h; h2 < h + FilterSize; h2++) {
				for (int w2 = w; w2 < w + FilterSize; w2++) {
					list[n] = In_Pad[h2][w2];
					n++;
				}
			}
			//Insertion sort => 해당 정렬 알고리즘을 사용하여 1차원 배열 정렬
			for (int i = 1; i < FilterSize * FilterSize; i++) {
				int key = list[i], j;
				for (j = i - 1; j >= 0; j--) {
					if (list[j] > key) list[j + 1] = list[j];
					else break;
				}
				list[j + 1] = key;
			}
			//Median value selection => 정렬된 배열에서 중간 값 선택
			Out[h][w] = list[(FilterSize * FilterSize) / 2];
			//1차원 배열 동적할당 해제
			delete[] list;
		}
	}

}

