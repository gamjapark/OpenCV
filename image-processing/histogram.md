# Histogram

## Introduction

영상의 Pixel 값의 발생 빈도를 나타내는 그래프를 의미합니다. 

![](../.gitbook/assets/image%20%2820%29.png)

위와 같은 히스토그램의 가로축은 Pixel의 범위\(0~255\)를 나타내고,

세로축은 입력 영상에서 Pixel의 빈도 수를 나타냅니다.

## Practice 1 Using C++

이는 입력 영상의 히스토그램을 구하고 정규화시킨 후, 히스토그램을 출력시키는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2\opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define HEIGHT 256
#define WIDTH 256

void main() {

	//1차원 배열에 입력 영상 받아오기
	unsigned char* InputArray = new unsigned char[HEIGHT * WIDTH];
	FILE* fp;
	fopen_s(&fp, "lena256.raw", "rb");
	fread(InputArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	//히스토그램 얻기
	int Hist[256] = { 0 };
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++)
			Hist[InputArray[h * WIDTH + w]]++;

	//히스토그램의 최댓값 구하기
	int max = 0;
	for (int i = 0; i < 256; i++)
		if (max < Hist[i]) max = Hist[i];

	//히스토그램 영상을 저장할 배열 선언
	unsigned char* Output = new unsigned char[HEIGHT * WIDTH];
	
	//결과 영상확인을 위해 opencv 사용함
	Mat image = Mat(256,256, CV_8UC1);
	image = Scalar::all(0);

	double NormalFactor = 255.0 / max;
	for (int w = 0; w < 256; w++) {
		int normal = (int)Hist[w] * NormalFactor;
		for (int h = 255; h > 255 - normal; h--) {
			Output[h * WIDTH + w] = 255;
			image.at<uchar>(h, w) = 255;
		}
	}
	
	Mat input(Size(WIDTH, HEIGHT), 0);
	for (int h = 0; h < HEIGHT; h++) 
		for (int w = 0; w < WIDTH; w++) 
			input.at<uchar>(h, w) = InputArray[h * WIDTH + w];

	imshow("lena", input);
	imshow("Histo", image);
	delete[] InputArray;
	delete[] Output;

	waitKey(0);
}

```

## Practice 1 Result

![](../.gitbook/assets/image%20%2812%29.png)

## Histogram Equalization

히스토그램을 이용해서 영상의 명암을 전체적으로 고르게 분포시키는 것입니다.

아마, 영상의 분포가 아래와 같이 되기를 원할 것입니다.

![](../.gitbook/assets/image%20%2845%29.png)

하지만, Histogram Equalization을 통해서는 이를 완벽하게 할 수 없고, 실제로는 다음과 같이 수행할 수 있습니다. 

![](../.gitbook/assets/image%20%285%29.png)

이를 구현하는 방법을 살펴보겠습니다.

1. 가장 먼저 앞에서 했던 바와 같이 영상의 히스토그램을 구합니다.
2. 히스토그램의 누적 합을 \[0, 1\] 로 정규화한 누적 분포 함수\(CDF\)를 구합니다.
3. CDF 값을 Scaling합니다.

## Practice 2 Using C++

이는 Histogram Equalization을 구현해보는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2\opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define HEIGHT 256
#define WIDTH 256
void DisplayHistogram(int * Hist, const char* me) {

	int max = 0; 
	for (int i = 0; i < 256; i++)
		if (max < Hist[i]) max = Hist[i];

	double NormalFactor = 255.0 / max;

	Mat image = Mat(256, 256, CV_8UC1);
	image = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int normal = (int)Hist[w] * NormalFactor;
		for (int h = 255; h > 255 - normal; h--) {
			image.at<uchar>(h, w) = 255;
		}
	}

	imshow(me, image);
}
void main() {

	//1차원 배열에 입력 영상 받아오기
	unsigned char* InputArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char* OutputArray = new unsigned char[HEIGHT * WIDTH];

	FILE* fp;
	fopen_s(&fp, "lena256.raw", "rb");
	fread(InputArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	//영상의 히스토그램 얻기
	int *Hist = new int[256];
	memset(Hist, 0, sizeof(int) * 256);

	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++)
			Hist[InputArray[h * WIDTH + w]]++;

	//Histogram 누적분포함수 얻기
	double HistCDF[256] = { 0.0 };
	double out[256] = { 0.0 };
	HistCDF[0] = (double)Hist[0] / (HEIGHT * WIDTH);
	for (int i = 1; i < 256; i++) {
		HistCDF[i] = (double)Hist[i] / (HEIGHT * WIDTH) + HistCDF[i - 1];
		out[i] = round(HistCDF[i] * 255);	//Scaling

	}
	
	//Histogram Equalization 적용한 출력 영상과 그의 히스토그램 얻기
	int* outHist = new int[256];
	memset(outHist, 0, sizeof(int) * 256);
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			OutputArray[h * WIDTH + w] = out[InputArray[h * WIDTH + w]];
			outHist[OutputArray[h * WIDTH + w]]++;
		}
	}
	
	Mat inputImage(256, 256, CV_8UC1);
	Mat outputImage(256, 256, CV_8UC1);
	inputImage = Scalar::all(0);
	outputImage = Scalar::all(0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			inputImage.at<uchar>(h, w) = InputArray[h * WIDTH + w];
			outputImage.at<uchar>(h, w) = OutputArray[h * WIDTH + w];
		}
	}

	imshow("InputImage", inputImage);
	imshow("outputImage", outputImage);

	DisplayHistogram(Hist, "input Histogram");
	DisplayHistogram(outHist, "output Histogram");

	delete[] InputArray;
	delete[] OutputArray;
	delete[] Hist;
	delete[] outHist;
	waitKey(0);
}

```

