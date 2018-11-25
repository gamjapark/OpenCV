# Rotation

### Introduction

회전변환 공식을 이용하여 영상을 회전시킬 수 있습니다.

![](../.gitbook/assets/image%20%2822%29.png)

해당 각도만큼 반시계 방향 회전시 회전 변환 공식을 사용해서 계산할 수 있습니다ㅏ.

$$
(x, y) => (x', y')
$$

![](../.gitbook/assets/image%20%2856%29.png)

해당 각도만큼 시계 방향 회전시 회전 변환 공식의 역행열을 이용해서 구할 수 있습니다.

$$
(x', y') => (x, y)
$$

![](../.gitbook/assets/image%20%2821%29.png)

영상의 좌표계는 다음과 같으므로 영상의 중심을 \(0, 0\)이 되도록 변환합니다.  


![](../.gitbook/assets/image%20%2834%29.png)

다음과 같이 영상의 중심이 \(0, 0\)이 되도록 한 후 회전 공식을 적용해야 합니다.

![](../.gitbook/assets/image%20%2833%29.png)


