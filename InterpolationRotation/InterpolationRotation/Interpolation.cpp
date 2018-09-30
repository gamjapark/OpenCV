
#include "InterpolationRotation.h"

void BilinearInterpolation(double ScaleRate) {

	//2차원 메모리에 입력 영상 읽어오기 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//결과 영상 h, w 크기
	int nHeightResult = (int)HEIGHT * ScaleRate, nWidthResult = (int)WIDTH * ScaleRate;

	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(nHeightResult, nWidthResult, 0);

	//Interpolation!!!!!
	for (int h = 0; h < nHeightResult; h++)
		for (int w = 0; w < nWidthResult; w++)
			Output2D[h][w] = Interpolation(Input2D, HEIGHT, WIDTH, (double)(h / ScaleRate), (double)(w / ScaleRate));

	//출력 영상 생성
	FileMake(Output2D, nHeightResult, nWidthResult, "lenaInterpolation.raw");

	//2차원 배열 동적할당 해제 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, nHeightResult);
}


unsigned char Interpolation(unsigned char** Input, int nHeight, int nWidth, double h_Cvt, double w_Cvt) {

	int H[2], W[2];
	H[0] = floor(h_Cvt);
	W[0] = floor(w_Cvt);

	double interval_w = w_Cvt - W[0], interval_h = h_Cvt - H[0];

	//가장 자리를 벗어나는 것에 대한 예외처리 -> 인접값 반복
	if (H[0] == nHeight - 1 && W[0] == nWidth - 1) {
		H[1] = nHeight - 1;
		W[1] = nWidth - 1;
	}
	else if (H[0] == nHeight - 1 && W[0] != nWidth - 1) {
		H[1] = nHeight - 1;
		W[1] = W[0] + 1;
	}
	else if (H[0] != nHeight - 1 && W[0] == nWidth - 1) {
		H[1] = H[0] + 1;
		W[1] = nWidth - 1;
	}
	else {
		H[1] = H[0] + 1;
		W[1] = W[0] + 1;
	}

	unsigned char result[2];

	result[0] = ((Input[H[0]][W[0]] * (1 - interval_w)) + (Input[H[0]][W[1]] * interval_w));
	result[1] = ((Input[H[1]][W[0]] * (1 - interval_w)) + (Input[H[1]][W[1]] * interval_w));
	return (unsigned char)((result[0] * (1 - interval_h)) + (result[1] * interval_h));
}
