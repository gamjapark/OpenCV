

// Gamja Comment
// ViewerDlg.cpp / ViewerDlg.hpp
// 1. 대화 상자 클래스인 CViewerDlg Class가 선언되고 정의됨
// 2. GUI 화면을 담는 Resource 파일과 연동
// 3. 이벤트 발생시 멤버 변수로 하여금 처리하게 함

// MFC(Micosoft Foundation Classes)
// 1. Win32 API를 각 기능 별로 클래스화 한 것
// 2. 창, 메뉴, 대화상자 등 관리과 기본 입출력 작업 등에 필요한 코드 제공
// 3. 간단한 데스크톱 응용 프로그램부터 복잡한 사용자 인터페이스 개발에 유용
// 4. Handle
//		1) 어떤 대상에 붙여진 Lable 같은 것 - 대상 식별에 사용
//		2) 사용자가 조작할 타겟(윈도우, 컨트롤 등) 관리
//		3) 운영체제가 발급
//		4) 사용자는 할당된 핸들 사용
// 5. Message Loop
//		1) 입력으로 임의의 이벤트 발생시, 즉각 반응 처리
//		2) 발생된 이벤트를 Message Loop에서 감지
//		3) 해당 메시지를 윈도우 procedure 함수로 전달
//		4) 과정 
//			Input  ->  OS (System Que)  ->  | Application Que  -> WinMain() (Message Loop)  -> WndProc() (Window Procedure) |
//                                          |								응용 프로그램                                    |
// 6. 용량 줄이기 
//		1) 빌드 -> 솔루션 정리 
//		2) 불필요한 폴더 삭제 : ipch 폴더 삭제


// ViewerDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewerDlg 대화 상자



CViewerDlg::CViewerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEWER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE_IN, m_PicOri);
	DDX_Control(pDX, IDC_PICTURE_OUT, m_PicProc);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_FILE_FILEOPEN, &CViewerDlg::OnFileOpenBtnClick)
	ON_COMMAND(ID_FILE_FILECLOSE, &CViewerDlg::OnCloseMenuClick)
	ON_COMMAND(ID_EDIT_CONVERT, &CViewerDlg::OnConvertgrayBtnClick)
	ON_COMMAND(ID_EDIT_SCALING, &CViewerDlg::OnEditScalingClick)
	ON_COMMAND(ID_HISTOGRAM_GETHISTOGRAM, &CViewerDlg::OnHistogramGetClick)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMEQUALIZATION, &CViewerDlg::OnHistogramEqualizationClicked)
	ON_COMMAND(ID_HISTOGRAM_HISTOGRAMMATCHING, &CViewerDlg::OnHistogramMatchingClicked)
END_MESSAGE_MAP()


// CViewerDlg 메시지 처리기

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CViewerDlg::DisplayImage(Mat DispMat, bool bInOut)
{
	if (bInOut)
	{
		GetDlgItem(IDC_PICTURE_IN)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicOri.GetDC();
		m_PicOri.GetClientRect(&rect);
	}
	else
	{
		GetDlgItem(IDC_PICTURE_OUT)->GetWindowRect(rect);
		ScreenToClient(rect);
		m_pDC = m_PicProc.GetDC();
		m_PicProc.GetClientRect(&rect);
	}

	IplImage* Ori = &IplImage(DispMat);
	IplImage* Resize = NULL;

	ResizeImage(Ori, &Resize, &rect);
	DisplayBitmap(m_pDC, rect, Resize);
}

