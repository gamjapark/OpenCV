

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
#include <algorithm>


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
	ON_COMMAND(ID_EDIT_ROTATION, &CViewerDlg::OnEditRotationClick)
	ON_COMMAND(ID_Menu, &CViewerDlg::OnEditFilterClicked)
	ON_COMMAND(ID_NOISEREDUCTION_ADDNOISE, &CViewerDlg::OnNoisereductionAddnoiseClicked)
	ON_COMMAND(ID_NOISEFILTERING_MEANFILTER, &CViewerDlg::OnNoisefilteringMeanfilterClicked)
	ON_COMMAND(ID_NOISEFILTERING_ORDERSTATISTICFILTER, &CViewerDlg::OnNoisefilteringOrderstatisticfilter)
	ON_COMMAND(ID_NOISEREDUCTION_GETPSNR, &CViewerDlg::OnGetpsnr)
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
unsigned char** MemAlloc2D(int nHeight, int nWidth, unsigned char nInitVal)
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
		InpFileName = dlg.GetFileTitle();
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

					for (int h = 0; h < nHeight_in; h++) fread(ch_in_1[h], sizeof(uchar), nWidth_in, fp_in);
					for (int h = 0; h < nHeight_in; h++) fread(ch_in_2[h], sizeof(uchar), nWidth_in, fp_in);
					for (int h = 0; h < nHeight_in; h++) fread(ch_in_3[h], sizeof(uchar), nWidth_in, fp_in);

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

	// 영상이 열려 있을 때 && 영상 포맷이 Gray (1 Channel)일 때만 실행
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		// 스케일링 정보를 받아옴
		if (imgscaledlg.DoModal() == IDOK)
		{
			int nInterpolationType = imgscaledlg.GetInterpType();	//보간 타입
			double dScaleRate = imgscaledlg.GetScaleRate();		// 스케일링 비율


			if (dScaleRate < 1 || dScaleRate > 2) //스케일링 비율이 잘못되었을 경우, 범위를 벗어났을 경우 실행X
			{
				MessageBox("스케일링 범위가 잘못되었습니다.");
				return;
			}
			//Scaling 후 영상의 크기 계산
			int nHeight_Scale = nHeight_in * dScaleRate;
			int nWidth_Scale = nWidth_in * dScaleRate;

			//스케일링 결과를 담을 2차원 unsigned char 형 배열
			unsigned char** Out = MemAlloc2D(nHeight_Scale, nWidth_Scale, 0);

			unsigned char** Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 5);
			
			for (int h = 0; h < nHeight_Scale; h++) {
				for (int w = 0; w < nWidth_Scale; w++)
				{
					double h_Ori = h / dScaleRate;
					double w_Ori = w / dScaleRate;
					if (nInterpolationType == BILINEAR) Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Ori, w_Ori);
					else {
						Out[h][w] = BicubicInterpolation(Pad, nHeight_in, nWidth_in, h_Ori, w_Ori);
					}
				}
			}

			
			Ori_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
			Proc_Img = Mat(nHeight_Scale, nWidth_Scale, CV_8UC1);
			//MessageBox("aaaa");

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
			MemFree2D(Pad, nHeight_in + 2);
			
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}



// * Interpolation
//		1. 불연속적인 데이터 점들을 이용하여 그 점들 사이의 값을 추정하는 방법
//		2. nearest-neighbor interpolation =  보간 하려는 값의 index에서 가장 가까이 있는 값을 대입
//		3. Linear Interpoloation =  보간하려는 점 양쪽의 두점을 이용해 값을 구함 (두 점이 일직선상에 있다고 가정)
//		4. Bilinear Interpolation = Linear Interpolation의 확장 -> 보간하려는 점 근처의 네 점을 이용하여 값 구함
//		5. cubic Interpolation = 보간하려는 점 양쪽의 두 픽셀, 총 4개의 픽셀을 이용해 값 구함 -> 4개 픽셀이 3차 곡선상에 있다고 가정

