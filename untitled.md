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

Test라는 이름으로 프로젝트를 생성하고 설정을 마친 후에

간단하게 Mat Class를 사용해서 영상 입출력을 해보기 위해서 

프로젝트 폴더 내에 다음과 같이 입력 영상\(lena.jpg\)을 넣어줍니다.

![](.gitbook/assets/image%20%2811%29.png)

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

![](.gitbook/assets/image%20%285%29.png)

다음, imwrite함수를 통해 `lean_new.jpg` 라는 출력영상이 생성되는 것을 확인할 수 있습니다. 

![](.gitbook/assets/image%20%284%29.png)