void CViewerDlg::DisplayBitmap(CDC* pDC, CRect rect, IplImage* DisplplImage)
{
	BITMAPINFO bitmapInfo;
	bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfo.bmiHeader.biPlanes = 1;
	bitmapInfo.bmiHeader.biCompression = BI_RGB;
	bitmapInfo.bmiHeader.biXPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biYPelsPerMeter = 100;
	bitmapInfo.bmiHeader.biClrUsed = 0;
	bitmapInfo.bmiHeader.biClrImportant = 0;
	bitmapInfo.bmiHeader.biSizeImage = 0;
	bitmapInfo.bmiHeader.biWidth = DisplplImage->width;
	bitmapInfo.bmiHeader.biHeight = DisplplImage->height;

	if (DisplplImage->nChannels == 3) {
		IplImage* ReverseImg = cvCreateImage(CvSize(DisplplImage->width, DisplplImage->height), 8, 3);

		for (int h = 0; h < DisplplImage->height; h++) {
			for (int w = 0; w < DisplplImage->width; w++) {
				ReverseImg->imageData[h * DisplplImage->widthStep + w * 3] = DisplplImage->imageData[(DisplplImage->height - h)*DisplplImage->widthStep + w * 3];
				ReverseImg->imageData[h * DisplplImage->widthStep + w * 3 + 1] = DisplplImage->imageData[(DisplplImage->height - h)*DisplplImage->widthStep + w * 3 + 1];
				ReverseImg->imageData[h * DisplplImage->widthStep + w * 3 + 2] = DisplplImage->imageData[(DisplplImage->height - h)*DisplplImage->widthStep + w * 3 + 2];
			}
		}
		bitmapInfo.bmiHeader.biBitCount = DisplplImage->depth*DisplplImage->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom,
			0, 0, DisplplImage->width, DisplplImage->height, ReverseImg->imageData, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
	}
	else
	{
		IplImage* ReverseImg = cvCreateImage(cvGetSize(DisplplImage), DisplplImage->depth, 1);

		for (int h = 0; h < DisplplImage->height; h++) 
			for (int w = 0; w < DisplplImage->width; w++) 
				ReverseImg->imageData[h * DisplplImage->widthStep + w] = DisplplImage->imageData[(DisplplImage->height - h)*DisplplImage->widthStep + w];

		IplImage* tmp = cvCreateImage(cvGetSize(DisplplImage), DisplplImage->depth, 3);
		cvCvtColor(ReverseImg, tmp, CV_GRAY2BGR);

		bitmapInfo.bmiHeader.biBitCount = tmp->depth*tmp->nChannels;
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(), rect.left, rect.top, rect.right, rect.bottom,
			0, 0, tmp->width, tmp->height, tmp->imageData, &bitmapInfo,
			DIB_RGB_COLORS, SRCCOPY);

		cvReleaseImage(&ReverseImg);
		cvReleaseImage(&tmp);
	}
}
void CViewerDlg::ResizeImage(IplImage* src, IplImage ** dst, CRect* rect)
{
	int ndst_width = 0, ndst_height = 0;

	if (src->width > src->height) {
		ndst_width = rect->Width();
		ndst_height = (src->height * rect->Width()) / src->width;
	}
	else
	{
		ndst_width = (src->width * rect->Height()) / src->height;
		ndst_height = rect->Height();
	}

	(*dst) = cvCreateImage(cvSize(ndst_width, ndst_height), IPL_DEPTH_8U, src->nChannels);

	cvResize(src, (*dst));
	rect->right = rect->left + ndst_width;
	rect->bottom = rect->top + ndst_height;
}

unsigned char** MemAlloc2D(int  nHeight, int nWidth, unsigned char nInitVal)
{
	unsigned char** rtn = new unsigned char*[nHeight];
	for (int n = 0; n < nHeight; n++)
	{
		rtn[n] = new unsigned char[nWidth];
		memset(rtn[n], nInitVal, sizeof(unsigned char) * nWidth);
	}

	return rtn;
}

void MemFree2D(unsigned char** Mem, int nHeight)
{
	for (int n = 0; n < nHeight; n++) {
		delete[] Mem[n];
	}
	delete[] Mem;
}

