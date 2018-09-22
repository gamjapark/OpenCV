#include <opencv2\opencv.hpp>
using namespace cv;

#define HEIGHT 512
#define WIDTH 512
void main() {
	Mat img = imread("lena.jpg", 1);
	//이미지를 Color로 읽고, 투명한 부분은 무시(Default)

	Mat imgGray(Size(HEIGHT, WIDTH), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Vec3b RGB = img.at<Vec3b>(h, w);
			// * Vec3b
			//	 = 컬러 영상 Mat는 세개의 8비트(byte) 값의 unsigned char벡터 반환함
			// * RGB[0] = B / RGB[1] = G / RGB[2] = R

			imgGray.at<uchar>(h, w) = RGB[2] * 0.299 + RGB[1] * 0.587 + RGB[0] * 0.114;
			// => RGB에서 Y값 계산
			// * RGB -> YUV 변환 
			//	1. Y = 0.299R + 0.587G + 0.114B
			//	2. U = 0.564(B - Y) = -0.169R - 0.331G + 0.499B
			//	3. V = 0.713(R - Y) = 0.499R - 0.418G - 0.081B
		}
	}
	imshow("Input Image", img);
	imshow("Gray Scale Image", imgGray);
	waitKey(0);
}

