// Main.cpp : implementation file
//

#include "stdafx.h"
#include "Chess.h"
#include "Main.h"
#include "afxdialogex.h"
#include "client.h"
#include "ProfileDialog.h"
#include<thread>
using namespace std;

// Main dialog

IMPLEMENT_DYNAMIC(Main, CDialogEx)

Main::Main(CWnd* pParent /*=nullptr*/)
	: CDialogEx(MAIN_FORM, pParent)
{

}

Main::~Main()
{
}
void Main :: setClient(Client* client) {
	_client = client;
	
}
void Main::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, LIST_PLAYER, cListBoxPlayer);
	DDX_Control(pDX, PLAYER_CONNECT, cEditPlayerConnect);
}


BEGIN_MESSAGE_MAP(Main, CDialogEx)
	
	ON_BN_CLICKED(UPDATE_BUTTON, &Main::OnClickedUpdateButton)
	ON_BN_CLICKED(CANCEL_BUTTON, &Main::OnBnClickedButton)
	ON_BN_CLICKED(CREATE_ROOM_BUTTON, &Main::OnBnClickedRoomButton)
	ON_BN_CLICKED(CONNECT_BUTTON, &Main::OnClickedConnectButton)
	ON_LBN_SELCHANGE(LIST_PLAYER, &Main::OnLbnSelectPlayer)
	ON_BN_CLICKED(PROFILE, &Main::OnBnClickedProfile)
END_MESSAGE_MAP()


// Main message handlers

void Main::updateListPlayer() {
	std::vector<std::string> listPlayer = _client->getListRoom();
	cListBoxPlayer.ResetContent();
	for (int i = 0; i < listPlayer.size(); i++)
		cListBoxPlayer.InsertString(i,CString(CA2T(listPlayer[i].c_str())));
}

// CChessDlg message handlers

BOOL Main::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HICON hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon, true);
	ShowWindow(SW_SHOW);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Main::OnClickedUpdateButton()
{
	updateListPlayer();
}


void Main::OnBnClickedButton()
{
	OnCancel();
}


void Main::OnBnClickedRoomButton()
{
	_client->createRoom();
}


void Main::OnClickedConnectButton()
{
	CString player;
	cEditPlayerConnect.GetWindowTextW(player);
	std::string s_player = (CStringA)player;
	_client->join(s_player);
}


void Main::OnLbnSelectPlayer()
{
	CString selectItem;
	cListBoxPlayer.GetText(cListBoxPlayer.GetCurSel(),selectItem);
	cEditPlayerConnect.SetWindowTextW(selectItem);
}


void Main::OnBnClickedProfile()
{
	ProfileDialog profileDialog;
	profileDialog.setInfor(_client->getProfile());
	profileDialog.DoModal();


}