void CViewerDlg::OnFileOpenBtnClick()
{
	CFileDialog dlg(TRUE);

	if (isImageOpened) {
		if (ch_in_1 != NULL)
			MemFree2D(ch_in_1, nHeight_in);
		if (ch_in_2 != NULL) {
			if (nFormat == FORMAT_YUV420)	MemFree2D(ch_in_2, nHeight_in / 2);
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL) {
			if (nFormat == FORMAT_YUV420)	MemFree2D(ch_in_3, nHeight_in / 2);
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL)
			MemFree2D(ch_in_gray, nHeight_in);
	}

	if (dlg.DoModal() == IDOK) {
		if (dlg.GetFileExt() != "jpg" && dlg.GetFileExt() != "JPG" && dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "yuv")
		{
			MessageBox("JPG 또는 YUV, Gray 파일이 아닙니다.");
			return;
		}

		if (dlg.GetFileExt() == "jpg" || dlg.GetFileExt() == "JPG")
		{
			Ori_Img = imread((const char*)dlg.GetPathName());
			nHeight_in = Ori_Img.rows;
			nWidth_in = Ori_Img.cols;

			if (Ori_Img.channels() == 3) {
				ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
				ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						Vec3b RGB = Ori_Img.at<Vec3b>(h, w);
						ch_in_1[h][w] = RGB[2];
						ch_in_2[h][w] = RGB[1];
						ch_in_3[h][w] = RGB[0];
					}
				}
				
				nFormat = FORMAT_RGB;
			}
			else {
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						ch_in_gray[h][w] = Ori_Img.at<unsigned char>(h, w);
					}
				}

				nFormat = FORMAT_GRAY;
			}
			isImageOpened = true;
			DisplayImage(Ori_Img, true);
		}
		else {
			CRawInfoDlg RawInfoDlg;

			if (RawInfoDlg.DoModal() == IDOK) {
				nHeight_in = RawInfoDlg.GetRawHeight();
				nWidth_in = RawInfoDlg.GetRawWidth();
				nFormat = RawInfoDlg.GetRawFormat();

				if (nHeight_in == 0 || nWidth_in == 0) {
					MessageBox("값을 잘못 입력하였습니다.");
					return;
				}
				
				FILE *fp_in;

				fopen_s(&fp_in, (const char *)dlg.GetPathName(), "rb");

				if (nFormat == FORMAT_GRAY) {
					ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);

					for (int h = 0; h < nHeight_in; h++) {
						fread(ch_in_gray[h], sizeof(unsigned char), nWidth_in, fp_in);
					}
					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Ori_Img.at<uchar>(h, w) = ch_in_gray[h][w];
						}
					}
				}
				else if (nFormat == FORMAT_YUV444) {
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_3 = MemAlloc2D(nHeight_in, nWidth_in, 0);

					for (int h = 0; h < nHeight_in; h++)
						fread(ch_in_1[h], sizeof(uchar), nWidth_in, fp_in);
					for (int h = 0; h < nHeight_in; h++)
						fread(ch_in_2[h], sizeof(uchar), nWidth_in, fp_in);
					for (int h = 0; h < nHeight_in; h++)
						fread(ch_in_3[h], sizeof(uchar), nWidth_in, fp_in);

					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC3);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Ori_Img.at<Vec3b>(h, w)[2] = ch_in_1[h][w] + 1.402 * (ch_in_3[h][w] - 128);
							Ori_Img.at<Vec3b>(h, w)[1] = ch_in_1[h][w] - 0.344 * (ch_in_2[h][w] - 128) - 0.714 * (ch_in_3[h][w] - 128);
							Ori_Img.at<Vec3b>(h, w)[0] = ch_in_1[h][w] + 1.772 * (ch_in_2[h][w] - 128);

						}
					}
				}
				else {
					ch_in_1 = MemAlloc2D(nHeight_in, nWidth_in, 0);
					ch_in_2 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);
					ch_in_3 = MemAlloc2D(nHeight_in / 2, nWidth_in / 2, 0);

					for (int h = 0; h < nHeight_in; h++)
						fread(ch_in_1[h], sizeof(uchar), nWidth_in, fp_in);
					for (int h = 0; h < nHeight_in / 2; h++)
						fread(ch_in_2[h], sizeof(uchar), nWidth_in / 2, fp_in);
					for (int h = 0; h < nHeight_in / 2; h++)
						fread(ch_in_3[h], sizeof(uchar), nWidth_in / 2, fp_in);

					Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC3);
					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Ori_Img.at<Vec3b>(h, w)[2] = ch_in_1[h][w] + 1.402 * (ch_in_3[h / 2][w / 2] - 128);
							Ori_Img.at<Vec3b>(h, w)[1] = ch_in_1[h][w] - 0.344 * (ch_in_2[h / 2][w / 2] - 128) - 0.714 * (ch_in_3[h / 2][w / 2] - 128);
							Ori_Img.at<Vec3b>(h, w)[0] = ch_in_1[h][w] + 1.772 * (ch_in_2[h / 2][w / 2] - 128);

						}
					}
				}
				
				fclose(fp_in);
				isImageOpened = true;
				DisplayImage(Ori_Img, true);
			}
		}

	}
}


