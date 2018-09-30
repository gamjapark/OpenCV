
#include "InterpolationRotation.h"

void BilinearInterpolation(double ScaleRate) {

	//2���� �޸𸮿� �Է� ���� �о���� 
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");

	//��� ���� h, w ũ��
	int nHeightResult = (int)HEIGHT * ScaleRate, nWidthResult = (int)WIDTH * ScaleRate;

	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(nHeightResult, nWidthResult, 0);

	//Interpolation!!!!!
	for (int h = 0; h < nHeightResult; h++)
		for (int w = 0; w < nWidthResult; w++)
			Output2D[h][w] = Interpolation(Input2D, HEIGHT, WIDTH, (double)(h / ScaleRate), (double)(w / ScaleRate));

	//��� ���� ����
	FileMake(Output2D, nHeightResult, nWidthResult, "lenaInterpolation.raw");

	//2���� �迭 �����Ҵ� ���� 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, nHeightResult);
}


unsigned char Interpolation(unsigned char** Input, int nHeight, int nWidth, double h_Cvt, double w_Cvt) {

	int H[2], W[2];
	H[0] = floor(h_Cvt);
	W[0] = floor(w_Cvt);

	double interval_w = w_Cvt - W[0], interval_h = h_Cvt - H[0];

	//���� �ڸ��� ����� �Ϳ� ���� ����ó�� -> ������ �ݺ�
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
