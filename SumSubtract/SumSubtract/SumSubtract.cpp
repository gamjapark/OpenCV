#include <opencv2\opencv.hpp>
using namespace cv;

#define HEIGHT 512
#define WIDTH 512

void main() {

	Mat img = imread("lena.jpg", 1);

	Mat GrayImg(Size(HEIGHT, WIDTH), 0);
	Mat DarkImg(Size(HEIGHT, WIDTH), 0);
	Mat BrightImg(Size(HEIGHT, WIDTH), 0);

	//Gray Scale Image 얻기
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Vec3b RGB = img.at<Vec3b>(h, w);
			GrayImg.at<uchar>(h,w) = RGB[2] * 0.299 + RGB[1] * 0.587 + RGB[0] * 0.114;
		}
	}

	imshow("GrayImage", GrayImg);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//픽셀값 0~255 범위 밖 예외처리 함 - 예외처리 하지 않았을 때 영상이 깨짐

			//이미지 픽셀 값에서 고정된 값 빼서 어두운 이미지 얻기
			if(GrayImg.at<uchar>(h, w) - 70 >= 0)
				DarkImg.at<uchar>(h, w) = GrayImg.at<uchar>(h, w) - 70;
			else DarkImg.at<uchar>(h, w) = 0;

			//이미지 픽셀 값에서 고정된 값 더해서 밝은 이미지 얻기
			if(GrayImg.at<uchar>(h, w) + 70 <= 255)
				BrightImg.at<uchar>(h,w) = GrayImg.at<uchar>(h, w) + 70;
			else BrightImg.at<uchar>(h, w) = 255;
		}
	}
	imshow("DarkImg", DarkImg);
	imshow("BrightImg", BrightImg);

	waitKey(0);
}