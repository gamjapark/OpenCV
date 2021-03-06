// PSNRDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "PSNRDlg.h"
#include "afxdialogex.h"
#include <cmath>


// CPSNRDlg 대화 상자

IMPLEMENT_DYNAMIC(CPSNRDlg, CDialogEx)

CPSNRDlg::CPSNRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PSNRDIALOG, pParent)
{

}

CPSNRDlg::~CPSNRDlg()
{
}

void CPSNRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ORIFILENAME, Edit_File_Ori);
	DDX_Control(pDX, IDC_EDIT_NOISEFILENAME, Edit_File_Dst);
	DDX_Control(pDX, IDC_EDIT_PSNR, Edit_PSNR);
}


BEGIN_MESSAGE_MAP(CPSNRDlg, CDialogEx)
	ON_BN_CLICKED(ID_CLOSE_BUTTON, &CPSNRDlg::OnBnClickedCloseButton)
	ON_BN_CLICKED(OriopenButton, &CPSNRDlg::OnBnClickedOriopenbutton)
	ON_BN_CLICKED(GetpsnrButton, &CPSNRDlg::OnBnClickedGetpsnrbutton)
	ON_BN_CLICKED(NoiseopenButton, &CPSNRDlg::OnBnClickedNoiseopenbutton)
END_MESSAGE_MAP()


// CPSNRDlg 메시지 처리기


void CPSNRDlg::OnBnClickedCloseButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Ori != NULL) {
		MemFree2D(Ori, nHeight_Ori);
	}
	if (Dst != NULL) {
		MemFree2D(Dst, nHeight_Dst);
	}
	CDialog::OnCancel();
}


void CPSNRDlg::OnBnClickedOriopenbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;

	if (Ori != NULL) {
		MemFree2D(Ori, nHeight_Ori);
	}

	if (dlg.DoModal() == IDOK) {

		if (dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "RAW") {
			MessageBox("파일 확장자가 raw 파일이 아닙니다.");
			return;
		}

		FILE* fp;
		fopen_s(&fp, dlg.GetPathName(), "rb");	//GetFileName -> GetPathName (경로를 넣어줌)
		if (RawDlg.DoModal() == IDOK) {

			nHeight_Ori = RawDlg.GetRawHeight();
			nWidth_Ori = RawDlg.GetRawWidth();
			if (nHeight_Ori == 0 || nWidth_Ori == 0 || RawDlg.GetRawFormat() != 3) {
				MessageBox("입력 값이 잘못되었습니다.");
				return;
			}
			Edit_File_Ori.SetWindowTextA(dlg.GetFileTitle());
			Ori = MemAlloc2D(nHeight_Ori, nWidth_Ori, 0);

			for (int h = 0; h < nHeight_Ori; h++) {
				fread(Ori[h], sizeof(unsigned char), nWidth_Ori, fp);
			}
			fclose(fp);
		}

	}
}


void CPSNRDlg::OnBnClickedGetpsnrbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (Ori == NULL || Dst == NULL) {
		MessageBox("원본 영상과 비교 영상을 불러오지 않았습니다.");
		return;
	}
	if (nHeight_Ori != nHeight_Dst || nWidth_Ori != nWidth_Dst) {
		MessageBox("원본 영상과 비교영상의 크기가 다릅니다.");
		return;
	}
	CString strTemp;
	double dPSNR = GetPSNR(Ori, Dst, nHeight_Ori, nWidth_Ori);

	strTemp.Format(_T("%f"), dPSNR);
	Edit_PSNR.SetWindowTextA(strTemp);
}


void CPSNRDlg::OnBnClickedNoiseopenbutton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CFileDialog dlg(TRUE);
	CRawInfoDlg RawDlg;

	if (Dst != NULL) {
		MemFree2D(Dst, nHeight_Dst);
	}

	if (dlg.DoModal() == IDOK) {

		if (dlg.GetFileExt() != "raw" && dlg.GetFileExt() != "RAW") {
			MessageBox("파일 확장자가 raw 파일이 아닙니다.");
			return;
		}

		FILE* fp;
		fopen_s(&fp, dlg.GetPathName(), "rb");	//GetFileName -> GetPathName (경로를 넣어줌)
		if (RawDlg.DoModal() == IDOK) {

			nHeight_Dst = RawDlg.GetRawHeight();
			nWidth_Dst = RawDlg.GetRawWidth();
			if (nHeight_Dst == 0 || nWidth_Dst == 0 || RawDlg.GetRawFormat() != 3) {
				MessageBox("입력 값이 잘못되었습니다.");
				return;
			}
			Edit_File_Dst.SetWindowTextA(dlg.GetFileTitle());
			Dst = MemAlloc2D(nHeight_Dst, nWidth_Dst, 0);

			for (int h = 0; h < nHeight_Dst; h++) {
				fread(Dst[h], sizeof(unsigned char), nWidth_Dst, fp);
			}
			fclose(fp);
		}
	}
}


double CPSNRDlg::GetPSNR(unsigned char**Ori, unsigned char** Dst, int nHeight, int nWidth) {
	/*
	구현 부분
	: PSNR
	영상의 변화 또는 차이의 정도를 판별하는 기준 값 중 하나
	잡음 첨가 전의 원본과 필터를 적용한 결과를 비교
	차기가 적을 수록 값이 큼
	*/
	double MSE = 0.0;
	for (int h = 0; h < nHeight; h++) {
		for (int w = 0; w < nWidth; w++) {
			MSE += ((double)(pow(Ori[h][w] - Dst[h][w], 2)) / (double)(nHeight * nWidth));
		}
	}
	return 10.0 * log10l((double)(pow(255, 2)) / MSE);
}