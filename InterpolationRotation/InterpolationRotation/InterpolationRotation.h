#pragma once
#include <iostream>
#include "NeedFunction.h"

using namespace std;

//H1 - 1 = Bilinear Interpolation
void BilinearInterpolation(double ScaleRate);
unsigned char Interpolation(unsigned char** Input, int nHeight, int nWidth, double h_Cvt, double w_Cvt);

//H1 -2 = Rotation
void Rotation(int RotationAngle);