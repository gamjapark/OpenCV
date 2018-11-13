# Spatial Filtering

## Filter

영상을 사용자가 원하는 대로 변화시키거나 개선시키기 위해 사용합니다.

## Padding

필터를 적용할 때 발생되는 Boundary 문제를 해결하기 위해 사용합니다.

* Mirroring = 근처에 있는 Pixel 값을 취합니다. 

![](../.gitbook/assets/image%20%2839%29.png)

```cpp
unsigned char ** Padding(unsigned char** InputArray, int FilterSize) {
	int PadSize = (int)FilterSize / 2;
	unsigned char **Pad = new unsigned char*[HEIGHT + 2 * PadSize];
	for (int i = 0; i < HEIGHT + 2 * PadSize; i++) {
		Pad[i] = new unsigned char[WIDTH + 2 * PadSize];
		memset(Pad[i], 0, sizeof(unsigned char) * (WIDTH + 2 * PadSize));
	}

	//중앙
	for (int h = 0; h < HEIGHT; h++) 
		for (int w = 0; w < WIDTH; w++) 
			Pad[h + PadSize][w + PadSize] = InputArray[h][w];
	//위, 아래
	for (int h = 0; h < PadSize; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Pad[h][w + PadSize] = InputArray[0][w];
			Pad[h + HEIGHT - 1 + PadSize][w + PadSize] = InputArray[HEIGHT - 1][w];
		}
	}
	//왼쪽 오른쪽
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < PadSize; w++) {
			Pad[h + PadSize][w] = InputArray[h][0];
			Pad[h + PadSize][w + WIDTH - 1 + PadSize] = InputArray[h][WIDTH - 1];
		}
	}

	//모서리
	for (int h = 0; h < PadSize; h++) {
		for (int w = 0; w < PadSize; w++) {
			Pad[h][w] = InputArray[0][0];
			Pad[h][w + WIDTH - 1 + PadSize] = InputArray[0][WIDTH - 1];
			Pad[h + HEIGHT - 1 + PadSize][w] = InputArray[HEIGHT - 1][0];
			Pad[h + HEIGHT - 1 + PadSize][w + WIDTH - 1 + PadSize] = InputArray[HEIGHT - 1][WIDTH - 1];
		}
	}
}
```

* Zero Padding = 0으로 채웁니다.

![](../.gitbook/assets/image%20%2816%29.png)

```cpp
unsigned char ** Padding(unsigned char** InputArray, int FilterSize) {
	int PadSize = (int)FilterSize / 2;
	unsigned char **Pad = new unsigned char*[HEIGHT + 2 * PadSize];
	for (int i = 0; i < HEIGHT + 2 * PadSize; i++) {
		Pad[i] = new unsigned char[WIDTH + 2 * PadSize];
		memset(Pad[i], 0, sizeof(unsigned char) * (WIDTH + 2 * PadSize));
	}

	//중앙
	for (int h = 0; h < HEIGHT; h++) 
		for (int w = 0; w < WIDTH; w++) 
			Pad[h + PadSize][w + PadSize] = InputArray[h][w];
	//위, 아래
	for (int h = 0; h < PadSize; h++) {
		for (int w = 0; w < WIDTH; w++) {
			Pad[h][w + PadSize] = 0;
			Pad[h + HEIGHT - 1 + PadSize][w + PadSize] = 0;
		}
	}
	//왼쪽 오른쪽
	for (int h = 0; h < HEIGHT; h++) {
		for (int w = 0; w < PadSize; w++) {
			Pad[h + PadSize][w] = 0;
			Pad[h + PadSize][w + WIDTH - 1 + PadSize] = 0;
		}
	}

	//모서리
	for (int h = 0; h < PadSize; h++) {
		for (int w = 0; w < PadSize; w++) {
			Pad[h][w] = 0;
			Pad[h][w + WIDTH - 1 + PadSize] = 0;
			Pad[h + HEIGHT - 1 + PadSize][w] = 0;
			Pad[h + HEIGHT - 1 + PadSize][w + WIDTH - 1 + PadSize] = 0;
		}
	}
}
```

## Average Filter

필터 범위 내에 있는 픽셀 값들의 평균으로 픽셀 값을 변환하는 필터 입니다.

## Gaussian Filter

## Smooth Filter

## Sharpen Filter