void CViewerDlg::OnCloseMenuClick()
{
	if (isImageOpened)
	{
		if (ch_in_1 != NULL) MemFree2D(ch_in_1, nHeight_in);
		if (ch_in_2 != NULL)
		{
			if (nFormat == FORMAT_YUV420) MemFree2D(ch_in_2, nHeight_in / 2);
			else MemFree2D(ch_in_2, nHeight_in);
		}
		if (ch_in_3 != NULL)
		{
			if (nFormat == FORMAT_YUV420) MemFree2D(ch_in_3, nHeight_in / 2);
			else MemFree2D(ch_in_3, nHeight_in);
		}
		if (ch_in_gray != NULL) MemFree2D(ch_in_gray, nHeight_in);
	}

	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


void CViewerDlg::OnConvertgrayBtnClick()
{

	if (isImageOpened) {
		if (nFormat == FORMAT_GRAY) {
			MessageBox("입력 영상이 회색조 영상입니다.");
			return;
		}
		if (nFormat == FORMAT_YUV444 || nFormat == FORMAT_YUV420)
		{
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_1[h][w];
				}
			}
			DisplayImage(Proc_Img, false);
			return;
		}
		if (nFormat == FORMAT_RGB) {
			if (ch_in_gray == NULL) {
				ch_in_gray = MemAlloc2D(nHeight_in, nWidth_in, 0);
			}
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					ch_in_gray[h][w] = saturate_cast<unsigned char>(0.299 *ch_in_1[h][w] + 0.587 * ch_in_2[h][w] + 0.114 * ch_in_3[h][w]);
				}
			}
			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Proc_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			DisplayImage(Proc_Img, false);
			return;
		}
	}
	else {
		MessageBox("불러온 영상이 없습니다.");
	}
}



void CViewerDlg::OnEditScalingClick()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (ImgScalingDlg.DoModal() == IDOK)
		{
			int nInterpolationType = ImgScalingDlg.GetInterpType();
			double dScaleRate = ImgScalingDlg.GetScaleRate();

			if (dScaleRate < 1 || dScaleRate > 2)
			{
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}

			int nHeight_Scale = nHeight_in * dScaleRate;
			int nWidth_Scale = nWidth_in * dScaleRate;

			unsigned char** Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);

			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++)
				{
					double h_Ori = h / dScaleRate;
					double w_Ori = w / dScaleRate;
					if (nInterpolationType == BILINEAR) Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
					else Out[h][w] = BicubicInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
				}
			}

			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					Ori_Img.at<unsigned char>(h, w) = ch_in_gray[h][w];
				}
			}

			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++) {
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];
				}
			}

			imshow("Original Image", Ori_Img);
			imshow("Scaled Image", Proc_Img);
			waitKey(0);

		}
	}
}



//bool isInsideBoundary(int nHeight, int nWidth, double h, double w);
unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, double nWidth_Ori, double h_Cvt, double w_Cvt)
{
	//if(h1 == nHeight_Ori - 1 && w1 == )
	return 0;
}
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, double nWidth_Ori, double h_Cvt, double w_Cvt)
{
	return 0;
}