unsigned char BilinearInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{
	int H[2], W[2];
	H[0] = floor(h_Cvt);
	W[0] = floor(w_Cvt);

	double interval_w = w_Cvt - W[0], interval_h = h_Cvt - H[0];

	//가장 자리를 벗어나는 것에 대한 예외처리 -> 인접값 반복
	if (H[0] == nHeight_Ori - 1 && W[0] == nWidth_Ori - 1) {
		H[1] = nHeight_Ori - 1;
		W[1] = nWidth_Ori - 1;
	}
	else if (H[0] == nHeight_Ori - 1 && W[0] != nWidth_Ori - 1) {
		H[1] = nHeight_Ori - 1;
		W[1] = W[0] + 1;
	}
	else if (H[0] != nHeight_Ori - 1 && W[0] == nWidth_Ori - 1) {
		H[1] = H[0] + 1;
		W[1] = nWidth_Ori - 1;
	}
	else {
		H[1] = H[0] + 1;
		W[1] = W[0] + 1;
	}
	unsigned char result[2];
	result[0] = (int)((In[H[0]][W[0]] * (1 - interval_w)) + (In[H[0]][W[1]] * interval_w));
	result[1] = (int)((In[H[1]][W[0]] * (1 - interval_w)) + (In[H[1]][W[1]] * interval_w));

	return (unsigned char)((result[0] * (1 - interval_h)) + (result[1] * interval_h));
}
double CubicInterpolate(double p0, double p1, double p2, double p3, double x)
{
	return p1 + 0.5 * x*(p2 - p0 + x * (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 + x * (3.0*(p1 - p2) + p3 - p0)));
}
unsigned char BicubicInterpolation(unsigned char** In, int nHeight_Ori, int nWidth_Ori, double h_Cvt, double w_Cvt)
{

	int H[4], W[4];
	H[1] = floor(h_Cvt);
	W[1] = floor(w_Cvt);

	for (int i = -1; i <= 2; i++) {
		H[i + 1] = H[1] + i;
		W[i + 1] = W[1] + i;
	}

	double result[4];
	result[0] = CubicInterpolate(In[H[0] + 2][W[0] + 2], In[H[0] + 2][W[1] + 2], In[H[0] + 2][W[2] + 2], In[H[0] + 2][W[3] + 2], w_Cvt - W[1]);
	result[1] = CubicInterpolate(In[H[1] + 2][W[0] + 2], In[H[1] + 2][W[1] + 2], In[H[1] + 2][W[2] + 2], In[H[1] + 2][W[3] + 2], w_Cvt - W[1]);
	result[2] = CubicInterpolate(In[H[2] + 2][W[0] + 2], In[H[2] + 2][W[1] + 2], In[H[2] + 2][W[2] + 2], In[H[2] + 2][W[3] + 2], w_Cvt - W[1]);
	result[3] = CubicInterpolate(In[H[3] + 2][W[0] + 2], In[H[3] + 2][W[1] + 2], In[H[3] + 2][W[2] + 2], In[H[3] + 2][W[3] + 2], w_Cvt - W[1]);
	return CubicInterpolate(result[0], result[1], result[2], result[3], h_Cvt - H[1]);

}

// * Image Rotation 
//		1. 정해진 각도 만큼 영상 회전
//		2. 회전 변환 공식을 이용해 각 픽셀의 위치를 변환
//		3. 픽셀 좌표는 모두 정수형
//			1) 회전변환 시 발생하는 소수 값의 손실 대문에 정확한 픽셀 값을 대입할 수 없음
//			2) Interpolation을 통해 픽셀 값 보정

void CViewerDlg::OnEditRotationClick()
{

	//영상이 열려 있을 때 & 영상 포맷이 Gray (1 channel)일 때만 실행
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (RotationDlg.DoModal() == IDOK) {	
			//회전 정보 받아옴
			int nInterpolationType = RotationDlg.GetInterpolationType();	//보간 타입
			int nAngle = RotationDlg.GetAngle();							//회전 각도

			if (nAngle < 0 || nAngle > 360) {	//회전 각도 범위가 잘못 되었을 경우 실행하지 않음
				MessageBox("회전 범위가 잘못되었습니다.");
				return;
			}
			//회전 결과를 담을 2차원 unsigned char 형 배열
			unsigned char ** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);
			
			int nHeight_2 = nHeight_in / 2;
			int nWidth_2 = nWidth_in / 2;
			double h_Rotate = 0, w_Rotate = 0;

			unsigned char** Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, 5);

			for (int h = 0; h < nHeight_in; h++) {
				for (int w = 0; w < nWidth_in; w++) {
					double PI = 3.14159265358979323846;
					//회전한 좌표 계산
					double h_origin = ((nHeight_in - 1) - h) - (nHeight_2 - 1);
					double w_origin = w - (nWidth_2 - 1);

					w_Rotate = ((w_origin * cos(nAngle * PI / 180)) + (h_origin * sin(nAngle * PI / 180))) + (nWidth_2 - 1);
					h_Rotate = (h_origin * cos(nAngle * PI / 180)) - (w_origin * sin(nAngle * PI / 180)) + (nHeight_2 - 1);
					h_Rotate = (nHeight_in - 1) - h_Rotate;
					
					if (isInsideBoundary(nHeight_in, nWidth_in, h_Rotate, w_Rotate)) {
						if (nInterpolationType == BILINEAR) Out[h][w] = BilinearInterpolation(ch_in_gray, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
						else Out[h][w] = BicubicInterpolation(Pad, nHeight_in, nWidth_in, h_Rotate, w_Rotate);
					}
				}
			}

			Proc_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);

			for (int h = 0; h < nHeight_in; h++)
				for (int w = 0; w < nWidth_in; w++)
					Proc_Img.at<unsigned char>(h, w) = Out[h][w];

			DisplayImage(Proc_Img, false);
			MemFree2D(Out, nHeight_in);
		}
	}
	else {
		MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
	}
}

