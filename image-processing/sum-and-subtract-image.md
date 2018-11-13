# Sum & Subtract Image

## Introduction

픽셀 값에서 고정된 값을 더하거나 빼어서 영상의 밝기를 조절할 수 있습니다.

이때, 더하거나 뺄 때, 0~255의 픽셀 값을 넘어가는 것에 대한 예외처리를 해야합니다.

## Practice 1 Using OpenCV

이는 입력 영상에서 고정된 값인 80을 더하고 빼서 더 밝고 어두운 출력 영상을 만드는 예제입니다.

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

#define WIDTH 512
#define HEIGHT 512

void main() {
	Mat img = imread("lena.jpg", 1);

	Mat GrayImg(Size(HEIGHT, WIDTH), 0);
	Mat DarkImg(Size(HEIGHT, WIDTH), 0);
	Mat BrightImg(Size(HEIGHT, WIDTH), 0);

	//Gray Scale Image 얻기
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Vec3b RGB = img.at<Vec3b>(h, w);
			GrayImg.at<uchar>(h, w) = RGB[2] * 0.299 + RGB[1] * 0.587 + RGB[0] * 0.114;
		}
	}

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//어두운 이미지 얻기
			if (GrayImg.at<uchar>(h, w) - 80 >= 0)
				DarkImg.at<uchar>(h, w) = GrayImg.at<uchar>(h, w) - 80;
			else DarkImg.at<uchar>(h, w) = 0;
			//밝은 이미지 얻기
			if (GrayImg.at<uchar>(h, w) + 80 <= 255)
				BrightImg.at<uchar>(h, w) = GrayImg.at<uchar>(h, w) + 80;
			else BrightImg.at<uchar>(h, w) = 255;
		}
	}

	imshow("GrayImage", GrayImg);
	imshow("DarkImage", DarkImg);
	imshow("BrightImage", BrightImg);
	waitKey(0);
}
```

## Practice 1 Using C++

이는 입력 영상에서 고정된 값인 80을 더하고 빼서 더 밝고 어두운 출력 영상을 만드는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define WIDTH 512
#define HEIGHT 512

void main() {

	unsigned char * InputArray = new unsigned char[HEIGHT *WIDTH];
	unsigned char * DarkArray = new unsigned char[HEIGHT *WIDTH];
	unsigned char * BrightArray = new unsigned char[HEIGHT *WIDTH];

	FILE* fp;
	fopen_s(&fp, "lena.raw", "rb");
	fread(InputArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//어두운 이미지 얻기
			if (InputArray[h * WIDTH + w] - 80 >= 0)
				DarkArray[h * WIDTH + w] = InputArray[h * WIDTH + w] - 80;
			else DarkArray[h * WIDTH + w] = 0;

			//밝은 이미지 얻기
			if (InputArray[h * WIDTH + w] + 80 <= 255)
				BrightArray[h * WIDTH + w] = InputArray[h * WIDTH + w] + 80;
			else BrightArray[h * WIDTH + w] = 255;
		}
	}

	//결과 확인을 위해 Opencv함수 사용
	Mat GrayImg(Size(WIDTH, HEIGHT), 0);
	Mat DarkImg(Size(WIDTH, HEIGHT), 0);
	Mat BrightImg(Size(WIDTH, HEIGHT), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			GrayImg.at<uchar>(h, w) = InputArray[h * WIDTH + w];
			DarkImg.at<uchar>(h, w) = DarkArray[h * WIDTH + w];
			BrightImg.at<uchar>(h, w) = BrightArray[h * WIDTH + w];
		}
	}

	imshow("GrayImage", GrayImg);
	imshow("DarkImage", DarkImg);
	imshow("BrightImage", BrightImg);

	delete[] InputArray;
	delete[] DarkArray;
	delete[] BrightArray;

	waitKey(0);
}
```

## Practice 1 Result

Practice 1의 출력 결과는 다음과 같습니다.

![](../.gitbook/assets/image%20%2826%29.png)

![](../.gitbook/assets/image%20%2813%29.png)

## Practice 2 Using OpenCV

이는 입력 영상에서 Reversed 영상을 얻어 해당 픽셀 값을 빼서 출력 영상을 만드는 예제입니다.

```cpp
#include <opencv2\opencv.hpp>
using namespace cv;

#define HEIGHT 512
#define WIDTH 512

void main() {

	Mat GrayImage = imread("lena.jpg", 0);
	Mat ReversedImg(Size(WIDTH, HEIGHT), 0);
	Mat SubImg(Size(HEIGHT, HEIGHT), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//Reversed Image 얻기
			ReversedImg.at<uchar>(h, w) = 255 - GrayImage.at<uchar>(h, w);

			//GrayImage에서 Reversed Image 픽셀값을 뺀 Image 얻기
			if (GrayImage.at<uchar>(h, w) - ReversedImg.at<uchar>(h, w) < 0)
				SubImg.at<uchar>(h, w) = 0;
			else
				SubImg.at<uchar>(h, w) = GrayImage.at<uchar>(h, w) - ReversedImg.at<uchar>(h, w);
		}
	}

	imshow("GrayImage", GrayImage);
	imshow("ReversedImage", ReversedImg);
	imshow("SubtractImage", SubImg);
	waitKey(0);
}
```

## Practice 2 Using C++

이는 입력 영상에서 Reversed 영상을 얻어 해당 픽셀 값을 빼서 출력 영상을 만드는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2\opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define HEIGHT 512
#define WIDTH 512

void main() {

	unsigned char* InputArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char* ReversedArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char* SubArray = new unsigned char[HEIGHT * WIDTH];

	FILE* fp;
	fopen_s(&fp, "lena.raw", "rb");
	fread(InputArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//Reversed Image 얻기
			ReversedArray[h * WIDTH + w] = 255 - InputArray[h * WIDTH + w];

			//GrayImage에서 Reversed Image 픽셀값을 뺀 Image 얻기
			if (InputArray[h * WIDTH + w] - ReversedArray[h * WIDTH + w] < 0)
				SubArray[h * WIDTH + w] = 0;
			else
				SubArray[h * WIDTH + w] = InputArray[h * WIDTH + w] - ReversedArray[h * WIDTH + w];
		}
	}

	//결과 확인을 위해 Opencv함수 사용
	Mat GrayImage(Size(WIDTH, HEIGHT), 0);
	Mat ReversedImg(Size(WIDTH, HEIGHT), 0);
	Mat SubImg(Size(HEIGHT, HEIGHT), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			GrayImage.at<uchar>(h, w) = InputArray[h * WIDTH + w];
			ReversedImg.at<uchar>(h, w) = ReversedArray[h * WIDTH + w];
			SubImg.at<uchar>(h, w) = SubArray[h * WIDTH + w];
		}
	}

	imshow("GrayImage", GrayImage);
	imshow("ReversedImage", ReversedImg);
	imshow("SubtractImage", SubImg);

	delete[] InputArray;
	delete[] ReversedArray;
	delete[] SubArray;

	waitKey(0);
}
```

## Practice 2 Result

Practice 2의 출력 결과는 다음과 같습니다.

![](../.gitbook/assets/image%20%2862%29.png)

![](../.gitbook/assets/image%20%2866%29.png)