void CViewerDlg::OnHistogramGetClick()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);
		for (int h = 0; h < nHeight_in; h++)
			for (int w = 0; w < nWidth_in; w++) Hist[ch_in_gray[h][w]]++;

		int nMax = 0;
		for (int n = 0; n < 256; n++) if (nMax < Hist[n]) nMax = Hist[n];

		double dNormalizeFactor = 255.0 / nMax;
		Mat HistDisp = Mat(256, 256, CV_8UC1);
		HistDisp = Scalar::all(0);

		for (int w = 0; w < 256; w++)
		{
			int nNormalizedValue = (int)Hist[w] * dNormalizeFactor;
			for (int h = 255; h > 255 - nNormalizedValue; h-- ) 
				HistDisp.at<unsigned char>(h, w) = 255;
		}

		imshow("Histogram of Input Image", HistDisp);
		waitKey(0);

	}
}


void CViewerDlg::OnHistogramEqualizationClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}
		/////////////////////////////////////////////////////////////////////
		double Hist_CDF[256] = { 0.0 };
		double out[256] = { 0 };
		Mat Eq_Img = Mat(256, 256, CV_8UC1);

		double remember = 0;
		for (int i = 0; i < 256; i++) {
			if (Hist[i] != 0) {
				remember += Hist[i];
				Hist_CDF[i] = (double)(remember) / (nHeight_in * nWidth_in);	
				out[i] = round(Hist_CDF[i] * 255);
			}
		}

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Eq_Img.at<unsigned char>(h, w) = out[ch_in_gray[h][w]];
			}
		}
		/////////////////////////////////////////////////////////////////////
	
		int *Hist_Eq = new int[256];
		memset(Hist_Eq, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist_Eq[Eq_Img.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(Eq_Img, false);
		DisplayHistogram(Hist);
		DisplayHistogram(Hist_Eq);

		delete[] Hist_Eq;
	}
}

void DisplayHistogram(int * Hist)
{
	int nMax = 0;

	for (int n = 0; n < 256; n++) {
		if (nMax < Hist[n]) nMax = Hist[n];
	}

	double dNormalizeFactor = 255.0 / nMax;

	Mat HistDisp = Mat(256, 256, CV_8UC1);
	HistDisp = Scalar::all(0);

	for (int w = 0; w < 256; w++) {
		int nNormalizeValue = (int)Hist[w] * dNormalizeFactor;
		for (int h = 255; h > 255 - nNormalizeValue; h--) {
			HistDisp.at<unsigned char>(h, w) = 255;
		}
	}
	imshow("Histogram", HistDisp);
	waitKey(0);
}


void CViewerDlg::OnHistogramMatchingClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);

		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Hist[ch_in_gray[h][w]]++;
			}
		}
		Mat MatchImg(256, 256, CV_8UC1), RefImg(nHeight_in, nWidth_in, CV_8UC1);
		int Hist_Ref[256] = { 0 }, Hist_Mat[256] = { 0 };
		float Hist_CDF[256], Hist_CDF_Ref[256];

		MatchImg = Scalar::all(0);
		RefImg = Scalar::all(0);

		srand(GetTickCount());

		for (int h = 0; h < RefImg.rows; h++) {
			for (int w = 0; w < RefImg.cols; w++) {
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}

		Hist_CDF[0] = (double)Hist[0] / (nHeight_in *nWidth_in);
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in *nWidth_in);

		for (int n = 1; n < 256; n++) {
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in *nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in *nWidth_in) + Hist_CDF_Ref[n - 1];

 		}
		for (int i = 0; i < 256; i++) {
			for (int n = 0; n < 256; n++) {
				if (Hist_CDF_Ref[n] == Hist_CDF[i])
				{
					Hist_Mat[n] = Hist_CDF_Ref[n];
				}
			}
		}

		for (int h = 0; h < MatchImg.rows; h++) {
			for (int w = 0; w < MatchImg.cols; w++) {
				MatchImg.at<unsigned char>(h, w) = Hist_Mat[ch_in_gray[h][w]];
			}
		}

		DisplayImage(MatchImg, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);

	}
}
