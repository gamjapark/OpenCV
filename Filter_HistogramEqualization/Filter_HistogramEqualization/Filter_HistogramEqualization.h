#pragma once
#include <iostream>
#include <sstream>
#include "NeedFunction.h"

using namespace std;

//Histogram Equlization
void HistogramEqualization();

//Average Filter
void SpatialFilter_Average(int nFilterSize);
void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize);

//Smooth Filter
void SpatialFilter_Smooth();
void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize);

// Sharpening Filter
void SpatialFilter_Sharpen();
void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize);

//Median Filter
void SpatialFilter_Median(int FilterSize, const char *FileName);
void MedianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize);