// 해당 좌표가 영상 내부에 있으면 true, 영상 외부에 있으면 false를 return
bool isInsideBoundary(int nHeight, int nWidth, double h, double w) {
	if (h >= 0 && w >= 0 && h < nHeight && w < nWidth) return true;
	else return false;
}
// * Histogram
//		1. 영상 전체의 픽셀 값의 발생 빈도를 나타낸 그래프
//		2. 가로죽 : 픽셀값(0 ~ 255) / 세로축 : 픽셀 값의 개수(발생 빈도)
// * Gamma Correction
//		1. 영상의 밝기 값을 비선형(Non-Linear)적으로 변형하는 것
//		2. 정해진 정보량 내에서 화질을 향상시키기 위함
//		3. 베버의 법칙(Weber's Law)
//			1) 인간의 시각은 밝기에 따라 비선형적으로 반응
//			2) 어두운 부분에서의 밝기 변화에는 민감
//			3) 밝기 부분에서의 밝기 변화에는 둔감
//		4. 모니터의 비선형적 출력 보정에 사용
//		5. 입력을 비선형적으로 입력하여 출력을 보정


void CViewerDlg::OnHistogramGetClick()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);
		//히스토그램 얻기
		for (int h = 0; h < nHeight_in; h++)
			for (int w = 0; w < nWidth_in; w++) Hist[ch_in_gray[h][w]]++;
		
		//히스토그램 dispaly
		DisplayHistogram(Hist);
	}
}

// * Histogram Equalization
//		1. 히스토그램을 이용, 영상의 명암을 전체적으로 고르게 분포
//		2. 명암대비 향상시키는 효과
//		3. 히스토그램을 통하여 얻을 수 있는 cdf(누적 분포 함수)를 이용
//		4. 히스토그램을 좌우로 확장
//		5. 과정 
//			1) 영상의 히스토그램을 구함
//			2) 히스토그램의 누적 합을 [0, 1]로 정규화 한 cdf를 구함
//				-> cdf(j) = 씨그마(0 ~ j) Hist / (Height * Width) : j = 픽셀 값
//				-> out(j) = round( cdf(j) * 255 )

void CViewerDlg::OnHistogramEqualizationClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);
		
		//히스토그램 얻기
		for (int h = 0; h < nHeight_in; h++) 
			for (int w = 0; w < nWidth_in; w++) Hist[ch_in_gray[h][w]]++;

		double Hist_CDF[256] = { 0.0 };			//Cumulative Distribution Function
		double out[256] = { 0 };
		Mat Eq_Img = Mat(256, 256, CV_8UC1);	//Mat 객체 생성

		double remember = 0;
		for (int i = 0; i < 256; i++) {
			if (Hist[i] != 0) {
				remember += Hist[i];				//히스토그램의 누적 합
				Hist_CDF[i] = (double)(remember) / (nHeight_in * nWidth_in);	//cdf 구하기
				out[i] = round(Hist_CDF[i] * 255);	//cdf값을 scaling-> 픽셀값으로 만듬

			}
		}

		//Histogram Equalization을 한 영상 생성
		for (int h = 0; h < nHeight_in; h++) {
			for (int w = 0; w < nWidth_in; w++) {
				Eq_Img.at<unsigned char>(h, w) = out[ch_in_gray[h][w]];
			}
		}

		//Histogram Equalization을 한 영상의 히스토그램 얻기
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

