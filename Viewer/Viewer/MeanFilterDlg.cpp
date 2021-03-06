// MeanFilterDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Viewer.h"
#include "MeanFilterDlg.h"
#include "afxdialogex.h"


// CMeanFilterDlg 대화 상자

IMPLEMENT_DYNAMIC(CMeanFilterDlg, CDialogEx)

CMeanFilterDlg::CMeanFilterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEANFILTERDLG, pParent)
{

}

CMeanFilterDlg::~CMeanFilterDlg()
{
}

void CMeanFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ARITHMETIC, m_radio_mf);
	DDX_Control(pDX, IDC_EDIT_MF_FILTERSIZE, Edit_FilterSize);
	DDX_Control(pDX, IDC_EDIT_Q, Edit_Q);
}


BEGIN_MESSAGE_MAP(CMeanFilterDlg, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_ARITHMETIC, IDC_RADIO_CONTRA_HARMONIC, OnRadioClicked)
	ON_BN_CLICKED(IDOK_MF, &CMeanFilterDlg::OnBnClickedMf)
END_MESSAGE_MAP()


// CMeanFilterDlg 메시지 처리기


void CMeanFilterDlg::OnRadioClicked(UINT msg) {
	UpdateData(TRUE);
	switch (m_radio_mf)
	{
	case 0:
		nMeanFilterType = 0;
		break;
	case 1:
		nMeanFilterType = 1;
		break;
	case 2:
		nMeanFilterType = 2;
		break;
	case 3:
		nMeanFilterType = 3;
		break;
	}
}

int CMeanFilterDlg::GetFilterSize() { return nFilterSize; }
int CMeanFilterDlg::GetMeanFilterType() { return nMeanFilterType; }
int CMeanFilterDlg::GetQ() { return Q; }

void CMeanFilterDlg::OnBnClickedMf()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTemp1, strTemp2;
	Edit_FilterSize.GetWindowTextA(strTemp1);
	Edit_Q.GetWindowTextA(strTemp2);

	nFilterSize = atoi(strTemp1);
	Q = atoi(strTemp2);
	CDialogEx::OnOK();
}
