#include <opencv2\opencv.hpp>
using namespace cv;

#define HEIGHT 512
#define WIDTH 512

void main() {

	Mat img = imread("lena.jpg", 1);

	Mat GrayImg(Size(HEIGHT, WIDTH), 0);
	Mat DarkImg(Size(HEIGHT, WIDTH), 0);
	Mat BrightImg(Size(HEIGHT, WIDTH), 0);

	//Gray Scale Image ���
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Vec3b RGB = img.at<Vec3b>(h, w);
			GrayImg.at<uchar>(h,w) = RGB[2] * 0.299 + RGB[1] * 0.587 + RGB[0] * 0.114;
		}
	}

	imshow("GrayImage", GrayImg);

	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++) {
			//�ȼ��� 0~255 ���� �� ����ó�� �� - ����ó�� ���� �ʾ��� �� ������ ����

			//�̹��� �ȼ� ������ ������ �� ���� ��ο� �̹��� ���
			if(GrayImg.at<uchar>(h, w) - 70 >= 0)
				DarkImg.at<uchar>(h, w) = GrayImg.at<uchar>(h, w) - 70;
			else DarkImg.at<uchar>(h, w) = 0;

			//�̹��� �ȼ� ������ ������ �� ���ؼ� ���� �̹��� ���
			if(GrayImg.at<uchar>(h, w) + 70 <= 255)
				BrightImg.at<uchar>(h,w) = GrayImg.at<uchar>(h, w) + 70;
			else BrightImg.at<uchar>(h, w) = 255;
		}
	}
	imshow("DarkImg", DarkImg);
	imshow("BrightImg", BrightImg);

	waitKey(0);
}