// * Histogram Matching
//		1. 원본 Histogram의 cdf를 Reference 영상의 CDF를 통해 변화
//		2. 영상의 일부 영역의 대조를 개선할 수 있음
//		3. 과정
//			1) 입력 영상과 참조 영상의 Histogram을 구함
//			2) Histogram을 이용, 각 영상의 cdf를 계산
//				-> 입력 픽셀 값의 Histogram 찾아서 
//				-> 참조 영상의 Histogram에서 같은 값의 출력 픽셀 값을 찾음
//		3. 균일 분포(uniform distribution)을 가지는 영상

void CViewerDlg::OnHistogramMatchingClicked()
{
	if (isImageOpened && nFormat == FORMAT_GRAY)
	{
		if (Hist == NULL) Hist = new int[256];
		memset(Hist, 0, sizeof(int) * 256);
		//히스토그램 얻기
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

		//참조 영상 생성 및 히스토그램 게산
		srand(GetTickCount());
		for (int h = 0; h < RefImg.rows; h++) {
			for (int w = 0; w < RefImg.cols; w++) {
				RefImg.at<unsigned char>(h, w) = (unsigned char)rand() % 255;
				Hist_Ref[RefImg.at<unsigned char>(h, w)]++;
			}
		}


		//원래 영상과 참조 영상의 cdf 구하기
		Hist_CDF[0] = (double)Hist[0] / (nHeight_in *nWidth_in);			//원본 첫번째
		Hist_CDF_Ref[0] = (double)Hist_Ref[0] / (nHeight_in *nWidth_in);	//참조 첫번
		for (int n = 1; n < 256; n++) {									//원본&참조 나머지
			Hist_CDF[n] = (double)Hist[n] / (nHeight_in *nWidth_in) + Hist_CDF[n - 1];
			Hist_CDF_Ref[n] = (double)Hist_Ref[n] / (nHeight_in *nWidth_in) + Hist_CDF_Ref[n - 1];

		}


		double out[256] = { 0 };
		for (int i = 0; i < 256; i++) {
			double nMin = 1;
			int FindNum = 0;
			for (int j = 0; j < 256; j++) {
				//실제로 가지고 있는 데이터는 연속적인 함수가 아니므로 
				//입력값의 cdf 값과 가장 가까운 참조영상의 cdf값의 픽셀값을 찾아야하는 것!!!
				//최소값을 지속적으로 갱신함으로써 0-255 사이의 픽셀중 가장 최소로 가까운 것을 찾는다! 
				if (Hist_CDF[i] == Hist_CDF_Ref[j]) {
					FindNum = j;
					break;
				}
			    if (abs(Hist_CDF[i] - Hist_CDF_Ref[j]) < nMin) {
					nMin = abs(Hist_CDF[i] - Hist_CDF_Ref[j]);
					FindNum = j;
				}
			}
			out[i] = FindNum;
		}

		//Histogram Matching 과정을 통해 Matching 결과를 Mat에 저장
		for (int h = 0; h < MatchImg.rows; h++) {
			for (int w = 0; w < MatchImg.cols; w++) {
				MatchImg.at<unsigned char>(h, w) = out[ch_in_gray[h][w]];
				Hist_Mat[MatchImg.at<unsigned char>(h, w)]++;
			}
		}

		DisplayImage(MatchImg, false);
		DisplayHistogram(Hist_Ref);
		DisplayHistogram(Hist_Mat);


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


void CViewerDlg::OnEditFilterClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//영상이 열려 있을 때 && 영상 포멧이 Gray(1 channel) 일 때만 실행
	if (isImageOpened && nFormat == FORMAT_GRAY) {
		CFilterDlg FilterDlg;
		unsigned char** Pad, **Out;
		if (FilterDlg.DoModal() == IDOK) {

			int nFilterType = FilterDlg.getFilterType();
			int nFilterSize = FilterDlg.getFilterSize();
			int nFilterGauss = FilterDlg.getFilterGauss();
			switch (nFilterType)
			{
			case FILTER_AVERAGE:
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) nFilterSize++;
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					AverageFilter(Pad, Out, nHeight_in, nWidth_in, nFilterSize);
					DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

					Mat Out_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					Out_Img = Scalar::all(0);

					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Out_Img.at<unsigned char>(h, w) = Out[h][w];
						}
					}
					DisplayImage(Out_Img, false);

					MemFree2D(Pad, nHeight_in + (int)(nFilterSize / 2) * 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter Size는 3이상의 정수 값을 가져야 합니다.");
				break;

			case FILTER_SMOOTH:
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) nFilterSize++;
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					SmoothFilter(Pad, Out, nHeight_in, nWidth_in);
					DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);


					Mat Out_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					Out_Img = Scalar::all(0);

					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Out_Img.at<unsigned char>(h, w) = Out[h][w];
						}
					}
					DisplayImage(Out_Img, false);


					MemFree2D(Pad, nHeight_in + 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter Size는 3이상의 정수 값을 가져야 합니다.");
				break;
			case FILTER_SHARPEN:
				if (nFilterSize >= 3) {
					if (nFilterSize % 2 == 0) nFilterSize++;
					Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
					Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

					SharpenFilter(Pad, Out, nHeight_in, nWidth_in);
					DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);


					Mat Out_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
					Out_Img = Scalar::all(0);

					for (int h = 0; h < nHeight_in; h++) {
						for (int w = 0; w < nWidth_in; w++) {
							Out_Img.at<unsigned char>(h, w) = Out[h][w];
						}
					}
					DisplayImage(Out_Img, false);


					MemFree2D(Pad, nHeight_in + 2);
					MemFree2D(Out, nHeight_in);
				}
				else MessageBox("Filter Size가 잘못되었습니다. Filter Size는 3이상의 정수 값을 가져야 합니다.");

				break;
			case FILTER_GAUSSIAN:
				Pad = Padding(ch_in_gray, nHeight_in, nWidth_in, nFilterSize);
				Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

				GaussianFilter(Pad, Out, nHeight_in, nWidth_in, nFilterGauss, nFilterSize);
				DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

				Mat Out_Img = Mat(nHeight_in, nWidth_in, CV_8UC1);
				Out_Img = Scalar::all(0);

				for (int h = 0; h < nHeight_in; h++) {
					for (int w = 0; w < nWidth_in; w++) {
						Out_Img.at<unsigned char>(h, w) = Out[h][w];
					}
				}
				DisplayImage(Out_Img, false);




				MemFree2D(Pad, nHeight_in + 2);
				MemFree2D(Out, nHeight_in);


				break;
			}
		}
	}
	else MessageBox("불러온 영상이 없거나 영상 포맷이 회색조 영상이 아닙니다.");
}

