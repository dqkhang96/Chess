#pragma once
#include"string"
using namespace std;

// ProfileDialog dialog

class ProfileDialog : public CDialogEx
{
	DECLARE_DYNAMIC(ProfileDialog)

public:
	ProfileDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ProfileDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = PROFILE_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit UserId;
	CEdit score;
	CEdit Rank;
	CEdit countGame;
	CEdit countGameWin;
	string _infor;
	afx_msg void OnBnClickedButton1();
	BOOL OnInitDialog();
	void setInfor(string infor);
};
