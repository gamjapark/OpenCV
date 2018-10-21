# Image I/O

## namespace cv

이는 **opencv의 모든 함수와 클래스를 포함**하고 있으며,

다음과 같이 header를 추가한 이후에 사용해주면 된다.

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;
```

## Mat Class

먼저, Image I/O를 하기 위해 사용할 `Mat Class`에 대해서 알아볼 것입니다.

```cpp
class CV_EXPORTS mat{
    public:
        ....
}
```

Mat은 **수치 데이터를 표현하는 N차원 Matrix Class** 로서 

여러 가지의 생성자를 가지고 있지만, 간단하게 3가지 정도를 살펴보겠습니다!

* 가장 **기본 생성자**에 해당합니다.

```cpp
Mat();
```

* 다음은 Parameter 순으로 **'영상의 높이', '영상의 폭', '영상의 데이터 타입 및 채널'**을 이용한 생성자입니다.

```cpp
Mat(int rows(h), int cols(w), int type);
```

* 다음은 영상의 폭과 높이를 저장하는 구조체인 `Size`와 '**영상의 데이터 타입 및 채널**'을 이용한 생성자 입니다. 

```cpp
Mat(Size size, int type);
Mat(Size(cols, rows), int type);
```

## imread

**입력 영상을 읽어와 Mat 객체에 저장**하기 위한 Method입니다. 

```cpp
Mat imread(const String & filename, int flags=IMREAD_COLOR);
```

flags에는 3가지의 속성이 있습니다. 

1. IMREAD\_COLOR\(1\) : 이미지를 Color로 읽고, 투명한 부분은 무시\(DEFAULT\)
2. IMREAD\_GRAYSCALE\(0\) : 이미지를 GRAYSCALE로 읽고, 실제 이미지 처리 시 중간 단계로 많이 이용
3. IMREAD\_UNCHANGED\(-1\) : 이미지를 alpha cnannel까지 포함하여 읽

## imshow

Mat 객체에 저장된 영상을 특정 윈도우에 Dispaly하는 Method입니다.

```cpp
void imshow(const String & filename, InputArray mat);
```

## imwrite

Mat 객체에 저장된 영상을 특정 프레임에 저장하는 Method입니다.

```cpp
bool imwrite(const String & filname, InputArray mat, 
        const std::vector<int> & param=std::vector<int>());
```

## waitKey

지정된 시간만큼 키의 입력을 기다리는 Method입니다.

Parameter에 0을 넣을 경우 키보드 입력이 들어올 때까지 무한대기입니다.

```cpp
int waitKey(int delay = 0);
```

## Practice Using OpenCV

먼저, opencv의 함수를 사용해서 입력 영상을 읽어와 보여주고

출력 영상으로 저장시키는 예제를 만들어 보겠습니다.



Test라는 이름으로 프로젝트를 생성하고 설정을 마친 후에

간단하게 `Mat Class`를 사용해서 영상 입출력을 해보기 위해서 

프로젝트 폴더 내에 다음과 같이 입력 영상 `lena.jpg`을 넣어줍니다.

![](.gitbook/assets/image%20%2850%29.png)

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

void main(){

    Mat image = imread("lena.jpg", 1);
    imshow("lena", imgge);
    imwrite("lena_new.jpg", image);
    waitKey(0);

}
```

위와 같은 코드를 작성하며 다음과 결과를 보여줍니다. 

imshow함수를 통해 `lena` 라는 title로 입력 영상을 확인할 수 있습니다. 

![](.gitbook/assets/image%20%2831%29.png)

{% hint style="info" %}
여기서, imread의 type 값을 0으로 설정한다면,

```cpp
Mat image = imread("lena.jpg", 0);
```

이와 같은 image를 GRAYSCALE로 얻어낼 수 있습니다.

![](.gitbook/assets/image%20%289%29.png)
{% endhint %}

다음, imwrite함수를 통해 `lean_new.jpg` 라는 출력영상이 생성되는 것을 확인할 수 있습니다. 

![](.gitbook/assets/image%20%2829%29.png)

## Practice Using C++

위에 opencv를 활용한 예제에 대한 이해를 돕기 위해 

C++을 이용하면 어떤 식으로 구성이 되는지 보여드리겠습니다.



`FILE` 구조체를 사용해서 영상을 읽어올 때 1차원 배열을 통해서 읽어올 수 있습니다.

하지만 후에 영상을 처리할 때 픽셀 값들을 2차원적으로\(좀 더 시각적으로\) 처리하기 위해

1차원 배열을 2차원 배열로 변환하는 과정을 거쳐서 코드를 작성하였습니다.

```cpp
#include <iostream>
using namespace std;

#define WIDTH 256
#define HEIGHT 256

void main() {

	//Mat img = imread("lena.jpg", 1);
	//입력 영상을 받아오기 위한 1차원 배열 동적 할당
	unsigned char * Input1DArray = new unsigned char[HEIGHT * WIDTH];

	//입력 영상을 FILE 구조체를 이용하여 읽어옴
	FILE * fp;
	fopen_s(&fp, "lena.jpg", "rb");
	fread(Input1DArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);

	//2차원 배열 동적 할당
	unsigned char ** Input2DArray = new unsigned char*[HEIGHT];
	for (int i = 0; i < HEIGHT; i++) {
		Input2DArray[i] = new unsigned char[WIDTH];
		memset(Input2DArray[i], 0, sizeof(unsigned char) * WIDTH);
	}

	//1차원 배열로 받아온 입력 영상을 2차원 배열에 옮겨 담기
	for (int h = 0; h < HEIGHT; h++) 
		for (int w = 0; w < WIDTH; w++) 
			Input2DArray[h][w] = Input1DArray[h * WIDTH + w];

	//imwrite("lena_new.jpg", img);
	//....
	//입력 영상에 여러가지 image processing을 거친 후

	//2차원 배열에 담긴 영상을 다시 1차원 배열로 옮겨 담기
	for (int h = 0; h < HEIGHT; h++)
		for (int w = 0; w < WIDTH; w++)
			Input1DArray[h * WIDTH + w] = Input2DArray[h][w];

	fopen_s(&fp, "lena_new.jpg", "wb");
	fwrite(Input1DArray, sizeof(unsigned char), HEIGHT * WIDTH, fp);
	fclose(fp);


	//1차원 배열 동적 할당 해제
	delete[] Input1DArray;
	
	//2차원 배열 동적 할당 해제
	for (int i = 0; i < HEIGHT; i++) {
		delete[] Input2DArray[i];
	}
	delete[] Input2DArray;
}
```

위는 다음과 같이 opencv를 활용해서 입출력 한 결과와 같음을 확인할 수 있습니다.

![](.gitbook/assets/image%20%2863%29.png)

