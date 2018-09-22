#include <opencv2\opencv.hpp>
using namespace cv;

void main() {
	// 영상처리 시작 기초 
	// Mat Class : class CV_EXPORTS Mat - 수치 데이터를 표현하는 N차원 행렬 클래스
	// ---- 생성자 ----
	// 1. Mat(); : 기본 생성자
	// 2. Mat(int rows(h), int cols(w), int type); : 영상 높이, 영상 폭, 영상 데이터 타입 & 채널
	// 3. Mat(Size size, int type); : 영상 폭과 높이, 영상의 데이터 타입 & 채널
	// cf) Size : 영상 폭과 높이를 저장하는 구조체 ex) Mat(Size(cols, rows), type);

	Mat img = imread("lena.jpg", 1);
	// Mat imread(const String & filename, int flags=IMREAD_COLOR);
	// : 영상 파일을 Mat 객체에 저장
	// cf) flag 
	//     1. IMREAD_COLOR (1) : 이미지를 Color로 읽고, 투명한 부분은 무시(Default)
	//	   2. IMREAD_GRAYSCALE (0) : 이미지를 Grayscale로 읽고, 실제 이미지 처리시 중간단계로 많이 사용
	//	   3. IMREAD_UNCHANGED (-1) : 이미지를 alpha channel까지 포함하여 읽음

	imshow("Image Name = lena", img);
	// void imshow(const String & filename, InputArray mat);
	// : 읽어온 이미지를 특정 윈도우에 Display

	imwrite("NewName_lena.jpg", img);
	// bool imwrite(const String & filename, InputArray img, const std::vector< int > &params=std::vector< int >());
	// : 변환된 이미지나 동영상의 특정 프레임 저장

	waitKey(0);
	// int waitKey(int delay = 0);
	// : 지정된 시간만큼 키의 입력을 기다리는 메서드 -> 0 입력시 키보드 입력 들어올 때까지 무한대기
}