Mat GrayToMat(unsigned char ** Img, int nHeight, int nWidth) {
	Mat rtn = Mat(nHeight, nWidth, CV_8UC1);
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			rtn.at<unsigned char >(h, w) = Img[h][w];
		}
	}
	return rtn;
}

unsigned char** Padding(unsigned char** In, int nHeight, int nWidth, int nFilterSize)
{
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Pad = MemAlloc2D(nHeight + 2 * nPadSize, nWidth + 2 * nPadSize, 0);
	for (int h = 0; h < nHeight; h++)
		for (int w = 0; w < nWidth; w++)
			Pad[h + nPadSize][w + nPadSize] = In[h][w];

	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nWidth; w++) {
			Pad[h][w + nPadSize] = In[0][w];
			Pad[h + nHeight + nPadSize][w + nPadSize] = In[nHeight - 1][w];
		}
	}

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h + nPadSize][w] = In[h][0];
			Pad[h + nPadSize][w + nWidth + nPadSize] = In[h][nWidth - 1];
		}
	}
	for (int h = 0; h < nPadSize; h++) {
		for (int w = 0; w < nPadSize; w++) {
			Pad[h][w] = In[0][0];
			Pad[h + nHeight + nPadSize][w] = In[nHeight - 1][0];
			Pad[h][w + nWidth + nPadSize] = In[0][nWidth - 1];
			Pad[h + nHeight + nPadSize][w + nWidth + nPadSize] = In[nHeight - 1][nWidth - 1];
		}
	}

	return Pad;
}
void AverageFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0;
			for (int h2 = h; h2 < h + FilterSize; h2++)
				for (int w2 = w; w2 < w + FilterSize; w2++) 
					sum += In_Pad[h2][w2];
			Out[h][w] = sum / (FilterSize * FilterSize);
		}
	}

}
void SmoothFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0.0;
			for (int i = 0; i < FilterSize; i++) {
				for (int j = 0; j < FilterSize; j++) {
					if ((i == 0 || i == FilterSize - 1) && (j == 0 || j == FilterSize - 1))
						sum += In_Pad[h + i][w + j] * 1;
					else if (i == j && i == FilterSize / 2)
						sum += In_Pad[h + i][w + j] * 4;
					else if (i != j && (i == FilterSize / 2 || j == FilterSize / 2))
						sum += In_Pad[h + i][w + j] * 2;
				}
			}
			sum /= 16;
			if (sum < 0) sum = 0;
			else if (sum > 255) sum = 255;
			Out[h][w] = sum;
		}
	}
}
void SharpenFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterSize) {

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double sum = 0;
			for (int i = 0; i < FilterSize; i++) {
				for (int j = 0; j < FilterSize; j++) {
					if ((i == 0 || i == FilterSize - 1) && (j == 0 || j == FilterSize - 1)) 
						sum += In_Pad[h + i][w + j] * 0;
					else if (i == j && i == FilterSize / 2)
						sum += In_Pad[h + i][w + j] * 5;
					else if (i != j && (i == FilterSize / 2 || j == FilterSize / 2))
						sum += In_Pad[h + i][w + j] * -1;
				}
			}
			if (sum < 0) sum = 0;
			else if (sum > 255) sum = 255;
			Out[h][w] = sum;
		}
	}
}
void GaussianFilter(unsigned char** In_Pad, unsigned char** Out, int nHeight, int nWidth, int FilterGauss ,int FilterSize) {
	int halfSize = FilterSize / 2;

	double PI = 3.14159265358979323846;
	double sum = 0;
	double* kernel = new double[FilterSize * FilterSize];
	for (int i = -halfSize; i <= halfSize; i++) {
		for(int j = -halfSize; j <= halfSize; j++) {
			double c = 1 / sqrt(2 * pow(FilterGauss, 2) * PI);
			double x = -((pow(i, 2) + pow(j, 2)) / (2 * pow(FilterGauss, 2)));
			double f = c * exp(x);
			kernel[(i + halfSize)*FilterSize + (j + halfSize)] = f;
			sum += f;
		}
	}
	for (int i = 0; i < FilterSize * FilterSize; i++) {
		kernel[i] /= sum;
	}

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			sum = 0;
			for (int i = 0; i < FilterSize; i++) {
				for (int j = 0; j < FilterSize; j++) {
					double val = (double)In_Pad[h + i][w + j] * kernel[i* FilterSize + j];
					sum += val;
				}
			}
			if (sum > 255) sum = 255;
			else if (sum < 0) sum = 0;
			Out[h][w] = (unsigned char)sum;
		}
	}

}


