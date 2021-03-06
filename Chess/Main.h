#pragma once

#include "client.h"
#include<thread>
using namespace std;


// Main dialog

class Main : public CDialogEx
{
	DECLARE_DYNAMIC(Main)

public:
	Main(CWnd* pParent = nullptr);   // standard constructor
	virtual ~Main();
	void setClient(Client* client);
	BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = MAIN_FORM };
#endif

private:
	Client* _client;
	void updateListPlayer();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox cListBoxPlayer;
	afx_msg void OnClickedUpdateButton();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedRoomButton();
	CEdit cEditPlayerConnect;
	afx_msg void OnClickedConnectButton();
	afx_msg void OnLbnSelectPlayer();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedProfile();
};
