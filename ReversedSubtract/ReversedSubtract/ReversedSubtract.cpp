#include <opencv2\opencv.hpp>

using namespace cv;

#define HEIGHT 512
#define WIDTH 512

void main() {
	Mat Img = imread("lena.jpg", 1);

	Mat GrayImage(Size(HEIGHT, WIDTH), 0);
	Mat ReversedImg(Size(HEIGHT, WIDTH), 0);
	Mat SubImg(Size(HEIGHT, WIDTH), 0);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//Gray Scale Image 얻기
			Vec3b RGB = Img.at<Vec3b>(h, w);
			GrayImage.at<uchar>(h, w) = RGB[2] * 0.299 + RGB[1] * 0.587 + RGB[0] * 0.114;
			//Reversed Image 얻기
			ReversedImg.at<uchar>(h, w) = 255 - GrayImage.at<uchar>(h, w);

			//Gray Scale Image에서 Reversed Image 픽셀값을 뺀 Image 얻기
			if (GrayImage.at<uchar>(h, w) - ReversedImg.at<uchar>(h, w) < 0) {
				SubImg.at<uchar>(h, w) = 0;
			}
			else {
				SubImg.at<uchar>(h, w) = GrayImage.at<uchar>(h, w) - ReversedImg.at<uchar>(h, w);
			}
		}
	}

	imshow("GrayImage", GrayImage);
	imshow("ReversedImage", ReversedImg);
	imshow("SubtractImage", SubImg);

	waitKey(0);
}
