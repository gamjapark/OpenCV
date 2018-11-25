#pragma once
#include <iostream>
#include <fstream>
using namespace std;

#define PI 3.14159

class ImageEncoding {
	enum ImageFormat {
		FORMAT_RGB,
		FORMAT_GRAY
	};

public:
	ImageEncoding(const char * filePath);
	~ImageEncoding();
	void OnCompression(const char * filePath);
	void WriteSOI(ofstream * file);
	void WriteDQT(ofstream * file);
	void WriteDHT(ofstream * file);
	void WriteSOF(ofstream * file, int Width, int Height);
	void WriteSOS(ofstream * file);
	void WriteEOI(ofstream * file);
	void Encode(ofstream * file, short * ptr, int num, int img_W, int * cnt);
	void WriteCode(ofstream * file, unsigned short code, int size, int * cnt);
	unsigned char CountBits(short V);
	void DCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo);

private:
	unsigned char **inputImg;
	int nHeight_in = 256, nWidth_in = 256;
	int depth;
};