## Practice 2 Result

![](../.gitbook/assets/image%20%282%29.png)

## Histogram Specification\(Matching\)

이는 입력 영상의 Histogram이 원하는 형태의 Histogram이 되도록 만들어 주는 것입니다.

이를 통해 영상의 일부 Contrast를 개선할 수 있습니다. 

즉, Histogram Equalization은 Histogram의 값이 동일하게 되도록 matching하는 것이므로

Histogram Matching에 속한다고 볼 수 있습니다.

{% hint style="info" %}
여기서, CDF의 값은 실수의 형태로 존재하기 때문에

완전히 일치하는 것을 찾을 수는 없습니다.

따라서, 완전 일치하는 것이 아닌 차이가 최소가 되는 쪽으로 값을 찾아야합니다.
{% endhint %}

이를 구현하는 과정은 다음과 같습니다.

1. 원본 영상과 참조 영상\(Target\)의 Histogram을 구합니다.
2. 각각의 Histogram의 CDF를 계산합니다. \(원본 G\(x\), 참조 H\(x\)\)
3. 원본 영상의 CDF를 참조 영상\(Target\)의 CDF를 통해 변환합니다. \(여기서, G\(xi\) - H\(xj\)가 최소가 되는 xj의 값을 찾습니다.\)

![](../.gitbook/assets/image%20%2825%29.png)

## Practice 3 Using C++

이는 Histogram Matching을 구현해보는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2\opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define HEIGHT 256
#define WIDTH 256
void DisplayHistogram(int * Hist, const char* me) {

	int max = 0; 
	for (int i = 0; i < 256; i++)
		if (max < Hist[i]) max = Hist[i];

	double NormalFactor = 255.0 / max;

	Mat image = Mat(256, 256, CV_8UC1);
	image = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int normal = (int)Hist[w] * NormalFactor;
		for (int h = 255; h > 255 - normal; h--) {
			image.at<uchar>(h, w) = 255;
		}
	}

	imshow(me, image);
}
void main() {

	unsigned char* InputArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char* OutputArray = new unsigned char[HEIGHT * WIDTH];
	memset(OutputArray, 0, sizeof(unsigned char) * HEIGHT * WIDTH);

	FILE *fp;
	fopen_s(&fp, "lena256.raw", "rb");
	fread(InputArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	unsigned char* RefArray = new unsigned char[HEIGHT * WIDTH];
	srand(getTickCount());

	int *Hist = new int[256];
	int *RefHist = new int[256];
	int *OutHist = new int[256];
	memset(Hist, 0, sizeof(int) * 256);
	memset(RefHist, 0, sizeof(int) * 256);
	memset(OutHist, 0, sizeof(int) * 256);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			RefArray[h * WIDTH + w] = (unsigned char)rand() % 255;
			Hist[InputArray[h * WIDTH + w]]++;
			RefHist[RefArray[h * WIDTH + w]]++;
		}
	}

	double HistCDF[256] = { 0.0 };
	double HistRefCDF[256] = { 0.0 };
	double out[256] = { 0.0 };

	HistCDF[0] = (double)Hist[0] / (HEIGHT * WIDTH);
	HistRefCDF[0] = (double)RefHist[0] / (HEIGHT * WIDTH);
	for (int i = 1; i < 256; i++) {
		HistCDF[i] = (double)Hist[i] / (HEIGHT * WIDTH) + HistCDF[i - 1];
		HistRefCDF[i] = (double)RefHist[i] / (HEIGHT * WIDTH) + HistRefCDF[i - 1];
	}


	for (int i = 0; i < 256; i++) {
		double min = 1;
		int FindNum = 0;
		for (int j = 0; j < 256; j++) {
			if (HistCDF[i] == HistRefCDF[j]) {
				FindNum = j;
				break;
			}
			if (abs(HistCDF[i] - HistRefCDF[j]) < min) {
				min = abs(HistCDF[i] - HistRefCDF[j]);
				FindNum = j;
			}
		}
		out[i] = round(255 * HistRefCDF[FindNum]);
	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			OutputArray[h * WIDTH + w] = out[InputArray[h * WIDTH + w]];
			OutHist[OutputArray[h * WIDTH + w]]++;
		}
	}
	
	//결과 출력을 위해 opencv 사
	Mat InputImage(HEIGHT, WIDTH, CV_8UC1);
	Mat RefImage(HEIGHT, WIDTH, CV_8UC1);
	Mat OutImage(HEIGHT, WIDTH, CV_8UC1)
	InputImage = Scalar::all(0);
	RefImage = Scalar::all(0);
	OutImage = Scalar::all(0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			InputImage.at<uchar>(h, w) = InputArray[h * WIDTH + w];
			OutImage.at<uchar>(h, w) = OutputArray[h * WIDTH + w];
			RefImage.at<uchar>(h, w) = RefArray[h * WIDTH + w];
		}
	}

	imshow("Input", InputImage);
	imshow("Reference", RefImage);
	imshow("Output", OutImage);

	DisplayHistogram(Hist, "Input Histogram");
	DisplayHistogram(RefHist, "Reference Histogram");
	DisplayHistogram(OutHist, "Output Histogram");

	delete[] InputArray;
	delete[] OutputArray;
	delete[] RefArray;
	delete[] Hist;
	delete[] RefHist;
	delete[] OutHist;

	waitKey(0);
}

```

## Practice 3 Result

![](../.gitbook/assets/image%20%2855%29.png)



