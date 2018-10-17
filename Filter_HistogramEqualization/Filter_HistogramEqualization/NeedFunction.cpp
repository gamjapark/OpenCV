
#include <iostream>
#include "NeedFunction.h"

using namespace std;

//������ �޸� �����Ҵ�
unsigned char** Alloc2D(int nHeight, int nWidth, unsigned char start)
{
	unsigned char** ArrAlloc = new unsigned char*[nHeight];
	for (int i = 0; i < nHeight; i++)
	{
		ArrAlloc[i] = new unsigned char[nWidth];
		memset(ArrAlloc[i], start, sizeof(unsigned char) * nWidth);
	}
	return ArrAlloc;
}

//������ �޸� ��������
void free2D(unsigned char** Array2D, int nHeight)
{
	for (int i = 0; i < nHeight; i++)
		delete[] Array2D[i];
	delete[] Array2D;
}

//2���� �迭�� �ִ� ���� 1���� �迭�� �ű��
unsigned char* Array2DtoArray1D(unsigned char ** Array2D, int nHeight, int nWidth) {
	unsigned char* Array1D = new unsigned char[nHeight * nWidth];
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Array1D[h * nWidth + w] = Array2D[h][w];
	return Array1D;
}

//1���� �迭�� �ִ� ���� 2���� �迭�� �ű��
unsigned char** Array1DtoArray2D(unsigned char * Array1D, int nHeight, int nWidth, unsigned char start) {
	unsigned char** Array2D = Alloc2D(nHeight, nWidth, start);
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Array2D[h][w] = Array1D[h * nWidth + w];
	delete[] Array1D;
	return Array2D;
}

//1���� �迭�� �Է¿��� �ҷ��ͼ� 2���� �迭�� ��ȯ
unsigned char** FileRead(int nHeight, int nWidth, const char *FileName) {
	//���� �Է� ���� �������迭�� �о����
	unsigned char * Input = new unsigned char[nHeight * nWidth];
	FILE* fp1;
	fopen_s(&fp1, FileName, "rb");
	fread(Input, sizeof(unsigned char), nHeight * nWidth, fp1);
	fclose(fp1);

	return  Array1DtoArray2D(Input, nHeight, nWidth, 0);
}


//2���� �迭�� ��� ������ 1���� �迭�� �Ű� ��� ��� ���� ����
void FileMake(unsigned char ** Output2D, int nHeight, int nWidth, const char *FileName) {
	//������ �迭�� ��� ��� ������ ������ �迭�� �Ű� ���
	unsigned char * Output = Array2DtoArray1D(Output2D, nHeight, nWidth);

	//���Ͽ� ��� ���� ����
	FILE* fp2;
	fopen_s(&fp2, FileName, "wb");
	fwrite(Output, sizeof(unsigned char), nHeight * nWidth, fp2);
	fclose(fp2);

	//������ �޸� �����Ҵ� ����
	delete[] Output;
}

//HW2���� �߰�
//������ Padding�ϴ� �Լ�
unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = Alloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Pad[h + nPadSize][w + nPadSize] = In[h][w];
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + (nHeight - 1)][w + nPadSize] = In[nHeight - 1][w];
		}
	}
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + (nWidth - 1)] = In[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + (nHeight - 1)][w] = In[nHeight - 1][0];
			Pad[h][w + (nWidth - 1)] = In[0][nWidth - 1];
			Pad[h + (nHeight - 1)][w + (nWidth - 1)] = In[nHeight - 1][nWidth - 1];
		}
	}
	return Pad;
}