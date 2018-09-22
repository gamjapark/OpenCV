// Pixel Data�� �����ϱ�
//
// YUV(YCbCr)
// 1. FILE* ����ü�� ���� ���� ������ Bitstream ���·� ���� 
// 2. �迭�� ������ ���� �� �ε����� ���� ����
//
//
// OpenCV - Mat 
// 1. �޼��� ���� : Mat.at<_Tp>(int,int)
//				- template �޼ҵ�
//				- point, vec, uchar, int double �� ���·� ����� �ȼ� �ҷ���
// 2. ���� ���� : char * Mat.data
//				- Pixel Data�� ������ ����Ǿ��ִ� 1���� Array
//				- ������ ���� �ֻ���� pixel�� data���� ���ʴ�� ����� ����
//				- channel (RGB)�� ��� BGR ������ ������ �����
//			ex) gray ���󿡼� pixel data�� ���� 
//				- img(y, x)�� �ִ� ������ ����
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
			//��� ��� �簢�� �����
			//BlackImg.at<uchar>(h, w) = 255;

			//������ ��� �簢�� ����� 
			BackImg.at<uchar>(h, w) = 0;

		}
	}
	//circle(BlackImg, Point(127, 127),50, color, CV_FILLED);
	//�Ͼ�� �� �׸��� 

	//rectangle(BlackImg, Point(63, 63), Point(191, 192), color, CV_FILLED);
	//�Ͼ�� ���簢�� �׸���

	imshow("black", BackImg);

	waitKey(0);
}
