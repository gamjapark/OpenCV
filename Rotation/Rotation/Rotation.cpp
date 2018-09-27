#include <opencv2\opencv.hpp>
using namespace cv;
void main() {
	Mat In_Img, Nearest, Bilinear, Bicubic;

	int nHeight = 256, nWidth = 256;
	double dRotationAngle = 90.0;

	unsigned char * In = new unsigned char[nHeight * nWidth];

	FILE* fp;
	fopen_s(&fp, "lena256.raw", "rb");
	fread(In, sizeof(unsigned char), nHeight*nWidth, fp);

	In_Img = Mat(nHeight, nWidth, CV_8UC1, In);

	Mat RotateMat = getRotationMatrix2D(Point2f(nHeight / 2, nWidth / 2), dRotationAngle, 1);

	//Nearest-Neighbor 
	warpAffine(In_Img, Nearest, RotateMat, Size(nHeight, nWidth), CV_INTER_NN, BORDER_CONSTANT, Scalar(0));
	//Bilinear
	warpAffine(In_Img, Bilinear, RotateMat, Size(nHeight, nWidth), CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0));
	//Bicubic
	warpAffine(In_Img, Bicubic, RotateMat, Size(nHeight, nWidth), CV_INTER_CUBIC, BORDER_CONSTANT, Scalar(0));


	imshow("Original", In_Img);
	imshow("Nearest-Neighbor", Nearest);
	imshow("Bilinear", Bilinear);
	imshow("Bicubic", Bicubic);

	waitKey(0);


	delete[] In;
	fclose(fp);
}