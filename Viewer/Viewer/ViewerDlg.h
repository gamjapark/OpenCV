
// ViewerDlg.h: 헤더 파일
//


#pragma once
#include <opencv2/opencv.hpp>
using namespace cv;

#include "RawInfoDlg.h"
#include "ImgScalingDlg.h"
#include "RotationDlg.h"
#include "FilterDlg.h"
#include "AddNoiseDlg.h"
#include "MeanFilterDlg.h"
#include "OrderStatisticsDlg.h"
#include "PSNRDlg.h"



// CViewerDlg 대화 상자
class CViewerDlg : public CDialogEx
{
// 생성입니다.
	enum NoiseFilterTpye {
		NF_ARITHMETIC,
		NF_GEOMETRIC,
		NF_HARMONIC,
		NF_CONTRAHARMONIC,
		NF_MEDIAN,
		NF_ADAPTIVEMEDIAN
	};

	enum ImageFormat {
		FORMAT_RGB,
		FORMAT_YUV444,
		FORMAT_YUV420,
		FORMAT_GRAY
	};
	enum InterpolationType
	{
		BILINEAR,
		BICUBIC
	};
	enum FilterType {
		FILTER_AVERAGE,
		FILTER_SMOOTH,
		FILTER_SHARPEN,
		FILTER_GAUSSIAN
	};
	enum NoiseType
	{
		NOISE_GN,
		NOISE_SP
	};
public:
	CViewerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEWER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	unsigned char **ch_in_1, **ch_in_2, **ch_in_3;
	unsigned char **ch_in_gray;

	int *Hist;

	int nHeight_in, nWidth_in;
	int nFormat;
	bool isImageOpened;

	CString InpFileName;

public:
	CStatic m_PicOri;
	CStatic m_PicProc;
	CRawInfoDlg RawInfoDlg;
	CImgScalingDlg imgscaledlg;
	CRotationDlg RotationDlg;

	Mat Ori_Img, Proc_Img;
	CRect rect;
	CDC* m_pDC;

	void DisplayImage(Mat DispMat, bool binOut);
	void DisplayBitmap(CDC* pDC, CRect rect, IplImage* DisplplImage);
	void ResizeImage(IplImage* src, IplImage ** dst, CRect* rect);
	afx_msg void OnFileOpenBtnClick();
	afx_msg void OnCloseMenuClick();
	afx_msg void OnConvertgrayBtnClick();
	afx_msg void OnEditScalingClick();
	afx_msg void OnHistogramGetClick();
	afx_msg void OnHistogramEqualizationClicked();
	afx_msg void OnHistogramMatchingClicked();
	afx_msg void OnEditRotationClick();
	afx_msg void OnEditFilterClicked();
	afx_msg void OnNoisereductionAddnoiseClicked();
	afx_msg void OnNoisefilteringMeanfilterClicked();
	afx_msg void OnNoisefilteringOrderstatisticfilter();
	afx_msg void OnGetpsnr();
};

unsigned char** MemAlloc2D(int  nHeight, int nWidth, unsigned char nInitVal);
void MemFree2D(unsigned char** Mem, int nHeight);

unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);

bool isInsideBoundary(int nHeight, int nWidth, double h, double w);

void DisplayHistogram(int * Hist);

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize);
void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize);
void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize = 3);
void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize = 3);
void GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterGauss, int FilterSize = 3);

Mat GrayToMat(unsigned char ** Img, int nHeight, int nWidth);

void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev);
void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb);

void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev);
float GetNoise(float* PDF, int nLength);


void ArithmeticMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void GeometricMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void HarmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void ContraharmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q);

void MedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize);
void AdaptiveMedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize_min, int nFilterSize_Max);

