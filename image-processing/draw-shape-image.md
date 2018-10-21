# Draw Shape Image

## Practice Using OpenCV

이는 검은색 영상에 하얀색 정사각형과 하얀색 원을 그려보는 예제입니다.

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

#define WIDTH 256
#define HEIGHT 256

void main(){
	Mat RecImage(Size(WIDTH, HEIGHT), 0);
	Mat CirImage(Size(WIDTH, HEIGHT), 0);

	//두 이미지에 검은색 배경 만들기
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			RecImage.at<uchar>(h, w) = 0;
			CirImage.at<uchar>(h, w) = 0;
		}
	}
	rectangle(RecImage, Point(63, 63), Point(191, 192),
		 	Scalar(255, 255, 255), CV_FILLED);
	circle(CirImage, Point(127, 127), 50, 
			Scalar(255, 255, 255), CV_FILLED);

	imshow("RecImage", RecImage);
	imshow("CirImage", CirImage);
	waitKey(0);
}
```

위의 코드의 출력 영상은 다음과 같습니다.

![](../.gitbook/assets/image%20%2833%29.png)

## Practice Using C++

이는 검은색 영상에 하얀색 정사각형과 하얀색 원, 하얀색 삼각형을 그려보는 예제입니다.

```cpp
//결과영상 출력을 위해서만 opencv를 사용하였습니다.
#include <opencv2/opencv.hpp>
using namespace cv;

#include <iostream>
using namespace std;

#define WIDTH 256
#define HEIGHT 256

void main() {
	//입력, 출력 영상을 위한 1차원 배열 동적 할당
	unsigned char * RecArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char * CirArray = new unsigned char[HEIGHT * WIDTH];
	unsigned char * TriArray = new unsigned char[HEIGHT * WIDTH];
	
	//검정색 배경
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			RecArray[h * WIDTH + w] = 0;
			CirArray[h * WIDTH + w] = 0;
			TriArray[h * WIDTH + w] = 0;
		}
	}
	int r = 64; //원의 반지름 : 64
	for (int h = 0; h < 2 * r; h++) {
		for (int w = 0; w < 2 * r; w++) {
			RecArray[(h + 63) * WIDTH + (w + 63)] = 255;
			//원의 방정식 이
			if (r * r > (h + 0.5 - r) * (h + 0.5 - r) + (w + 0.5 - r) * (w + 0.5 - r)) 
				CirArray[(h + 63) * WIDTH + (w + 63)] = 255;
		}
	}
	//삼각형 그리기
	for (int h = 0; h < 90; h++) {
		for (int w = 0; w < h + 1; w++) {
			TriArray[(h + 64) * WIDTH + (w + 128)] = 255;
		}
		for (int w = -1 ; w > -(h + 1); w--) {
			TriArray[(h + 64) * WIDTH + (w + 128)] = 255;
		}
	}

	//결과 확인을 위해 opencv를 사용
	Mat RecImage(Size(256, 256), 0);
	Mat CirImage(Size(256, 256), 0);
	Mat TriImage(Size(256, 256), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			RecImage.at<uchar>(h, w) = RecArray[h * WIDTH + w];
			CirImage.at<uchar>(h, w) = CirArray[h * WIDTH + w];
			TriImage.at<uchar>(h, w) = TriArray[h * WIDTH + w];
		}
	}

	//1차원배열 동적할당 해제
	delete[] RecArray;
	delete[] CirArray;
	delete[] TriArray;

	imshow("RecImage", RecImage);
	imshow("CirImage", CirImage);
	imshow("TriImage", TriImage);
	waitKey(0);
}
```

위의 코드의 출력 결과는 다음과 같습니다.

![](../.gitbook/assets/image%20%281%29.png)

