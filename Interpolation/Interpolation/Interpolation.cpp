#include <opencv2\opencv.hpp>

using namespace cv;

void main() {

	Mat In_Img, Nearest, Bilinear, Bicubic;

	int nHeight = 256, nWidth = 256;
	double dScaleRate = 1.5;

	unsigned char * In = new unsigned char[nHeight * nWidth];

	FILE* fp;
	fopen_s(&fp, "lena256.raw", "rb");
	fread(In, sizeof(unsigned char), nHeight*nWidth, fp);

	In_Img = Mat(nHeight, nWidth, CV_8UC1, In);

	int nHeight_Scale = nHeight * dScaleRate;
	int nWidth_Scale = nWidth * dScaleRate;;

	//Nearest-Neighbor interpolation
	resize(In_Img, Nearest, Size(nWidth_Scale, nHeight_Scale), 0.0, 0.0, CV_INTER_NN);
	//Bilinear interpolation
	resize(In_Img, Bilinear, Size(nWidth_Scale, nHeight_Scale), 0.0, 0.0, CV_INTER_LINEAR);
	//Bicubic interpolation
	resize(In_Img, Bicubic, Size(nWidth_Scale, nHeight_Scale), 0.0, 0.0, CV_INTER_CUBIC);

	imshow("Original", In_Img);
	imshow("Nearest-Neighbor", Nearest);
	imshow("Bilinear", Bilinear);
	imshow("Bicubic", Bicubic);

	waitKey(0);

	delete[] In;
	fclose(fp);

}