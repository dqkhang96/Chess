// ProfileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Chess.h"
#include "ProfileDialog.h"
#include "afxdialogex.h"


// ProfileDialog dialog

IMPLEMENT_DYNAMIC(ProfileDialog, CDialogEx)

ProfileDialog::ProfileDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(PROFILE_DIALOG, pParent)
{

}
void ProfileDialog::setInfor(string infor) {
	_infor = infor;
}

ProfileDialog::~ProfileDialog()
{
}

void ProfileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, USER_ID, UserId);
	DDX_Control(pDX, SCORE, score);
	DDX_Control(pDX, RANK, Rank);
	DDX_Control(pDX, COUNT_GAME, countGame);
	DDX_Control(pDX, COUNT_GAME_WIN, countGameWin);
}

BOOL ProfileDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//set user id
	int begin = 0, end = 0;
	while (_infor[end] != '|') end++;
	UserId.SetWindowTextW(CString(CA2T(_infor.substr(begin, end - begin).c_str())));
	//set score
	end++;
	begin = end;
	while (_infor[end] != '|') end++;
	score.SetWindowTextW(CString(CA2T(_infor.substr(begin, end - begin).c_str())));
	//set number game
	end++;
	begin = end;
	while (_infor[end] != '|') end++;
	countGame.SetWindowTextW(CString(CA2T(_infor.substr(begin, end - begin).c_str())));
	//set number game win
	end++;
	begin = end;
	while (_infor[end] != '|') end++;
	countGameWin.SetWindowTextW(CString(CA2T(_infor.substr(begin, end - begin).c_str())));
	//set rank
	end++;
	begin = end;
	Rank.SetWindowTextW(CString(CA2T(_infor.substr(begin, _infor.size() - begin).c_str())));

	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, true);
	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(ProfileDialog, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &ProfileDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// ProfileDialog message handlers


void ProfileDialog::OnBnClickedButton1()
{
	OnCancel();
}
