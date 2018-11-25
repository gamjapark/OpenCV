
#include "ImageDecoding.h"

ImageDecoding::ImageDecoding(const char * filePath) {

	if (strcmp(strchr(filePath, '.'), ".JPG") != 0 && strcmp(strchr(filePath, '.'), ".jpg") != 0 &&
		strcmp(strchr(filePath, '.'), ".raw") != 0 && strcmp(strchr(filePath, '.'), ".RAW") != 0) {
		//파일 확장자 확인
		cout << "JPG 또는 Gray 파일이 아닙니다." << endl;
	}

	if (strcmp(strchr(filePath, '.'), ".JPG") == 0 || strcmp(strchr(filePath, '.'), ".jpg") == 0) {		//jpg image
		depth = 3;
	}
	else {	//gray image
		depth = 1;
	}

	depth = 1;
	outputImg = new unsigned char*[nHeight_in];
	for (int i = 0; i < nHeight_in; i++) {
		outputImg[i] = new unsigned char[nWidth_in * depth];
	}
}
ImageDecoding::~ImageDecoding() {
	if (outputImg != NULL) {
		for (int i = 0; i < nHeight_in; i++) {
			delete[] outputImg[i];
		}
		delete[] outputImg;
	}
}

void ImageDecoding::OnDeCompression(const char * filePath, const char * writePath) {
	ifstream readfile;
	readfile.open(filePath, ios::binary | ios::in);

	/*
	for (int i = 0; i < nHeight_in; i++) {
	readfile.read((char *)inputImg[i], sizeof(unsigned char) * nWidth_in * depth);
	}

	readfile.close();
	*/
	depth = 1;
	unsigned char *img;
	short *Y = NULL, *Cb = NULL, *Cr = NULL;
	int img_W = ((nWidth_in - 1) / 8 + 1) * 8;
	int img_H = ((nHeight_in - 1) / 8 + 1) * 8;
	int i, j;

	img = new unsigned char[img_W *  img_H * 3];
	memset(img, 0, img_W * img_H * 3);

	ReadSOI(&readfile);
	ReadDQT(&readfile);
	ReadDHT(&readfile);
	ReadSOF(&readfile);
	ReadSOS(&readfile);

	if (depth == 3) {
		Y = new short[img_W * img_H];
		memset(Y, 0, sizeof(short) * img_W * img_H);
		Cb = new short[img_W * img_H];
		memset(Cb, 0, sizeof(short) * img_W * img_H);
		Cr = new short[img_W * img_H];
		memset(Cr, 0, sizeof(short) * img_W * img_H);

	}
	else {
		Y = new short[img_W * img_H];
		memset(Y, 0, sizeof(short) * img_W * img_H);
	}

	int cnt = 8;
	unsigned char code = 0;
	for (int i = 0; i < img_H; i = i + 8)
	{
		for (int j = 0; j < img_W; j = j + 8)
		{
			Decode(&readfile, &Y[i*img_W + j], 0, img_W, &cnt, &code);
			if (depth == 3) {
				Decode(&readfile, &Cb[i*img_W + j], 1, img_W, &cnt, &code);
				Decode(&readfile, &Cr[i*img_W + j], 1, img_W, &cnt, &code);
			}
		}
	}
	ReadEOI(&readfile);
	readfile.close();

	ofstream writefile;
	writefile.open(writePath, ios::binary | ios::out);
	if (depth == 3) {
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

		for (i = 1; i < 64; i++)
		{
			DC1[i] = DC1[i] + DC1[i - 1];
			DC2[i] = DC2[i] + DC2[i - 1];
			DC3[i] = DC3[i] + DC3[i - 1];
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

		for (i = 0; i < img_H; i = i + 8)
		{
			for (j = 0; j < img_W; j = j + 8)
			{
				IDCT_Zigzag_Quantization(&Y[i*img_W + j], img_W, 0);
				IDCT_Zigzag_Quantization(&Cb[i*img_W + j], img_W, 1);
				IDCT_Zigzag_Quantization(&Cr[i*img_W + j], img_W, 1);
			}
		}

		int idx1, idx2;

		for (i = 0; i < nHeight_in; i++)
		{
			for (j = 0; j < nWidth_in; j++)
			{
				idx1 = i * img_W + j;
				idx2 = i * (img_W * 3) + j * 3;
				img[idx2] = Y[idx1] + 128;
				img[idx2 + 1] = Cb[idx1] + 128;
				img[idx2 + 2] = Cr[idx1] + 128;
			}
		}

		float R, G, B;
		float y_val, cb, cr;

		for (i = 0; i < nHeight_in; i++)
		{
			for (j = 0; j < nWidth_in; j++)
			{
				y_val = (float)img[(i*img_W + j) * 3];
				cb = (float)img[(i*img_W + j) * 3 + 1];
				cr = (float)img[(i*img_W + j) * 3 + 2];

				R = y_val + 1.402f*(cr - 128);
				G = y_val - 0.344f*(cb - 128) - 0.714f*(cr - 128);
				B = y_val + 1.772f*(cb - 128);

				if (R > 255.0) R = 255.0; if (R < 0.0) R = 0.0;
				if (G > 255.0) G = 255.0; if (G < 0.0) G = 0.0;
				if (B > 255.0) B = 255.0; if (B < 0.0) B = 0.0;

				img[(i*img_W + j) * 3] = (unsigned char)R;
				img[(i*img_W + j) * 3 + 1] = (unsigned char)G;
				img[(i*img_W + j) * 3 + 2] = (unsigned char)B;
			}
		}
	}
	else {
		int nBlocks_X = img_W / 8;
		int nBlocks_Y = img_H / 8;
		int idx = 0;

		short *DC = new short[nBlocks_X*nBlocks_Y];

		for (i = 0; i < img_H; i = i + 8)
			for (j = 0; j < img_W; j = j + 8)
				DC[idx++] = Y[i*img_W + j];

		for (int i = 1; i < nBlocks_X * nBlocks_Y; i++) {
			DC[i] = DC[i] + DC[i - 1];
		}

		idx = 0;
		for (i = 0; i < img_H; i = i + 8)
			for (j = 0; j < img_W; j = j + 8)
				Y[i*img_W + j] = DC[idx++];
		
		for (i = 0; i < img_H; i = i + 8) {
			for (j = 0; j < img_W; j = j + 8) {
				
				IDCT_Zigzag_Quantization(&Y[i*img_W + j], img_W, 0);
			}
		}


		for (i = 0; i < nHeight_in; i++) {
			for (j = 0; j < nWidth_in; j++) {
				img[i*nWidth_in + j] = (unsigned char) (Y[i*nWidth_in + j] + 128);
			}
		}
	}
	WriteFile(&writefile, img);
	writefile.close();
	delete[] img;
}


void ImageDecoding::ReadSOI(ifstream * file) {
	unsigned char buf[2];
	file->read((char *)buf, 2);
}
void ImageDecoding::ReadDQT(ifstream * file) {
	unsigned char marker[2];
	short int tableSize;
	unsigned char c;

	unsigned char QT0[64];
	unsigned char QT1[64];

	file->read((char *)marker, 2);

	file->read((((char *)(&tableSize)) + 1), 1);
	file->read((((char *)(&tableSize)) + 0), 1);

	file->read((char *)&c, 1);
	file->read((char *)QT0, 64);

	if (depth == 3) {
		file->read((char *)&c, 1);
		file->read((char *)QT1, 64);
	}
}
void ImageDecoding::ReadDHT(ifstream * file) {
	unsigned char marker[2];
	short int tableSize;
	unsigned char c;

	unsigned char HT_code_0[16];

	unsigned char HT_symbol_0[12];

	unsigned char HT_code_16[16];

	unsigned char HT_symbol_16[162];

	unsigned char HT_code_1[16];

	unsigned char HT_symbol_1[12];

	unsigned char HT_code_17[16];

	unsigned char HT_symbol_17[162];

	file->read((char *)marker, 2);

	file->read((((char *)(&tableSize)) + 1), 1);
	file->read((((char *)(&tableSize)) + 0), 1);


	file->read((char *)&c, 1);
	file->read((char *)HT_code_0, 16);
	file->read((char *)HT_symbol_0, 12);

	file->read((char *)&c, 1);
	file->read((char *)HT_code_16, 16);
	file->read((char *)HT_symbol_16, 162);

	if (depth == 3) {
		file->read((char *)&c, 1);
		file->read((char *)HT_code_1, 16);
		file->read((char *)HT_symbol_1, 12);

		file->read((char *)&c, 1);
		file->read((char *)HT_code_17, 16);
		file->read((char *)HT_symbol_17, 162);
	}

}
void ImageDecoding::ReadSOF(ifstream * file) {
	unsigned short headerSize, w;
	unsigned char c;
	int i;
	unsigned char marker[2];

	file->read((char *)marker, 2);

	//프레임 헤더 길이 읽기
	file->read((((char *)(&headerSize)) + 1), 1);
	file->read((((char *)(&headerSize)) + 0), 1);

	file->read((char *)&c, 1);

	//영상 높이 읽기
	file->read((((char *)(&w)) + 1), 1);
	file->read((((char *)(&w)) + 0), 1);
	//영상 넓이 읽기
	file->read((((char *)(&w)) + 1), 1);
	file->read((((char *)(&w)) + 0), 1);

	//depth저장 -> 컴포턴트 수 읽기
	file->read((char *)&c, 1);
	for (i = 1; i <= depth; i++)
	{
		//컴포턴트 번호
		file->read((char *)&c, 1);
		//수직 수평 비율 0x11읽기
		file->read((char *)&c, 1);
		//양자화 테이블 번호 읽기
		file->read((char *)&c, 1);
	}
}
void ImageDecoding::ReadSOS(ifstream * file) {
	unsigned short headerSize;
	unsigned char c;
	unsigned char marker[2];
	int i;

	file->read((char *)marker, 2);

	//스캔 헤더의 크기 읽기 
	file->read((((char *)(&headerSize)) + 1), 1);
	file->read((((char *)(&headerSize)) + 0), 1);

	//depth저장 -> 컴포턴트 수 읽기
	file->read((char *)&c, 1);

	for (i = 1; i <= depth; i++)
	{
		//컴포턴트 번호 읽기
		file->read((char *)&c, 1);
		//허프만 테이블 번호 읽기
		file->read((char *)&c, 1);
	}

	//c = 0;
	file->read((char *)&c, 1);
	//c = 0x3f;
	file->read((char *)&c, 1);
	//c = 0;
	file->read((char *)&c, 1);

}
void ImageDecoding::ReadEOI(ifstream * file) {
	unsigned char marker[2];
	file->read((char *)marker, 2);

}

void ImageDecoding::Decode(ifstream * file, short * ptr, int num, int img_W, int * cnt, unsigned char *code) {
	unsigned short int DC_HUFFCODE[2][13] = {
		{ 0x0, 0x2, 0x3, 0x4, 0x5, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0 },
		{ 0x0, 0x1, 0x2, 0x6, 0xe, 0x1e, 0x3e, 0x7e, 0xfe, 0x1fe, 0x3fe, 0x7fe, 0 }
	};

	unsigned short int AC_HUFFCODE[2][251] = {
		{ 0xa,0x0,0x1,0x4,0xb,0x1a,0x78,0xf8,0x3f6,0xff82,0xff83,0,0,0,0,0,
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
		0x7f9,0xfff5,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe },
		{ 0x0,0x1,0x4,0xa,0x18,0x19,0x38,0x78,0x1f4,0x3f6,0xff4,0,0,0,0,0,
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
		0x3fa,0x7fc3,0xfff6,0xfff7,0xfff8,0xfff9,0xfffa,0xfffb,0xfffc,0xfffd,0xfffe }
	};

	unsigned char DC_HUFFSIZE[2][13] = {
		{ 2, 3, 3, 3, 3, 3, 4, 5, 6, 7, 8, 9, 0 },
		{ 2, 2, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 0 }
	};

	unsigned char AC_HUFFSIZE[2][251] = {
		{ 4, 2, 2, 3, 4, 5, 7, 8, 10, 16, 16, 0, 0, 0, 0, 0,
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
		11, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
		{ 2, 2, 3, 4, 5, 5, 6, 7, 9, 10, 12, 0, 0, 0, 0, 0,
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
		0, 10, 15, 16, 16, 16, 16, 16, 16, 16, 16, 16 }
	};

	unsigned char *temp = new unsigned char[16];

	int index = 0;
	int x = 0;
	int i = 0;
	int sizeIndex = 0;
	int find = 0;
	int hufIndex = -1;
	unsigned short findValue = 0;

	// Decoding된 블럭
	short *block = new short[64];
	int blockIndex = 0;
	int preFind = -1;
	int finishCount = -1;

	while (finishCount == -1) { // 이미지 역역의 데이터만큼 반복 
		for (int size = 2; size <= 16; size++) {
			if (find == 1) {
				x = 0;
				sizeIndex = 0;
				size = hufIndex;
			}
			// 정보 하나당 입력비트가 2-16자리 수이기 때문에 자리수만큼 값을 변환하기 위한 반복문 
			for (int j = sizeIndex; j < size; j++) {
				if ((*cnt) == 8) {
					file->read((char *)code, 1);
					if ((*code) == 0xFF) {
						file->read((char *)code, 1);
						(*code) = 0xFF;
					}
					(*cnt) = 0;
				}
				if (x == 16) {
					x = 0;
				}
				temp[x] = getAbit((*code), (7 - (*cnt)));
				x++;
				(*cnt)++;
			}

			findValue = 0;
			for (int c = 0; c < size; c++) {
				findValue = setAbit(findValue, size - 1 - c, temp[c]);
			}

			if (find == 1) {
				short storCH = 0;
				if (getAbit(findValue, hufIndex - 1) == 0) {
					for (int c = 0; c < size; c++) {
						storCH = setAbit(storCH, size - 1 - c, 1 - getAbit(findValue, size - 1 - c));
					}
					storCH *= -1;
				}
				else {
					storCH = findValue;
				}

				block[blockIndex] = storCH;
				size = 1;
				sizeIndex = 0;
				x = 0;
				find = 0;
				preFind = 1;
				blockIndex++;
			}
			else if (find == 0) {
				if (blockIndex == 0) {
					for (int b = 0; b < 13; b++) {
						if (DC_HUFFCODE[num][b] == findValue && size == DC_HUFFSIZE[num][b]) {
							hufIndex = b;
							sizeIndex = 0;
							x = 0;
							find = 1;
							break;
						}
					}
				}
				else {
					for (int b = 0; b < 251; b++) {
						if (AC_HUFFCODE[num][b] == findValue && size == AC_HUFFSIZE[num][b]) {
							if (b == 0x00) {
								finishCount = 1;
							}
							else if (b == 0xf0) {
								for (int c = 0; c < 16; c++) {
									block[blockIndex] = 0;
									blockIndex++;
								}
								size = 1;
								sizeIndex = 0;
								x = 0;
								find = 0;
								preFind = 1;
							}
							else {
								int runLength = b / 16;
								int runCount = runLength;
								while (runCount > 0) {
									block[blockIndex] = 0;
									blockIndex++;
									runCount--;
								}
								hufIndex = b - runLength * 16;
								sizeIndex = 0;
								x = 0;
								find = 1;
							}
							break;
						}
					}
				}
			}

			if (find == 0) {
				sizeIndex = size;
				if (preFind == 1) {
					sizeIndex = 0;
					x = 0;
					preFind = -1;
				}
			}

			if (finishCount == 1)
				break;
		}
	}

	for (int c = blockIndex; c < 64; c++) {
		block[c] = 0;
	}

	for (i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			ptr[i*img_W + j] = block[i * 8 + j];
		}
	}

}
void ImageDecoding::WriteFile(ofstream * file, unsigned char * out)
{
	file->write((char *)out, nHeight_in*nWidth_in*depth);
}
void ImageDecoding::IDCT_Zigzag_Quantization(short * ptr, int img_W, int tableNo)
{
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
	{
		0, 1, 8, 16, 9, 2, 3, 10,
		17, 24, 32, 25, 18, 11, 4, 5,
		12, 19, 26, 33, 40, 48, 41, 34,
		27, 20, 13, 6, 7, 14, 21, 28,
		35, 42, 49, 56, 57, 50, 43, 36,
		29, 22, 15, 23, 30, 37, 44, 51,
		58, 59, 52, 45, 38, 31, 39, 46,
		53, 60, 61, 54, 47, 55, 62, 63
	};

	//Quantization
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (tableNo == 1) {
				ptr[i*img_W + j] = ptr[i * img_W + j] * Qtb1[(i << 3) + j];
			}
			else {
				ptr[i*img_W + j] = ptr[i * img_W + j] * Qtb0[(i << 3) + j];
			}
		}
	}


	short ZZ[64];
	short temp[64];
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i * 8 + j] = ptr[i * img_W + j];
		}
	}

	int idx;
	//Zigzag_
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
		{
			idx = (i << 3) + j;
			ZZ[Index[idx]] = temp[idx];
		}
	}

	//IDCT
	double cv1;
	double cv2;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			double result = 0.0;
			if (0 == x) cv1 = 1.0 / sqrt(2.0);
			else cv1 = 1.0;
			if (0 == y) cv2 = 1.0 / sqrt(2.0);
			else cv2 = 1.0;
			for (int u = 0; u < 8; u++) {
				for (int v = 0; v < 8; v++) {
					result += ZZ[u * 8 + v] * cv1 * cv2 * cos(((2.0*x + 1.0)*u*PI) / (2.0 * 8)) * cos(((2.0*y + 1.0)*v*PI) / (2.0 * 8));
				}
			}
			result *= ((2 / sqrt(8 * 8)));
			ptr[x * img_W + y] = (short)result;
		}
	}
}
short ImageDecoding::getAbit(short x, int n)
{
	return (x&(1 << n)) >> n;
}

short ImageDecoding::setAbit(short x, int n, int b)
{
	if (b == 1)
		return x | (1 << n);
	else
		return x & (~(1 << n));
}
