#include <opencv2\opencv.hpp>
using namespace cv;

void main() {
	// ����ó�� ���� ���� 
	// Mat Class : class CV_EXPORTS Mat - ��ġ �����͸� ǥ���ϴ� N���� ��� Ŭ����
	// ---- ������ ----
	// 1. Mat(); : �⺻ ������
	// 2. Mat(int rows(h), int cols(w), int type); : ���� ����, ���� ��, ���� ������ Ÿ�� & ä��
	// 3. Mat(Size size, int type); : ���� ���� ����, ������ ������ Ÿ�� & ä��
	// cf) Size : ���� ���� ���̸� �����ϴ� ����ü ex) Mat(Size(cols, rows), type);

	Mat img = imread("lena.jpg", 1);
	// Mat imread(const String & filename, int flags=IMREAD_COLOR);
	// : ���� ������ Mat ��ü�� ����
	// cf) flag 
	//     1. IMREAD_COLOR (1) : �̹����� Color�� �а�, ������ �κ��� ����(Default)
	//	   2. IMREAD_GRAYSCALE (0) : �̹����� Grayscale�� �а�, ���� �̹��� ó���� �߰��ܰ�� ���� ���
	//	   3. IMREAD_UNCHANGED (-1) : �̹����� alpha channel���� �����Ͽ� ����

	imshow("Image Name = lena", img);
	// void imshow(const String & filename, InputArray mat);
	// : �о�� �̹����� Ư�� �����쿡 Display

	imwrite("NewName_lena.jpg", img);
	// bool imwrite(const String & filename, InputArray img, const std::vector< int > &params=std::vector< int >());
	// : ��ȯ�� �̹����� �������� Ư�� ������ ����

	waitKey(0);
	// int waitKey(int delay = 0);
	// : ������ �ð���ŭ Ű�� �Է��� ��ٸ��� �޼��� -> 0 �Է½� Ű���� �Է� ���� ������ ���Ѵ��
}