void CViewerDlg::OnNoisereductionAddnoiseClicked()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	CAddNoiseDlg AddNoiseDlg;
	BYTE** NoiseImg = MemAlloc2D(nHeight_in, nWidth_in, 0);

	if (isImageOpened && nFormat == FORMAT_GRAY) {
		if (AddNoiseDlg.DoModal() == IDOK) {
			CString OutFileName = InpFileName;
			float Value_1 = AddNoiseDlg.GetVal_1();
			float Value_2 = AddNoiseDlg.GetVal_2();
			int nNoiseType = AddNoiseDlg.GetNoistType();

			if (nNoiseType == NOISE_GN) {
				if (Value_2 < 1) {
					MessageBox("표준편차 값이 잘못되었습니다. 표준편차 값은 1 이상이어야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}

				InputGaussianNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				DisplayImage(GrayToMat(NoiseImg, nHeight_in, nWidth_in), false);
				OutFileName += "_GN.raw";
			}
			else {

				if (Value_1 <= 0 && Value_2 <= 0) {
					MessageBox("두 개의 확률 값 중 적어도 하나는 0보다 커야 합니다.");
					MemFree2D(NoiseImg, nHeight_in);
					return;
				}

				InputSaltPepperNoise(ch_in_gray, NoiseImg, nHeight_in, nWidth_in, Value_1, Value_2);
				DisplayImage(GrayToMat(NoiseImg, nHeight_in, nWidth_in), false);

				OutFileName += "SPN.raw";
			}

			FILE* fp;
			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(NoiseImg[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
			MemFree2D(NoiseImg, nHeight_in);

		}
	}
}


void InputGaussianNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fMean, float fStdev) {
	float fTemp = 0, fPDF[256] = { 0.0f };

	GetGaussianPDF(fPDF, 256, fMean, fStdev);

	srand(GetTickCount());

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			fTemp = (float)In[h][w] + GetNoise(fPDF, 256);
			Out[h][w] = saturate_cast<unsigned char>(fTemp); //흰거방지
		}
	}
}

