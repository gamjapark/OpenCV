#include "InterpolationRotation.h"

void Rotation(int RotationAngle) {
	//2차원 메모리에 입력 영상 읽어오기
	unsigned char ** Input2D = FileRead(HEIGHT, WIDTH, "lena256.raw");
	//결과 영상을 담기 위한 이차원 메모리 공간할당
	unsigned char** Output2D = Alloc2D(HEIGHT, WIDTH, 0);

	//회전을 하기 위한 기준 중심 좌표
	int h_Mid = HEIGHT / 2, w_Mid = WIDTH / 2;
	double hRotation = 0, wRotation = 0;
	//Rotation - BilinearInterpolation!!!!!
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < WIDTH; w++)
		{
			//현재 회전한 위치의 중심 좌표를 기준으로한 좌표값 계산
			double H = ((HEIGHT - 1) - h) - (h_Mid - 1);
			double W = w - (w_Mid - 1);

			//회전 각도를 라디안으로 변환해서 계산 + 회전변환 공식을 활용해서 원래의 h,w의 값을 구함
			wRotation = ((W * cos(RotationAngle * PI / 180)) - (H * sin(RotationAngle * PI / 180))) + (w_Mid - 1);
			hRotation = (H * cos(RotationAngle * PI / 180)) + (W * sin(RotationAngle * PI / 180)) + (h_Mid - 1);
			hRotation = (HEIGHT - 1) - hRotation;

			if (hRotation >= 0 && wRotation >= 0 && hRotation < HEIGHT && wRotation < WIDTH)
				Output2D[h][w] = Interpolation(Input2D, HEIGHT, WIDTH, hRotation, wRotation);
		}
	}
	//출력 영상 생성
	FileMake(Output2D, HEIGHT, WIDTH, "lenaRotation.raw");

	//2차원 배열 동적할당 해제 
	free2D(Input2D, HEIGHT);
	free2D(Output2D, HEIGHT);
}
