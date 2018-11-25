
#include "ImageEncoding.h"

ImageEncoding::ImageEncoding(const char * filePath) {

	if (strcmp(strchr(filePath, '.'), ".JPG") != 0 && strcmp(strchr(filePath, '.'), ".jpg") != 0 &&
		strcmp(strchr(filePath, '.'), ".raw") != 0 && strcmp(strchr(filePath, '.'), ".RAW") != 0 ) {
		//파일 확장자 확인
		cout << "JPG 또는 Gray 파일이 아닙니다." << endl;
	}

	if (strcmp(strchr(filePath, '.'), ".JPG") == 0 || strcmp(strchr(filePath, '.'), ".jpg") == 0) {
			//jpg image
		depth = 3;
	}
	else {	//gray image
		depth = 1;
	}

	inputImg = new unsigned char*[nHeight_in];
	for (int i = 0; i < nHeight_in; i++) {
		inputImg[i] = new unsigned char[nWidth_in * depth];
	}

	ifstream file;
	file.open(filePath, ios::binary | ios::in);

	for (int i = 0; i < nHeight_in; i++) {
		file.read((char *)inputImg[i], sizeof(unsigned char) * nWidth_in * depth);
	}

	file.close();
}
ImageEncoding::~ImageEncoding() {
	if (inputImg != NULL) {
		for (int i = 0; i < nHeight_in; i++) {
			delete[] inputImg[i];
		}
		delete[] inputImg;
	}
}
void ImageEncoding::OnCompression(const char * filePath) {

	ofstream file;
	file.open(filePath, ios::out | ios::binary);	//write로 파일 open
	unsigned char *img;
	short *Y = NULL, *Cb = NULL, *Cr = NULL;
	
	if (strcmp(strchr(filePath, '.'), ".JPG") == 0 || strcmp(strchr(filePath, '.'), ".jpg") == 0) {

		int i, j;
		int img_W;
		int img_H;

		WriteSOI(&file);
		WriteDQT(&file);
		WriteDHT(&file);
		WriteSOF(&file, nWidth_in, nHeight_in);
		WriteSOS(&file);

		img_W = ((nWidth_in - 1) / 8 + 1) * 8;
		img_H = ((nHeight_in - 1) / 8 + 1) * 8;

		img = new unsigned char[img_W *  img_H * 3];
		memset(img, 0, img_W * img_H * 3);

		for (int y = 0; y < nHeight_in; y++)
			if (depth == 3)
				memcpy(&img[y*(img_W * 3)], inputImg[y], nWidth_in * depth);
			else
				memcpy(&img[y*(img_W)], inputImg[y], nWidth_in);

		if (depth == 3) {
			float R, G, B;
			float y_val, cb, cr;

			for (i = 0; i < nHeight_in; i++)
			{
				for (j = 0; j < nWidth_in; j++)
				{
					R = (float)img[(i*img_W + j) * 3];
					G = (float)img[(i*img_W + j) * 3 + 1];
					B = (float)img[(i*img_W + j) * 3 + 2];

					y_val = 0.299f * R + 0.587f * G + 0.114f * B;
					cb = -0.1687f * R - 0.3313f * G + 0.5f * B + 128;
					cr = 0.5f*R - 0.4187f*G - 0.0813f*B + 128;

					if (y_val > 255.0) y_val = 255.0; if (y_val < 0.0) y_val = 0.0;
					if (cb > 255.0) cb = 255.0; if (cb < 0.0) cb = 0.0;
					if (cr > 255.0) cr = 255.0; if (cr < 0.0) cr = 0.0;

					img[(i*img_W + j) * 3] = (unsigned char)y_val;
					img[(i*img_W + j) * 3 + 1] = (unsigned char)cb;
					img[(i*img_W + j) * 3 + 2] = (unsigned char)cr;
				}
			}

			Y = new short[img_W * img_H];
			memset(Y, 0, sizeof(short) * img_W * img_H);
			Cb = new short[img_W * img_H];
			memset(Cb, 0, sizeof(short) * img_W * img_H);
			Cr = new short[img_W * img_H];
			memset(Cr, 0, sizeof(short) * img_W * img_H);

			int idx1, idx2;

			for (i = 0; i < nHeight_in; i++)
			{
				for (j = 0; j < nWidth_in; j++)
				{
					idx1 = i * img_W + j;
					idx2 = i * (img_W * 3) + j * 3;
					Y[idx1] = img[idx2] - 128;
					Cb[idx1] = img[idx2 + 1] - 128;
					Cr[idx1] = img[idx2 + 2] - 128;
				}
			}

			for (i = 0; i < img_H; i = i + 8)
			{
				for (j = 0; j < img_W; j = j + 8)
				{
					DCT_Zigzag_Quantization(&Y[i*img_W + j], img_W, 0);
					DCT_Zigzag_Quantization(&Cb[i*img_W + j], img_W, 1);
					DCT_Zigzag_Quantization(&Cr[i*img_W + j], img_W, 1);
				}
			}

			int nBlocks_X = img_W / 8;
			int nBlocks_Y = img_H / 8;
			int idx = 0;

			short *DC1 = new short[nBlocks_X*nBlocks_Y];
			short *DC2 = new short[nBlocks_X*nBlocks_Y];
			short *DC3 = new short[nBlocks_X*nBlocks_Y];

			for (i = 0; i < img_H; i = i + 8)
			{
				for (j = 0; j < img_W; j = j + 8)
				{
					DC1[idx] = Y[i*img_W + j];
					DC2[idx] = Cb[i*img_W + j];
					DC3[idx] = Cr[i*img_W + j];
					idx++;
				}
			}

			for (i = nBlocks_X * nBlocks_Y - 1; i > 0; i--)
			{
				DC1[i] = DC1[i] - DC1[i - 1];
				DC2[i] = DC2[i] - DC2[i - 1];
				DC3[i] = DC3[i] - DC3[i - 1];
			}

			idx = 0;
			for (i = 0; i < img_H; i = i + 8)
			{
				for (j = 0; j < img_W; j = j + 8)
				{
					Y[i*img_W + j] = DC1[idx];
					Cb[i*img_W + j] = DC2[idx];
					Cr[i*img_W + j] = DC3[idx];
					idx++;
				}
			}
		}
		else {
			int i;
			Y = new short[img_W * img_H];
			memset(Y, 0, sizeof(short) * img_W * img_H);

			for (i = 0; i < img_W * img_H; i++)
				Y[i] = img[i] - 128;

			for (i = 0; i < img_H; i = i + 8)
				for (j = 0; j < img_W; j = j + 8)
					DCT_Zigzag_Quantization(&Y[i*img_W + j], img_W, 0);

			int nBlocks_X = img_W / 8;
			int nBlocks_Y = img_H / 8;
			int idx = 0;

			short *DC = new short[nBlocks_X*nBlocks_Y];

			for (i = 0; i < img_H; i = i + 8)
				for (j = 0; j < img_W; j = j + 8)
					DC[idx++] = Y[i*img_W + j];

			for (i = nBlocks_X * nBlocks_Y - 1; i > 0; i--)
				DC[i] = DC[i] - DC[i - 1];

			idx = 0;
			for (i = 0; i < img_H; i = i + 8)
				for (j = 0; j < img_W; j = j + 8)
					Y[i*img_W + j] = DC[idx++];

		}

		int cnt = 0;

		for (i = 0; i < img_H; i = i + 8)
		{
			for (j = 0; j < img_W; j = j + 8)
			{
				Encode(&file, &Y[i*img_W + j], 0, img_W, &cnt);
				if (depth == 3) {
					Encode(&file, &Cb[i*img_W + j], 1, img_W, &cnt);
					Encode(&file, &Cr[i*img_W + j], 1, img_W, &cnt);
				}
			}
		}

		if (cnt != 0)
			WriteCode(&file, 0, 8 - cnt, &cnt);

		WriteEOI(&file);

	}
	file.close();	
}
void ImageEncoding::WriteSOI(ofstream * file) {
	unsigned char buf[2] = { 0xFF, 0xD8 };
	file->write((char *)buf, 2);
}
void ImageEncoding::WriteDQT(ofstream * file) {
	unsigned char marker[2] = { 0xFF, 0xDB };
	short int tableSize;
	unsigned char c;

	unsigned char QT0[64] = { 16, 11, 12, 14, 12, 10, 16, 14,
					13, 14, 18, 17, 16, 19, 24, 40,
					26, 24, 22, 22, 24, 49, 35, 37,
					29, 40, 58, 51, 61, 60, 57, 51,
					56, 55, 64, 72, 92, 78, 64, 68,
					87, 69, 55, 56, 80, 109, 81, 87,
					95, 98, 103, 104, 103, 62, 77, 113,
					121, 112, 100, 120, 92, 101, 103, 99 };

	unsigned char QT1[64] = { 17, 18, 18, 24, 21, 24, 47, 26,
					26, 47, 99, 66, 56, 66, 99, 99,
					99 ,99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99 };

	file->write((char *)marker, 2);

	if (depth == 3) tableSize = 132;
	else tableSize = 67;

	//16비트 정수를 파일에 저장시 상위, 하위 8비트 값이 바뀌어 저장되기 때문에 바꿔저장
	//tableSize = (tableSize << 8) | (tableSize >> 8);
	//short형 -> char* 접근 write
	file->write((((char *)(&tableSize)) + 1), 1);
	file->write((((char *)(&tableSize)) + 0), 1);

	c = 0;
	file->write((char *)&c, 1);
	file->write((char *)QT0, 64);

	if (depth == 3) {
		c = 1;
		file->write((char *)&c, 1);
		file->write((char *)QT1, 64);
	}
}
void ImageEncoding::WriteDHT(ofstream * file) {
	unsigned char marker[2] = { 0xFF, 0xC4 };
	short int tableSize;
	unsigned char c;

	unsigned char HT_code_0[] = { 0, 1, 5, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char HT_symbol_0[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	unsigned char HT_code_16[] = { 0, 2, 1, 3, 3, 2, 4, 3, 5, 5, 4, 4, 0, 0, 1, 125 };

	unsigned char HT_symbol_16[] = { 1, 2, 3, 0, 4, 17, 5, 18, 33, 49,
						65, 6, 19, 81, 97, 7, 34, 113, 20, 50,
						129, 145, 161, 8, 35, 66, 177, 193, 21,
						82, 209, 240, 36, 51, 98, 114, 130, 9,
						10, 22, 23, 24, 25, 26, 37, 38, 39, 40,
						41, 42, 52, 53, 54, 55, 56, 57, 58, 67,
						68, 69, 70, 71, 72, 73, 74, 83, 84, 85,
						86, 87, 88, 89, 90, 99, 100, 101, 102,
						103, 104, 105, 106, 115, 116, 117, 118,
						119, 120, 121, 122, 131, 132, 133, 134,
						135, 136, 137, 138, 146, 147, 148, 149,
						150, 151, 152, 153, 154, 162, 163, 164,
						165, 166, 167, 168, 169, 170, 178, 179,
						180, 181, 182, 183, 184, 185, 186, 194,
						195, 196, 197, 198, 199, 200, 201, 202,
						210, 211, 212, 213, 214, 215, 216, 217,
						218, 225, 226, 227, 228, 229, 230, 231,
						232, 233, 234, 241, 242, 243, 244, 245,
						246, 247, 248, 249, 250 };

	unsigned char HT_code_1[] = { 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0 };

	unsigned char HT_symbol_1[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

	unsigned char HT_code_17[] = { 0, 2, 1, 2, 4, 4, 3, 4, 7, 5, 4, 4, 0, 1, 2, 119 };

	unsigned char HT_symbol_17[] = { 0, 1, 2, 3, 17, 4, 5, 33, 49,
						6, 18, 65, 81, 7, 97, 113, 19, 34, 50,
						129, 8, 20, 66, 145, 161, 177, 193, 9,
						35, 51, 82, 240, 21, 98, 114, 209, 10,
						22, 36, 52, 225, 37, 241, 23, 24, 25, 26,
						38, 39, 40, 41, 42, 53, 54, 55, 56, 57,
						58, 67, 68, 69, 70, 71, 72, 73, 74, 83,
						84, 85, 86, 87, 88, 89, 90, 99, 100, 101,
						102, 103, 104, 105, 106, 115, 116, 117,
						118, 119, 120, 121, 122, 130, 131, 132,
						133, 134, 135, 136, 137, 138, 146, 147,
						148, 149, 150, 151, 152, 153, 154, 162,
						163, 164, 165, 166, 167, 168, 169, 170,
						178, 179, 180, 181, 182, 183, 184, 185,
						186, 194, 195, 196, 197, 198, 199, 200,
						201, 202, 210, 211, 212, 213, 214, 215,
						216, 217, 218, 226, 227, 228, 229, 230,
						231, 232, 233, 234, 242, 243, 244, 245,
						246, 247, 248, 249, 250 };

	file->write((char *)marker, 2);

	if (depth == 3) tableSize = 418;
	else tableSize = 210;

	//16비트 정수를 파일에 저장시 상위, 하위 8비트 값이 바뀌어 저장되기 때문에 바꿔저장
	//tableSize = (tableSize << 8) | (tableSize >> 8);
	//short형 -> char* 접근 write
	file->write((((char *)(&tableSize)) + 1), 1);
	file->write((((char *)(&tableSize)) + 0), 1);

	c = 0;
	file->write((char *)&c, 1);
	file->write((char *)HT_code_0, 16);
	file->write((char *)HT_symbol_0, 12);

	c = 16;
	file->write((char *)&c, 1);
	file->write((char *)HT_code_16, 16);
	file->write((char *)HT_symbol_16, 162);

	if (depth == 3) {
		c = 1;
		file->write((char *)&c, 1);
		file->write((char *)HT_code_1, 16);
		file->write((char *)HT_symbol_1, 12);

		c = 17;
		file->write((char *)&c, 1);
		file->write((char *)HT_code_17, 16);
		file->write((char *)HT_symbol_17, 162);
	}

}
void ImageEncoding::WriteSOF(ofstream * file, int Width, int Height) {
	unsigned short headerSize, w;
	unsigned char c;
	int i;
	unsigned char marker[2] = { 0xFF, 0xC0 };

	file->write((char *)marker, 2);

	headerSize = 8 + depth * 3;
	//headerSize = (headerSize << 8) | (headerSize >> 8);
	//short형 -> char* 접근 write
	file->write((((char *)(&headerSize)) + 1), 1);
	file->write((((char *)(&headerSize)) + 0), 1);

	c = 8;
	file->write((char *)&c, 1);

	w = (unsigned short)Height;
	w = (w << 8) | (w >> 8);
	file->write((((char *)(&w)) + 0), 1);
	file->write((((char *)(&w)) + 1), 1);

	w = (unsigned short)Width;
	w = (w << 8) | (w >> 8);
	file->write((((char *)(&w)) + 0), 1);
	file->write((((char *)(&w)) + 1), 1);

	c = depth;
	file->write((char *)&c, 1);

	for (i = 1; i <= depth; i++)
	{
		c = (unsigned char)i;
		file->write((char *)&c, 1);

		c = 0x11;
		file->write((char *)&c, 1);

		if (i == 1) c = 0;
		else c = 1;
		file->write((char *)&c, 1);
	}
}
void ImageEncoding::WriteSOS(ofstream * file) {
	unsigned short headerSize;
	unsigned char c;
	unsigned char marker[2] = { 0xFF, 0xDA };
	int i;

	file->write((char *)marker, 2);

	headerSize = 6 + depth * 2;
	//headerSize = (headerSize << 8) | (headerSize >> 8);
	//short형 -> char* 접근 write
	file->write((((char *)(&headerSize)) + 1), 1);
	file->write((((char *)(&headerSize)) + 0), 1);

	c = depth;
	file->write((char *)&c, 1);

	for (i = 1; i <= depth; i++)
	{
		c = (unsigned char)i;
		file->write((char *)&c, 1);

		if (i == 1)
			c = 0;
		else
			c = 0x11;
		file->write((char *)&c, 1);
	}

	c = 0;
	file->write((char *)&c, 1);
	c = 0x3f;
	file->write((char *)&c, 1);
	c = 0;
	file->write((char *)&c, 1);

}
void ImageEncoding::WriteEOI(ofstream * file) {
	unsigned char marker[2] = { 0xFF, 0xD9 };
	file->write((char *)marker, 2);
}
void ImageEncoding::Encode(ofstream * file, short * ptr, int num, int img_W, int * cnt) {
	unsigned char symbol_1;
	unsigned char size;
	short data[64], symbol_2;
	int idx = 0;
	int i, j;
	unsigned char k = 0, run_length = 0;

	unsigned short int DC_HUFFCODE[2][13] = {
		{0x0, 0x2, 0x3, 0x4, 0x5, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0},
		{0x0, 0x1, 0x2, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0x3fe, 0x7fe, 0}
	};

	unsigned short int AC_HUFFCODE[2][251] = {
		{0xa,0x0,0x1,0x4,0xb,0x1a,0x78,0xf8,0x3f6,0xff82,0xff83,0,0,0,0,0,
		 0,0xc,0x1b,0x79,0x1f6,0x7f6,0xff84,0xff85,0xff86,0xff87,0xff88,0,0,0,0,0,
		 0,0x1c,0xf9,0x3f7,0xff4,0xff89,0xff8a,0xff8b,0xff8c,0xff8d,0xff8e,0,0,0,0,0,
		 0,0x3a,0x1f7,0xff5,0xff8f,0xff90,0xff91,0xff92,0xff93,0xff94,0xff95,0,0,0,0,0,
		 0,0x3b,0x3f8,0xff96,0xff97,0xff98,0xff99,0xff9a,0xff9b,0xff9c,0xff9d,0,0,0,0,0,
		 0,0x7a,0x7f7,0xff9e,0xff9f,0xffa0,0xffa1,0xffa2,0xffa3,0xffa4,0xffa5,0,0,0,0,0,
		 0,0x7b,0xff6,0xffa6,0xffa7,0xffa8,0xffa9,0xffaa,0xffab,0xffac,0xffad,0,0,0,0,0,
		 0,0xfa,0xff7,0xffae,0xffaf,0xffb0,0xffb1,0xffb2,0xffb3,0xffb4,0xffb5,0,0,0,0,0,
		 0,0x1f8,0x7fc0,0xffb6,0xffb7,0xffb8,0xffb9,0xffba,0xffbb,0xffbc,0xffbd,0,0,0,0,0,
		 0,0x1f9,0xffbe,0xffbf,0xffc0,0xffc1,0xffc2,0xffc3,0xffc4,0xffc5,0xffc6,0,0,0,0,0,
		 0,0x1fa,0xffc7,0xffc8,0xffc9,0xffca,0xffcb,0xffcc,0xffcd,0xffce,0xffcf,0,0,0,0,0,
		 0,0x3f9,0xffd0,0xffd1,0xffd2,0xffd3,0xffd4,0xffd5,0xffd6,0xffd7,0xffd8,0,0,0,0,0,
		 0,0x3fa,0xffd9,0xffda,0xffdb,0xffdc,0xffdd,0xffde,0xffdf,0xffe0,0xffe1,0,0,0,0,0,
		 0,0x7f8,0xffe2,0xffe3,0xffe4,0xffe5,0xffe6,0xffe7,0xffe8,0xffe9,0xffea,0,0,0,0,0,
		 0,0xffeb,0xffec,0xffed,0xffee,0xffef,0xfff0,0xfff1,0xfff2,0xfff3,0xfff4,0,0,0,0,0,
		 0x7f9,0xfff5,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe},
		{0x0,0x1,0x4,0xa,0x18,0x19,0x38,0x78,0x1f4,0x3f6,0xff4,0,0,0,0,0,
		 0,0xb,0x39,0xf6,0x1f5,0x7f6,0xff5,0xff88,0xff89,0xff8a,0xff8b,0,0,0,0,0,
		 0,0x1a,0xf7,0x3f7,0xff6,0x7fc2,0xff8c,0xff8d,0xff8e,0xff8f,0xff90,0,0,0,0,0,
		 0,0x1b,0xf8,0x3f8,0xff7,0xff91,0xff92,0xff93,0xff94,0xff95,0xff96,0,0,0,0,0,
		 0,0x3a,0x1f6,0xff97,0xff98,0xff99,0xff9a,0xff9b,0xff9c,0xff9d,0xff9e,0,0,0,0,0,
		 0,0x3b,0x3f9,0xff9f,0xffa0,0xffa1,0xffa2,0xffa3,0xffa4,0xffa5,0xffa6,0,0,0,0,0,
		 0,0x79,0x7f7,0xffa7,0xffa8,0xffa9,0xffaa,0xffab,0xffac,0xffad,0xffae,0,0,0,0,0,
		 0,0x7a,0x7f8,0xffaf,0xffb0,0xffb1,0xffb2,0xffb3,0xffb4,0xffb5,0xffb6,0,0,0,0,0,
		 0,0xf9,0xffb7,0xffb8,0xffb9,0xffba,0xffbb,0xffbc,0xffbd,0xffbe,0xffbf,0,0,0,0,0,
		 0,0x1f7,0xffc0,0xffc1,0xffc2,0xffc3,0xffc4,0xffc5,0xffc6,0xffc7,0xffc8,0,0,0,0,0,
		 0,0x1f8,0xffc9,0xffca,0xffcb,0xffcc,0xffcd,0xffce,0xffcf,0xffd0,0xffd1,0,0,0,0,0,
		 0,0x1f9,0xffd2,0xffd3,0xffd4,0xffd5,0xffd6,0xffd7,0xffd8,0xffd9,0xffda,0,0,0,0,0,
		 0,0x1fa,0xffdb,0xffdc,0xffdd,0xffde,0xffdf,0xffe0,0xffe1,0xffe2,0xffe3,0,0,0,0,0,
		 0,0x7f9,0xffe4,0xffe5,0xffe6,0xffe7,0xffe8,0xffe9,0xffea,0xffeb,0xffec,0,0,0,0,0,
		 0,0x3fe0,0xffed,0xffee,0xffef,0xfff0,0xfff1,0xfff2,0xfff3,0xfff4,0xfff5,0,0,0,0,0,
		 0x3fa,0x7fc3,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe}
	};


	unsigned char DC_HUFFSIZE[2][13] = {
		{2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 0},
		{2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0}
	};

	unsigned char AC_HUFFSIZE[2][251] = {
		{4, 2, 2, 3, 4, 5, 7, 8, 10, 16, 16, 0, 0, 0, 0, 0,
		 0, 4, 5, 7, 9, 11, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 6, 9, 12, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 7, 12, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 8, 12, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 15, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 10, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16},
		{2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 12, 0, 0, 0, 0, 0,
		 0, 4, 6, 8, 9, 11, 12, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 5, 8, 10, 12, 15, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 5, 8, 10, 12, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 6, 9, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 6, 10, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 7, 11, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 8, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 9, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0, 0,
		 0, 14, 16, 16, 16, 16, 16, 16, 16, 16, 16, 0, 0, 0, 0,
		 0, 10, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16}
	};

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			data[idx] = ptr[i*img_W + j];
			idx++;
		}

	symbol_2 = data[0];
	symbol_1 = CountBits(symbol_2);
	if (symbol_2 < 0) symbol_2 = symbol_2 - 1;
	WriteCode(file, DC_HUFFCODE[num][symbol_1], DC_HUFFSIZE[num][symbol_1], cnt);
	WriteCode(file, (unsigned short)symbol_2, symbol_1, cnt);

	for (k = 1; k <= 63; k++)
	{
		if (data[k] == 0)
		{
			run_length++;
		}
		else
		{

			while (run_length > 15)
			{
				symbol_1 = 0xf0;
				WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1],
					AC_HUFFSIZE[num][symbol_1], cnt);
				run_length = run_length - 16;
			}

			size = CountBits(data[k]);

			symbol_1 = run_length * 16 + size;
			WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1],
				AC_HUFFSIZE[num][symbol_1], cnt);

			symbol_2 = data[k];
			if (symbol_2 < 0) symbol_2 = symbol_2 - 1;
			WriteCode(file, (unsigned short)symbol_2, size, cnt);

			run_length = 0;
		}
	}

	symbol_1 = 0x00;
	WriteCode(file, (unsigned short)AC_HUFFCODE[num][symbol_1],
		AC_HUFFSIZE[num][symbol_1], cnt);
}
void ImageEncoding::WriteCode(ofstream * file, unsigned short code, int size, int * cnt) {
	int i;
	unsigned char bit;
	static unsigned char ch = 0;


	for (i = 0; i < size; i++)
	{
		bit = (code >> (size - 1 - i)) & 0x01;
		bit = bit << (7 - (*cnt));
		ch = ch | bit;

		(*cnt)++;

		if (*cnt == 8)
		{
			*cnt = 0;
			file->write((char *)&ch, 1);

			if (ch == 0xFF)
			{
				ch = 0;
				file->write((char *)&ch, 1);
			}
			ch = 0;
		}
	}

}
unsigned char ImageEncoding::CountBits(short V) {
	unsigned char Num = 0;

	if (V < 0) V = -V;
	while (V != 0)
	{
		V = V >> 1;
		Num++;
	}
	return Num;
}
void ImageEncoding::DCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo) {
	unsigned char Qtb0[64] = { 16, 11, 12, 14, 12, 10, 16, 14,
				13, 14, 18, 17, 16, 19, 24, 40,
				26, 24, 22, 22, 24, 49, 35, 37,
				29, 40, 58, 51, 61, 60, 57, 51,
				56, 55, 64, 72, 92, 78, 64, 68,
				87, 69, 55, 56, 80, 109, 81, 87,
				95, 98, 103, 104, 103, 62, 77, 113,
				121, 112, 100, 120, 92, 101, 103, 99 };

	unsigned char Qtb1[64] = { 17, 18, 18, 24, 21, 24, 47, 26,
					26, 47, 99, 66, 56, 66, 99, 99,
					99 ,99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99,
					99, 99, 99, 99, 99, 99, 99, 99 };


	int Index[64] =
	{ 0,  1,  5,  6,  14, 15, 27, 28,
	 2,  4,  7,  13, 16, 26, 29, 42,
	 3,  8,  12, 17, 25, 30, 41, 43,
	 9,  11, 18, 24, 31, 40, 44, 53,
	 10, 19, 23, 32, 39, 45, 52, 54,
	 20, 22, 33, 38, 46, 51, 55, 60,
	 21, 34, 37, 47, 50, 56, 59, 61,
	 35, 36, 48, 49, 57, 58, 62, 63 };


	int i, j;
	int x, y, u, v;
	short ZZ[64];
	double result;
	short Temp[64];
	int idx;

	for (u = 0; u < 8; u++) {
		for (v = 0; v < 8; v++) {
			result = 0.0;
			for (x = 0; x < 8; x++) {
				for (y = 0; y < 8; y++) {
					result += ptr[(int)(y*img_W + x)] *
						cos(((2.0*x + 1.0)*u*PI) / (2.0 * 8)) *
						cos(((2.0*y + 1.0)*v*PI) / (2.0 * 8));
				}
			}
			result *= 2.0 / sqrt((double)(8 * 8));
			if (u == 0) result *= 1.0 / sqrt(2.0);
			if (v == 0) result *= 1.0 / sqrt(2.0);
			ZZ[(int)(v * 8 + u)] = (short)result;
		}
	}

	memcpy(Temp, ZZ, 64 * sizeof(short));
	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
		{
			idx = (i << 3) + j;
			ZZ[Index[idx]] = Temp[idx];
		}

	for (i = 0; i < 64; i++)
		if (tableNo == 1)
			ZZ[i] = ZZ[i] / Qtb1[i];
		else
			ZZ[i] = ZZ[i] / Qtb0[i];

	for (i = 0; i < 8; i++)
		for (j = 0; j < 8; j++)
			ptr[i*img_W + j] = ZZ[i * 8 + j];
}