#include "InterpolationRotation.h"

void Rotation(int RotationAngle) {
	//2���� �޸𸮿� �Է� ���� �о����
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");
	//��� ������ ��� ���� ������ �޸� �����Ҵ�
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//ȸ���� �ϱ� ���� ���� �߽� ��ǥ
	int h_Mid = HEIGHT / 2, w_Mid = WIDTH / 2;
	double hRotation = 0, wRotation = 0;
	//Rotation - BilinearInterpolation!!!!!
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++)
		{
			//���� ȸ���� ��ġ�� �߽� ��ǥ�� ���������� ��ǥ�� ���
			double H = ((HEIGHT - 1) - h) - (h_Mid - 1);
			double W = w - (w_Mid - 1);

			//ȸ�� ������ �������� ��ȯ�ؼ� ��� + ȸ����ȯ ������ Ȱ���ؼ� ������ h,w�� ���� ����
			wRotation = ((W * cos(RotationAngle * PI / 180)) - (H * sin(RotationAngle * PI / 180))) + (w_Mid - 1);
			hRotation = (H * cos(RotationAngle * PI / 180)) + (W * sin(RotationAngle * PI / 180)) + (h_Mid - 1);
			hRotation = (HEIGHT - 1) - hRotation;

			if (hRotation >= 0 && wRotation >= 0 && hRotation < HEIGHT && wRotation < WIDTH)
				Output2D[h][w] = Interpolation(Input2D, HEIGHT, WIDTH, hRotation, wRotation);
		}
	}
	//��� ���� ����
	FileMake(Output2D, HEIGHT, WIDTH, "lenaRotation.raw");

	//2���� �迭 �����Ҵ� ���� 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, HEIGHT);
}
