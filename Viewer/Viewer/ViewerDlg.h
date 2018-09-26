
// ViewerDlg.h: 헤더 파일
//
#include <opencv2/opencv.hpp>
#include "RawInfoDlg.h"
#include "ImgScalingDlg.h"
#include "RotationDlg.h"

using namespace cv;

#pragma once


// CViewerDlg 대화 상자
class CViewerDlg : public CDialogEx
{
// 생성입니다.

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
};

unsigned char** MemAlloc2D(int  nHeight, int nWidth, unsigned char nInitVal);

void MemFree2D(unsigned char** Mem, int nHeight);
void DisplayHistogram(int * Hist);

unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt);

bool isInsideBoundary(int nHeight, int nWidth, double h, double w);