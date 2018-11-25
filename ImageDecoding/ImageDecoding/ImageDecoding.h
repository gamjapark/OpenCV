#pragma once
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

using namespace std;

#define PI 3.14159

class ImageDecoding {
	enum ImageFormat {
		FORMAT_RGB,
		FORMAT_GRAY
	};

public:
	ImageDecoding(const char * filePath);
	~ImageDecoding();
	void OnDeCompression(const char * filePath, const char * writePath);
	void ReadSOI(ifstream * file);
	void ReadDQT(ifstream * file);
	void ReadDHT(ifstream * file);
	void ReadSOF(ifstream * file);
	void ReadSOS(ifstream * file);
	void ReadEOI(ifstream * file);
	void Decode(ifstream * file, short * ptr, int num, int img_W, int * cnt, unsigned char * code);
	void WriteFile(ofstream * file, unsigned char * out);
	void IDCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo);
	short getAbit(short x, int n);
	short setAbit(short x, int n, int b);
private:
	unsigned char **outputImg;
	int nHeight_in = 256, nWidth_in = 256;
	int depth;
};
