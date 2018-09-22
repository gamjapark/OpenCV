//Image Processing - color conversion
// RGB - Red, Green, Blue
// 1. RGB565 (High Color) : 16비트 저장공간으로 2^16가지 색 표현
//		- 가시광선 영역에서 green이 차지하는 비중 높아 1비트 더 할당
//		- 4 3 2 1 0 / 5 4 3 2 1 0 / 4 3 2 1 0
// 2. RGB888 (True Color) : 24비트 저장공간으로 2^24가지 색 표현
//		- SRGB = 국제 전기 표준 회의 표준으로 등재
//		- 8비트 투명값(알파) 추가해 32비트 색으로 사용하기도 함 
// 3. file format = *.jpg, *jpeg
//		1) 원본 영상(RAW data)을 손실 압축 - image compression
//		2) 파일의 헤더에 영상의 정보 및 압축 방식이 저장됨
//
// YUV(YCbCr) - 휘도 : 빛 밝기(Y, Luminance) / 색차 신호(U & V, Chrominance)
// 1. 흑색 인프라 환경에서 컬러 추가 표현을 위해 고안
// 2. 사람 눈은 밝기에 민감해 U, V성분의 데이터를 줄여 RGB에 비해 적은 데이터로 비슷한 화질 표현
// 3. Sub-Sampling 방식에 따라 여러 Color Format으로 나뉨 
//		1) YUV444
//		2) YUV422
//		3) YUV411
//		4) YUV420
// 4. file format = *.yuv, *.raw
//		1) 영상 픽셀 값으로만 파일 구성
//		2) 영상 분석하기 위해 영상 정보를 반드시 알아야함 : 영상 폭, 높이, Color Format
//		3) BitStream 구성 


//////Y성분만 존재하는 영상(RAW file) 로드 및 데이터 저장

#include <opencv2\opencv.hpp>

#define HEIGHT 256
#define WIDTH 256

using namespace cv;


void main() {
	Mat BlackImg(Size(HEIGHT, WIDTH), 16);
	imshow("black", BlackImg);
	waitKey(0);
}