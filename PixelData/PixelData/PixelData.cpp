// Pixel Data에 접근하기
//
// YUV(YCbCr)
// 1. FILE* 구조체를 통해 영상 파일을 Bitstream 형태로 읽음 
// 2. 배열에 데이터 저장 후 인덱스를 통해 접근
//
//
// OpenCV - Mat 
// 1. 메서드 접근 : Mat.at<_Tp>(int,int)
//				- template 메소드
//				- point, vec, uchar, int double 등 형태로 저장된 픽셀 불러옴
// 2. 직접 접근 : char * Mat.data
//				- Pixel Data가 실제로 저장되어있는 1차원 Array
//				- 영상의 왼쪽 최상단의 pixel의 data부터 차례대로 저장되 있음
//				- channel (RGB)의 경우 BGR 순서로 데이터 저장됨
//			ex) gray 영상에서 pixel data에 접근 
//				- img(y, x)에 있는 데이터 접근
//				- img.data( y * width + x)



#include <opencv2\opencv.hpp>

#define HEIGHT 256
#define WIDTH 256

using namespace cv;


void main() {
	Scalar color(255, 255, 255);
	Mat BackImg(Size(HEIGHT, WIDTH), 0);
	
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//흰색 배경 사각형 만들기
			//BlackImg.at<uchar>(h, w) = 255;

			//검은색 배경 사각형 만들기 
			BackImg.at<uchar>(h, w) = 0;

		}
	}
	//circle(BlackImg, Point(127, 127),50, color, CV_FILLED);
	//하얀색 원 그리기 

	//rectangle(BlackImg, Point(63, 63), Point(191, 192), color, CV_FILLED);
	//하얀색 정사각형 그리기

	imshow("black", BackImg);

	waitKey(0);
}
