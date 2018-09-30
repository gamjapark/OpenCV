#include <iostream>
#include "NeedFunction.h"

using namespace std;

//이차원 메모리 공간할당
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

//이차원 메모리 공간해제
void free2D(unsigned char** Array2D, int nHeight)
{
	for (int i = 0; i < nHeight; i++)
		delete[] Array2D[i];
	delete[] Array2D;
}

//2차원 배열에 있는 값을 1차원 배열로 옮기기
unsigned char* Array2DtoArray1D(unsigned char ** Array2D, int nHeight, int nWidth) {
	unsigned char* Array1D = new unsigned char[nHeight * nWidth];
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Array1D[h * nWidth + w] = Array2D[h][w];
	return Array1D;
}

//1차원 배열의 있는 값을 2차원 배열로 옮기기
unsigned char** Array1DtoArray2D(unsigned char * Array1D, int nHeight, int nWidth, unsigned char start) {
	unsigned char** Array2D = Alloc2D(nHeight, nWidth, start);
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Array2D[h][w] = Array1D[h * nWidth + w];
	delete[] Array1D;
	return Array2D;
}

//1차원 배열에 입력영상 불러와서 2차원 배열로 반환
unsigned char** FileRead(int nHeight, int nWidth, const char *FileName) {
	//먼저 입력 영상 일차원배열에 읽어오기
	unsigned char * Input = new unsigned char[nHeight * nWidth];
	FILE* fp1;
	fopen_s(&fp1, FileName, "rb");
	fread(Input, sizeof(unsigned char), nHeight * nWidth, fp1);
	fclose(fp1);

	return  Array1DtoArray2D(Input, nHeight, nWidth, 0);
}


//2차원 배열에 담김 영상을 1차원 배열로 옮겨 담아 출력 영상 생성
void FileMake(unsigned char ** Output2D, int nHeight, int nWidth, const char *FileName) {
	//이차원 배열에 담긴 결과 영상을 일차원 배열에 옮겨 담기
	unsigned char * Output = Array2DtoArray1D(Output2D, nHeight, nWidth);

	//파일에 결과 영상 저장
	FILE* fp2;
	fopen_s(&fp2, FileName, "wb");
	fwrite(Output, sizeof(unsigned char), nHeight * nWidth, fp2);
	fclose(fp2);

	//일차원 메모리 동적할당 해제
	delete[] Output;
}