void GetGaussianPDF(float* EmptyPDF, int nLength, float fMean, float fStDev) {
	int Center = nLength / 2;
	float x;
	double PI = 3.14159265358979323846;

	for (int n = 0; n < nLength; n++) {
		x = (float)(n - Center);
		EmptyPDF[n] = (1 / ((float)sqrt(2 * PI)* fStDev)) *exp(-pow(x - fMean, 2) / (2 * fStDev *fStDev));
	}
}


float GetNoise(float* PDF, int nLength) {
	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float* CDF = new float[nLength];
	CDF[0] = 0;
	fRand = (float)rand() / (RAND_MAX + 1); // 0~1 Uniform Distribution

	for (n = 1; n < nLength; n++)
	{
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0)
		{
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);
	delete[] CDF;
	return x + fComp;
}

void InputSaltPepperNoise(unsigned char** In, unsigned char** Out, int nHeight, int nWidth, float fSProb, float fPProb) {
	float Low = fSProb;
	float High = 1.0f - fPProb;
	float fRand;
	srand(GetTickCount());
	for (int h = 0; h < nHeight; h++)
	{
		for (int w = 0; w < nWidth; w++)
		{
			fRand = ((float)rand() / RAND_MAX);
			if (fRand < Low)
			{
				Out[h][w] = 255;
			}
			else if (fRand > High)
			{
				Out[h][w] = 0;
			}
			else Out[h][w] = In[h][w];
		}
	}
}


void CViewerDlg::OnNoisefilteringMeanfilterClicked()
{
	CMeanFilterDlg MFDlg;

	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (MFDlg.DoModal() == IDOK) {
			int nFilterSize = MFDlg.GetFilterSize();
			int nFilterType = MFDlg.GetMeanFilterType();

			if (nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}
			if (nFilterSize % 2 == 0) nFilterSize++;

			CString OutFileName = InpFileName;
			unsigned char **Out = MemAlloc2D(nHeight_in, nWidth_in, 0);

			switch (nFilterType) {

			case NF_ARITHMETIC:
				ArithmeticMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_AMF.raw";
				break;
			case NF_GEOMETRIC:
				GeometricMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_GMF.raw";
				break;
			case NF_HARMONIC:
				HarmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);
				OutFileName += "_HMF.raw";
				break;
			case NF_CONTRAHARMONIC:
				int Q = MFDlg.GetQ();
				ContraharmonicMeanFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, Q);
				OutFileName += "_CHMF.raw";
				break;
			}
			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);

			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++)
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			fclose(fp);
		}
	}
}


void ArithmeticMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {

	int nTemp, nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			nTemp = 0;
			for (int m = 0; m < nFilterSize; m++)
				for (int n = 0; n < nFilterSize; n++)
					nTemp += Img_Pad[h + m][w + n];

			Out[h][w] = static_cast<unsigned char>(nTemp / (nFilterSize * nFilterSize));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void GeometricMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double nTemp = 1.0;
			double count = (double)(nFilterSize * nFilterSize);
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					if (Img_Pad[h + m][w + n] == 0) {
						--count;
					}
					else {
						nTemp *= Img_Pad[h + m][w + n];
					}
				}
			}
			Out[h][w] = static_cast<unsigned char>(pow(nTemp, 1.0 / count));
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void HarmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double nTemp = 0.0;
			double count = (double)(nFilterSize * nFilterSize);
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					if (Img_Pad[h + m][w + n] == 0) {
						--count;
					}
					else {
						nTemp += (double) 1.0 / (double)Img_Pad[h + m][w + n];
					}
				}
			}

			Out[h][w] = static_cast<unsigned char>(count / nTemp);
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void ContraharmonicMeanFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize, int Q) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			double nTemp1 = 0.0, nTemp2 = 0.0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					if (Img_Pad[h + m][w + n] != 0) {
						nTemp1 += pow((double)Img_Pad[h + m][w + n], (double)(Q + 1));
						nTemp2 += pow((double)Img_Pad[h + m][w + n], (double)(Q));
					}
				}
			}

			Out[h][w] = static_cast<unsigned char>(nTemp1 / nTemp2);
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}



