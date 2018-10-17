#pragma once

#define HEIGHT 256
#define WIDTH 256

//��� ������ ���� �ʿ��� �Լ���
unsigned char** Alloc2D(int nHeight, int nWidth, unsigned char start);
void free2D(unsigned char** Array2D, int nHeight);
unsigned char* Array2DtoArray1D(unsigned char ** Array2D, int nHeight, int nWidth);
unsigned char** Array1DtoArray2D(unsigned char * Array1D, int nHeight, int nWidth, unsigned char start);
unsigned char** FileRead(int nHeight, int nWidth, const char * FileName);
void FileMake(unsigned char ** Output2D, int nHeight, int nWidth, const char *FileName);
//+ Padding�Լ�
unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize);