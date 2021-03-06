
// ChessDlg.h : header file
//

#pragma once
#include "client.h"


// CChessDlg dialog
class CChessDlg : public CDialogEx
{
private:
	Client* _client;
	void login();
// Construction
public:
	CChessDlg(CWnd* pParent = nullptr);	// standard constructor
	void setClient(Client* client);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = LOGIN_FORM};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton();
	afx_msg void OnClickedCancelButton();
	CEdit ceditUserId;
	CEdit ceditPassword;
	afx_msg void OnEnterPassword();
};