void CViewerDlg::OnNoisefilteringOrderstatisticfilter()
{
	COrderStatisticsDlg OSFDlg;
	if (isImageOpened == true && nFormat == FORMAT_GRAY) {
		if (OSFDlg.DoModal() == IDOK) {
			int nFilterSize = OSFDlg.GetFilterSize1();
			int nFilterType = OSFDlg.GetOSFType();

			//CString t;

			//t.Format(_T("%d"), nFilterType);
			//AfxMessageBox(t);

			if (nFilterSize < 3) {
				MessageBox("필터 크기가 잘못되었습니다.");
				return;
			}
			if (nFilterSize % 2 == 0) {
				nFilterSize++;
			}

			CString OutFileName = InpFileName;
			unsigned char ** Out = MemAlloc2D(nHeight_in, nWidth_in, 0);



			switch (nFilterType) {
			case NF_MEDIAN:
				MedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize);

				OutFileName += "_MEDF.raw";
				break;
			case NF_ADAPTIVEMEDIAN:
				int MaxFilterSize = OSFDlg.GetFilterSize2();
				AdaptiveMedianFilter(ch_in_gray, Out, nHeight_in, nWidth_in, nFilterSize, MaxFilterSize);
				OutFileName += "_AMEDF.raw";
			}

			DisplayImage(GrayToMat(Out, nHeight_in, nWidth_in), false);
			FILE* fp;

			fopen_s(&fp, OutFileName, "wb");
			for (int h = 0; h < nHeight_in; h++) {
				fwrite(Out[h], sizeof(unsigned char), nWidth_in, fp);
			}
			fclose(fp);
		}
	}
}

void MedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize) {
	int nPadSize = (int)(nFilterSize / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize);

	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			unsigned char* list = new unsigned char[nFilterSize * nFilterSize];
			int idx = 0;
			for (int m = 0; m < nFilterSize; m++) {
				for (int n = 0; n < nFilterSize; n++) {
					list[idx] = Img_Pad[h + m][w + n];
					++idx;
				}
			}
			std::sort(list, list + nFilterSize * nFilterSize);
			Out[h][w] = list[(nFilterSize * nFilterSize) / 2];
			delete[] list;
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}
void AdaptiveMedianFilter(unsigned char **Img_in, unsigned char** Out, int nHeight, int nWidth, int nFilterSize_min, int nFilterSize_Max) {
	int nPadSize = (int)(nFilterSize_Max / 2);
	unsigned char** Img_Pad = Padding(Img_in, nHeight, nWidth, nFilterSize_Max);

	for (int h = nPadSize; h < nHeight + nPadSize; h++) {
		for (int w = nPadSize; w < nWidth + nPadSize; w++) {
			int curFilterSize = nFilterSize_min;

			int checkNext = -1;

			while (curFilterSize <= nFilterSize_Max) {
				int curPadSize = (int)(curFilterSize / 2);
				unsigned char* list = new unsigned char[curFilterSize * curFilterSize];
				int idx = 0;
				for (int m = -curPadSize; m <= curPadSize; m++) {
					for (int n = -curPadSize; n <= curPadSize; n++) {
						list[idx] = Img_Pad[h + m][w + n];
						++idx;
					}
				}
				std::sort(list, list + curFilterSize * curFilterSize);
				unsigned char zMed = list[(curFilterSize * curFilterSize) / 2];
				unsigned char zMin = list[0];
				unsigned char zMax = list[curFilterSize * curFilterSize - 1];

				delete[] list;

				if ((zMed > zMin) && (zMed < zMax)) {
					unsigned char zXY = Img_Pad[h][w];
					if ((zXY > zMin) && (zXY < zMax)) {
						Out[h - nPadSize][w - nPadSize] = zMed;
					}
					else {
						Out[h - nPadSize][w - nPadSize] = zXY;
					}
					checkNext = 1;
					break;
				}
				else {
					++curFilterSize;
					++curFilterSize;
				}
			}

			if (checkNext != 1) {
				Out[h - nPadSize][w - nPadSize] = Img_Pad[h][w];
			}
		}
	}
	MemFree2D(Img_Pad, nHeight + 2 * nPadSize);
}


void CViewerDlg::OnGetpsnr()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CPSNRDlg PSNRDlg;
	PSNRDlg.DoModal();
}
