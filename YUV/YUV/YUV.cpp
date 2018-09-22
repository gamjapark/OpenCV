//Image Processing - color conversion
// RGB - Red, Green, Blue
// 1. RGB565 (High Color) : 16��Ʈ ����������� 2^16���� �� ǥ��
//		- ���ñ��� �������� green�� �����ϴ� ���� ���� 1��Ʈ �� �Ҵ�
//		- 4 3 2 1 0 / 5 4 3 2 1 0 / 4 3 2 1 0
// 2. RGB888 (True Color) : 24��Ʈ ����������� 2^24���� �� ǥ��
//		- SRGB = ���� ���� ǥ�� ȸ�� ǥ������ ����
//		- 8��Ʈ ����(����) �߰��� 32��Ʈ ������ ����ϱ⵵ �� 
// 3. file format = *.jpg, *jpeg
//		1) ���� ����(RAW data)�� �ս� ���� - image compression
//		2) ������ ����� ������ ���� �� ���� ����� �����
//
// YUV(YCbCr) - �ֵ� : �� ���(Y, Luminance) / ���� ��ȣ(U & V, Chrominance)
// 1. ��� ������ ȯ�濡�� �÷� �߰� ǥ���� ���� ���
// 2. ��� ���� ��⿡ �ΰ��� U, V������ �����͸� �ٿ� RGB�� ���� ���� �����ͷ� ����� ȭ�� ǥ��
// 3. Sub-Sampling ��Ŀ� ���� ���� Color Format���� ���� 
//		1) YUV444
//		2) YUV422
//		3) YUV411
//		4) YUV420
// 4. file format = *.yuv, *.raw
//		1) ���� �ȼ� �����θ� ���� ����
//		2) ���� �м��ϱ� ���� ���� ������ �ݵ�� �˾ƾ��� : ���� ��, ����, Color Format
//		3) BitStream ���� 


//////Y���и� �����ϴ� ����(RAW file) �ε� �� ������ ����

#include <opencv2\opencv.hpp>

#define HEIGHT 256
#define WIDTH 256

using namespace cv;


void main() {
	Mat BlackImg(Size(HEIGHT, WIDTH), 16);
	imshow("black", BlackImg);
	waitKey(0);
}