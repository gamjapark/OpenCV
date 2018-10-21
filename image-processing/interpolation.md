# Interpolation

## Introduction

보간은 불연속적인 데이터 점들을 이용해 점들 사이의 값을 추정하는 방법입니다. 

![](../.gitbook/assets/image%20%2835%29.png)

![](../.gitbook/assets/image%20%284%29.png)

## Replication\(N Time\) Interpolation

보간하려고하는 index의 왼쪽의 값을 동일시하는 보간 방법입니다.

![](../.gitbook/assets/image%20%2838%29.png)

하지만 2의 바로 왼쪽의 Pixel의 경우 2와 더 가까움에도 불구하고 1의 Pixel 값을 취하고 있습니다.

이러한 경우에, 정확도는 좋지 않게 됩니다.

이를 한층 해결한 것이 Nearest-Neighbor Interpolation입니다.

## Nearest-Neighbor Interpolation

보간하려는 값의 index에서 가장 가까이에 있는 값을 대입하는 방법입니다.

![](../.gitbook/assets/image%20%2849%29.png)

영상에서는 다음와 같이 구할 수 있습니다. 

해당 Image에서 \(h, w\)의 값은 가장 가까이 있는 픽셀 값인 \(H\[1\], W\[1\]\)를 택해서 사용합니다.

![](../.gitbook/assets/image%20%2843%29.png)

하지만 이보다 한층 더 정확도를 높이기 위해서 Linear Interpolation을 사용합니다.

## Linear Interpolation

보간 하려는 점 양쪽의 두 점 사이를 선으로 만들어 직선의 방정식을 사용하는 방법입니다.

![](../.gitbook/assets/image%20%2827%29.png)

이는 다음과 같이 양옆의 두 점으로 보간하려는 점의 Pixel 값을 구할 수 있습니다. 

![](../.gitbook/assets/image%20%2865%29.png)

## Bilinear Interpolation

위에서 배운 Linear Interpolation을 확장하여 

보간하려는 점 근처의 4개의 점으로 3번의 Linear Interpolation을 통해 구하는 방법입니다.

![](../.gitbook/assets/image%20%2817%29.png)

## Cubic Interpolation

보간하려는 점 양쪽의 두 Pixel, 즉 총 4개의 Pixel이 3차 곡선 상에 있다고 가정하고,

3차 함수 식을 이용하는 방법입니다.

![](../.gitbook/assets/image%20%2842%29.png)

![](../.gitbook/assets/image%20%2811%29.png)

3차 함수와 그의 미분 다음과 같이 두자.

\(x = 0, x = 1 에서 함수의 미분 값을 알고 있는 경우에 구간 \[0, 1\]에서 이는 보간할 수 있습니다.\)

$$
f(x) = ax^3 + bx^2 + cx + d
$$

$$
f'(x) = 3ax^2 + 2bx + c
$$

우리가 보간하려는 점이 x = 0, x = 1 사이에 있다고 가정을 하고 식에 대입합니다.

$$
f(1) = a + b + c + d, f(0) = d, f'(1) = 3a + 2b + c, f'(0) = c
$$

위의 결과를 연립하여 다음과 같은 결과를 얻을 수 있습니다.

$$
a = f'(0) + 2f(0) + f'(1) - 2f(1)
,  b= -2f'(0) - 3f(0) - f'(1) + 3f(1)
$$

$$
c = f'(0),  d = f(0)
$$

여기서 x = -1, 0, 1, 2에 P0, P1, P2, P3 값이 있다고 가정하면 다음과 같이 표현할 수 있습니다.

$$
f(0) = P0, f(1) = P1, f'(0) = (P2 - P0) / 2, f'(1) = (P3 - P1) / 2
$$

이를 a, b, c, d에 각각 적용하면 다음과 같이 나타낼 수 있습니다.

![](../.gitbook/assets/image%20%2815%29.png)

따라서, 최종적으로 다음과 같은 식을 이용하여 보간을 할 수 있습니다.

$$
f(P0, P1, P2, P3, x) = (-0.5P0 + 1.5P1 -1.5P2 +0.5P3)x^3 + (P0 -2.5P1 + 2P2 -0.5P3)x^2 + (-0.5P0 + 0.5P2)x + P1
$$

## Bicubic Interpolation

위에서 배운 cubic Interpolation을 확장하여 

보간하려는 점 근처의 16개의 점으로 5번의 Cubic Interpolation을 통해 구하는 방법입니다.

![](../.gitbook/assets/image%20%2846%29.png)

