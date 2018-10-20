# Color Conversion

## RGB

빛의 3원색인 Red, Blue, Green의 합성어로서 다양한 색을 표현할 수 있습니다.

1. RGB565 \(High Color\)

   ![](../.gitbook/assets/image%20%288%29.png)  
   Red와 Blue에 5Bit, Green에 6Bit를 할당해서 총 16Bit 저장공간을 이용해서  
   다양한 색상을 표현합니다.  

2. RGB888 \(True Color\) == SRGB 이는 국제 전기 표준 회의의 표준으로 등재되었으며, 때로는 8 Bit의 투명 값\(Alpha\)을 추가해 32Bit Color로 사용합니다.

![](../.gitbook/assets/image%20%283%29.png)

## YUV

빛의 밝기를 나타내는 휘도\(Y\)와 색차 신호\(U & V\)를 이용해 색을 표현합니다.

이는 흑색 인프라 환경에서 컬러를 추가적으로 표현하기 위해 고안되었습니다.

사람의 눈은 밝기에 민감하여 U, V 성분의 데이터를 줄여 RGB에 비해 

적은 데이터로도 비슷한 화질을 표현할 수 있습니다.

1. YUV444

   ![](../.gitbook/assets/image%20%2811%29.png)

2. YUV422  


   ![](../.gitbook/assets/image%20%281%29.png)

3. YUV411  


   ![](../.gitbook/assets/image%20%2822%29.png)

4. YUV420  


   ![](../.gitbook/assets/image%20%2825%29.png)

Format

* YUV - .yuv\(Raw Data\) .raw 영상의 픽셀 값만으로 파일 구성이 되어 있습니다.
* RGB - .jpg\(Raw Data 손실 압축\) .jpeg 파일의 Header에 영상의 정보 및 압축 방식이 저장되어 있습니다. 



