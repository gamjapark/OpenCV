# Color Conversion

## RGB

빛의 3원색인 Red, Blue, Green의 합성어로서 다양한 색을 표현할 수 있습니다.

1. RGB565 \(High Color\)

   ![](../.gitbook/assets/image%20%289%29.png)  
   Red와 Blue에 5Bit, Green에 6Bit를 할당해서 총 16Bit 저장공간을 이용해서  
   다양한 색상을 표현합니다.  

2. RGB888 \(True Color\) == SRGB  
   이는 국제 전기 표준 회의의 표준으로 등재되었으며,  
   때로는 8 Bit의 투명 값\(Alpha\)을 추가해 32Bit Color로 사용합니다.

   ![](../.gitbook/assets/image%20%284%29.png)

3. File Format = \*.jpg / \*jpeg - Image Compression : 원본 영상\(Raw Data\)를 손실 압축  - 파일의 헤더에 영상의 정보 및 압축 방식이 저장

## YUV

빛의 밝기를 나타내는 휘도\(Y\)와 색차 신호\(U & V\)를 이용해 색을 표현합니다.

이는 흑색 인프라 환경에서 컬러를 추가적으로 표현하기 위해 고안되었습니다.

사람의 눈은 밝기에 민감하여 U, V 성분의 데이터를 줄여 RGB에 비해 

적은 데이터로도 비슷한 화질을 표현할 수 있습니다.

1. YUV444

   ![](../.gitbook/assets/image%20%2812%29.png)

2. YUV422  


   ![](../.gitbook/assets/image%20%281%29.png)

3. YUV411  


   ![](../.gitbook/assets/image%20%2823%29.png)

4. YUV420  


   ![](../.gitbook/assets/image%20%2826%29.png)

File Format = \*.yuv / \*.raw

* 영상의 픽셀 값만으로 파일 구성이 되어 있습니다.
* 영상을 분석하기 위해 영상 정보를 반드시 알아야 합니다\(폭, 높이, Color Format\)
* BitStream 구성\(YUV444, YUV422\)

## RGB -&gt; YUV 변환

Y = 0.299R + 0.587G + 0.114B

U = 0.564\(B - Y\) = -0.169R - 0.331G + 0.499B

V = 0.713\(R - Y\) = 0.499R - 0.418G - 0.081B

## YUV -&gt; RGB 변환

R = Y + 1.402V

G = Y - 0.344U - 0.714V

B = Y + 1.772U

## RGB -&gt; YCbCr 변환

Y = 0.299R + 0.587G + 0.114B

U = 0.564\(B - Y\) = -0.169R - 0.331G + 0.499B + 128

V = 0.713\(R - Y\) = 0.499R - 0.418G - 0.081B + 128

## YCbCr -&gt; RGB 변환

 = Y + 1.402\(V - 128\)

G = Y - 0.344\(U - 128\) - 0.714\(V - 128\)

B = Y + 1.772\(U - 128\)

## Practice Using OpenCV

이는 RGB에서 Y값을 계산하여 `imread` 시 type 값에 0을 넣는 것과 같은 결과를 얻을 수 있습니다.

```cpp
#include <opencv2/opencv.hpp>
using namespace cv;

void main() {

	Mat img = imread("lena.jpg", 1);
	Mat image_Gray(Size(512, 512), 0);
	
	for (int h = 0; h < 512; h++) {
		for (int w = 0; w < 512; w++) {
			Vec3b RGB = img.at<Vec3b>(h, w);
			image_Gray.at<uchar>(h, w) = RGB[2] * 0.299 + RGB[1] * 0.587 
											+ RGB[0] * 0.114;
		}
	}
	imshow("input", img);
	imshow("output", image_Gray);

	waitKey(0);
}
```

위의 코드는 다음과 같은 결과를 출력합니다.

![](../.gitbook/assets/image%20%282%29.png)



