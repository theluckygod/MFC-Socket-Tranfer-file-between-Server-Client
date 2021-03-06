
// SocketClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SocketClient.h"
#include "SocketClientDlg.h"
#include "afxdialogex.h"

#include "lib.h"
#include <iostream>
#include "MessageDlg.h"
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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


// CSocketClientDlg dialog


void CSocketClientDlg::errorSocket()
{
	MessageBox(_T("Socket is error!"), _T("ERROR"), 0);
	GetDlgItem(IDC_CHECK)->EnableWindow(TRUE);
	GetDlgItem(IDC_IPSERVER)->EnableWindow(TRUE);

	closesocket(_sockClient);
	_sockClient = NULL;
	UpdateData(FALSE);
}
bool CSocketClientDlg::readListToCString()
{
	FILE* pRead = fopen(FILENAMEALLOWEDDOWNLOAD, "rt");
	if (pRead == NULL) return 0;
	// file rỗng
	fseek(pRead, SEEK_END, 0);
	if (ftell(pRead) == 0)
	{
		fclose(pRead);
		return 0;
	}
	fseek(pRead, SEEK_SET, 0);

	char buf[MAXNAMEFILE];
	CListBox* lb = (CListBox*)GetDlgItem(IDC_ALLOWEDDOWNLOAD);
	CComboBox* cb = (CComboBox*)GetDlgItem(IDC_SELECTFILE);
	CString str;
	while (!feof(pRead))
	{
		buf[0] = '\0';
		fgets(buf, MAXNAMEFILE, pRead);
		if (strlen(buf) > 0)
		{
			str = buf;
			if (lb->FindStringExact(0, str) == LB_ERR)
				lb->AddString(str);
			if (cb->FindStringExact(0, str) == CB_ERR)
				cb->AddString(str);
			
			//_List = _List + (CString)(buf);
			//_List = _List + L"\r\n";
		}
	}
	UpdateData(false);
	fclose(pRead);
	return true;
}

CSocketClientDlg::CSocketClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SOCKETCLIENT_DIALOG, pParent),
	 _Client_name(_T("")), _IPServer(_T("127.0.0.1")),
	_SelectNameFile(_T("")), _sockClient(NULL), _isDownloading(false)//, _List(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	AfxSocketInit(NULL);
}

CSocketClientDlg::~CSocketClientDlg()
{
	closesocket(_sockClient);
	_sockClient = NULL;
}

void CSocketClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IPSERVER, _IPServer);
	DDX_Text(pDX, IDC_NHAPUSER, _Client_name);
	DDX_Text(pDX, IDC_NHAPPASSWORD, _Client_pass);
	DDV_MaxChars(pDX, _Client_name, MAXNAMECLIENT);
	DDV_MaxChars(pDX, _Client_pass, MAXPASSCLIENT);
	DDX_Text(pDX, IDC_SELECTFILE, _SelectNameFile);
	//DDX_Text(pDX, IDC_ALLOWEDDOWNLOAD, _List);
	DDX_Control(pDX, IDC_PROGRESS1, _progress);

	DDX_Control(pDX, IDC_SELECTFILE, _Select);
}

BEGIN_MESSAGE_MAP(CSocketClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSocketClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOGIN, &CSocketClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_REGISTER, &CSocketClientDlg::OnBnClickedRegister)
	ON_BN_CLICKED(IDC_CHECK, &CSocketClientDlg::OnBnClickedCheck)
	ON_BN_CLICKED(IDC_DOWLOAD, &CSocketClientDlg::OnBnClickedDowload)
	ON_BN_CLICKED(IDC_LOGOUT, &CSocketClientDlg::OnBnClickedLogout)
	ON_CBN_SELCHANGE(IDC_SELECTFILE, &CSocketClientDlg::OnCbnSelchangeSelectfile)
	ON_BN_CLICKED(IDC_MESSAGE, &CSocketClientDlg::OnBnClickedMessage)
END_MESSAGE_MAP()


// CSocketClientDlg message handlers

BOOL CSocketClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization her
	_progress.SetRange(0, 100);
	_progress.SetPos(0);

	// hide
	GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_USER2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NHAPUSER2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOGOUT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sLIST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALLOWEDDOWNLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sSELECTFILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SELECTFILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DOWLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	//
	GetDlgItem(IDC_USER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NHAPUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sPASSWORD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NHAPPASSWORD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sIPSERVER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_IPSERVER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOGIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_REGISTER)->ShowWindow(SW_SHOW);
	//

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocketClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocketClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSocketClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSocketClientDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CSocketClientDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (_Client_name.GetLength() == 0)
	{
		MessageBox(_T("Empty username is not legal!"), _T("ERROR"), 0);
		return;
	}
	if (_Client_pass.GetLength() == 0)
	{
		MessageBox(_T("Empty password is not legal!"), _T("ERROR"), 0);
		return;
	}

	int msgError = sendMessageToServer(_sockClient, LOGIN);

	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		_Client_pass = _T("");
		errorSocket();
		return;
	}



	char *CCPass = (char*)malloc(MAXPASSCLIENT + 1), CCName[MAXNAMECLIENT + 1];
	int msgFromServer;

	wcstombs(CCPass, _Client_pass, _Client_pass.GetLength() + 1);
	wcstombs(CCName, _Client_name, _Client_name.GetLength() + 1);
	Hash(CCPass);
	//MessageBox((CString)CCName, _T("Message"), 0);//
	//MessageBox((CString)CCPass, _T("Message"), 0); //

	_Client_pass = _T("");

	msgFromServer = requestLogin(_sockClient, CCName, CCPass);

	if (msgFromServer == VALIDACCOUNT)
	{
		if (CCPass != NULL)
		{
			free(CCPass);
			CCPass = NULL;
		}

		{
			msgError = receiveMessageFromServer(_sockClient, _countedMessage);// doc ten file
			if (msgError == 0 || msgError == SOCKET_ERROR)
			{
				errorSocket();
				return;
			}
			else
			{
				// if login success
				_Client_name2 = _Client_name;
				UpdateData(true);
				//_List = _T("");
				_progress.SetPos(0);
				_SelectNameFile = _T("");
				UpdateData(true);
				ProcessDlg();
			}
		}
	}
	else
		if (msgFromServer == ERROR_INVALIDACCOUNT)
		{
			MessageBox(_T("INVALIDACCOUNT: user or password is incorrect."), _T("ERROR"), 0);
			if (CCPass != NULL)
			{
				free(CCPass);
				CCPass = NULL;
			}
		}

	if (msgFromServer == EXITED)
	{
		if (CCPass != NULL)
		{
			free(CCPass);
			CCPass = NULL;
		}
		MessageBox(_T("Server has exited!"), _T("ERROR"), 0);
	}
			

}


void CSocketClientDlg::OnBnClickedRegister()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	if (_Client_name.GetLength() == 0)
	{
		MessageBox(_T("Empty username is not legal!"), _T("ERROR"), 0);
		return;
	}
	if (_Client_pass.GetLength() == 0)
	{
		MessageBox(_T("Empty password is not legal!"), _T("ERROR"), 0);
		return;
	}

	//Nhập tài khoản mới 
	int msgError = sendMessageToServer(_sockClient, REGISTRATION);

	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		_Client_pass = _T("");
		errorSocket();
		return;
	}

	char *CCPass = (char*)malloc(MAXPASSCLIENT + 1), CCName[MAXNAMECLIENT + 1];

	int msgFromServer;

	wcstombs(CCName, _Client_name, _Client_name.GetLength() + 1);
	wcstombs(CCPass, _Client_pass, _Client_pass.GetLength() + 1);
	Hash(CCPass);
	_Client_pass = _T("");

	msgFromServer = sendRequestRegister(_sockClient, CCName, CCPass);

	if (msgFromServer == REGISTERED) {
		MessageBox(_T("Register successfully!"), _T("SUCCESS"), 0);
		if (CCPass != NULL)
		{
			free(CCPass);
			CCPass = NULL;
		}
	}
	else if (msgFromServer == HAS_EXISTED)
	{
		MessageBox(_T("Existed account!"), _T("ERROR"), 0);
		if (CCPass != NULL)
		{
			free(CCPass);
			CCPass = NULL;
		}
	}
		
	if (msgFromServer == EXITED)
	{
		if (CCPass != NULL)
		{
			free(CCPass);
			CCPass = NULL;
		}
		MessageBox(_T("Server has exited!"), _T("ERROR"), 0);
	}
}


void CSocketClientDlg::OnBnClickedCheck()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	CSocket cs;
	cs.Create();

	UpdateData();
	// convert to char
	char c_IPServer[20];
	wcstombs(c_IPServer, _IPServer, _IPServer.GetLength() + 1);
	//


	if (cs.Connect(CA2W(c_IPServer), PORTNUMBER))
	{
		GetDlgItem(IDC_CHECK)->EnableWindow(FALSE);
		GetDlgItem(IDC_IPSERVER)->EnableWindow(FALSE);

		_sockClient = cs.Detach();
	}
	else
	{
		cs.Close();
		_sockClient = NULL;
		MessageBox(_T("Failed to connect to server"), _T("ERROR"), 0);
	}
}


void CSocketClientDlg::ProcessDlg()
{
	// disable
	GetDlgItem(IDC_MESSAGE)->EnableWindow(true);
	GetDlgItem(IDC_USER2)->EnableWindow(true);
	GetDlgItem(IDC_NHAPUSER2)->EnableWindow(true);
	CWnd *pEditField = GetDlgItem(IDC_NHAPUSER2);
	if (pEditField != NULL)
		pEditField->SetWindowText(_Client_name2);
	GetDlgItem(IDC_LOGOUT)->EnableWindow(true);
	GetDlgItem(IDC_sLIST)->EnableWindow(true);
	GetDlgItem(IDC_ALLOWEDDOWNLOAD)->EnableWindow(true);
	GetDlgItem(IDC_sSELECTFILE)->EnableWindow(true);
	GetDlgItem(IDC_SELECTFILE)->EnableWindow(true);
	GetDlgItem(IDC_DOWLOAD)->EnableWindow(true);
	GetDlgItem(IDC_PROGRESS1)->EnableWindow(true);
	//
	GetDlgItem(IDC_USER)->EnableWindow(false);
	GetDlgItem(IDC_NHAPUSER)->EnableWindow(false);
	GetDlgItem(IDC_sPASSWORD)->EnableWindow(false);
	GetDlgItem(IDC_NHAPPASSWORD)->EnableWindow(false);
	GetDlgItem(IDC_sIPSERVER)->EnableWindow(false);
	GetDlgItem(IDC_IPSERVER)->EnableWindow(false);
	GetDlgItem(IDC_CHECK)->EnableWindow(false);
	GetDlgItem(IDC_LOGIN)->EnableWindow(false);
	GetDlgItem(IDC_REGISTER)->EnableWindow(false);



	// hide
	GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_USER2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NHAPUSER2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOGOUT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sLIST)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALLOWEDDOWNLOAD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sSELECTFILE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_SELECTFILE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_DOWLOAD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_SHOW);
	//
	GetDlgItem(IDC_USER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NHAPUSER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sPASSWORD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NHAPPASSWORD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sIPSERVER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_IPSERVER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOGIN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_REGISTER)->ShowWindow(SW_HIDE);
	//
	// nhan file cac tệp dc phép tải
	if (nhanFile() == true)
	{
		// xuatCacFile duoc download len màn hình
		if (readListToCString() == true)
		{
			UpdateData();
		}
		else
		{
			MessageBox(_T("Failed to read allowed-download list!"), _T("ERROR"), 0);
			DeProcessDlg();
		}

	}
	else
	{
		MessageBox(_T("Failed to receive allowed-download list from server!"), _T("ERROR"), 0);
		DeProcessDlg();
	}
}

void CSocketClientDlg::DeProcessDlg()
{
	// disable
	GetDlgItem(IDC_MESSAGE)->EnableWindow(false);
	GetDlgItem(IDC_USER2)->EnableWindow(false);
	GetDlgItem(IDC_NHAPUSER2)->EnableWindow(false);
	GetDlgItem(IDC_LOGOUT)->EnableWindow(false);
	GetDlgItem(IDC_sLIST)->EnableWindow(false);
	GetDlgItem(IDC_ALLOWEDDOWNLOAD)->EnableWindow(false);
	GetDlgItem(IDC_sSELECTFILE)->EnableWindow(false);
	GetDlgItem(IDC_SELECTFILE)->EnableWindow(false);
	GetDlgItem(IDC_DOWLOAD)->EnableWindow(false);
	GetDlgItem(IDC_PROGRESS1)->EnableWindow(false);
	//
	GetDlgItem(IDC_USER)->EnableWindow(true);
	GetDlgItem(IDC_NHAPUSER)->EnableWindow(true);
	GetDlgItem(IDC_sPASSWORD)->EnableWindow(true);
	GetDlgItem(IDC_NHAPPASSWORD)->EnableWindow(true);
	GetDlgItem(IDC_sIPSERVER)->EnableWindow(true);
	GetDlgItem(IDC_IPSERVER)->EnableWindow(true);
	GetDlgItem(IDC_CHECK)->EnableWindow(true);
	GetDlgItem(IDC_LOGIN)->EnableWindow(true);
	GetDlgItem(IDC_REGISTER)->EnableWindow(true);

	// hide
	GetDlgItem(IDC_MESSAGE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_USER2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_NHAPUSER2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_LOGOUT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sLIST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_ALLOWEDDOWNLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_sSELECTFILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_SELECTFILE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_DOWLOAD)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PROGRESS1)->ShowWindow(SW_HIDE);
	//
	GetDlgItem(IDC_USER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NHAPUSER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sPASSWORD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_NHAPPASSWORD)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_sIPSERVER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_IPSERVER)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CHECK)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_LOGIN)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_REGISTER)->ShowWindow(SW_SHOW);
	//
}

void CSocketClientDlg::OnBnClickedDowload()
{
	// TODO: Add your control notification handler code here
	int msg = sendMessageToServer(_sockClient, COMMAND_READFILE);
	if (msg == 0 || msg == SOCKET_ERROR)
	{
		ServerClose();
		return;
	}

	AfxBeginThread(DownloadThreadHelper, this);
}


void CSocketClientDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	if (_isDownloading == false)
	{
		closesocket(_sockClient);
		_sockClient = NULL;
		DeProcessDlg();
	}
	else _isDownloading = false;
}

void CSocketClientDlg::OnCbnSelchangeSelectfile()
{
	// TODO: Add your control notification handler code here
	_SelectNameFile = _T("");
	int nSel = _Select.GetCurSel();
	if (nSel != LB_ERR)
	{
		_Select.GetLBText(nSel, _SelectNameFile);
	}
}


bool CSocketClientDlg::nhanFile()
{
	int count = 0, msgError, sizeOfFile, sizeOfDownloaded = 0; // dem ten
	char namefile[MAXNAMEFILE];

	msgError = receiveMessageFromServer(_sockClient, count);// doc ten file
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		ServerClose();
		return false;
	}
	msgError = receiveDataFromServer(_sockClient, namefile, count);
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		ServerClose();
		return false;
	}
	msgError = receiveMessageFromServer(_sockClient, sizeOfFile); // doc kich thuoc file
	if (msgError == 0 || msgError == SOCKET_ERROR)
	{
		ServerClose();
		return false;
	}

	if (strstr(namefile, FILENAMEALLOWEDDOWNLOAD)) // nhận file allowedDownload.txt
		strcpy(namefile, FILENAMEALLOWEDDOWNLOAD);

	FILE* pWrite = fopen(namefile, "wb");

	char buf[MAXWRITE];
	CSocket cstemp;
	cstemp.Attach(_sockClient);
	_sockClient = NULL;
	_isDownloading = true;
	do // doc file
	{
		count = 0; // dem bytes

		msgError = cstemp.Receive((char*)&count, sizeof(int), 0);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			ServerClose();
			break;
		}
		msgError = cstemp.Receive(buf, count, 0);
		if (msgError == 0 || msgError == SOCKET_ERROR)
		{
			ServerClose();
			break;
		}

		fwrite(buf, 1, count, pWrite);
		sizeOfDownloaded += count;

		_progress.SetPos(TienDo(sizeOfFile, sizeOfDownloaded));
	
	} while (count == MAXWRITE	&& _isDownloading == true);
	fclose(pWrite);

	_sockClient = cstemp.Detach();
	if (_isDownloading == false)
	{
		OnBnClickedLogout();
	}
	else _isDownloading = false;
	// Thêm phần kiểm tra đã gửi hết chưa 
	if (sizeOfFile == sizeOfDownloaded) return true;
	else return false;
}
void CSocketClientDlg::ServerClose()
{
	MessageBox((CString)"Server is close!", _T("ERROR"), 0);
	OnBnClickedLogout();
}


UINT CSocketClientDlg::DownloadThreadFunc()
{
	AfxSocketInit(NULL);

	GetDlgItem(IDC_MESSAGE)->EnableWindow(false);
	GetDlgItem(IDC_DOWLOAD)->EnableWindow(false);

	if (_SelectNameFile.GetLength() == 0)
	{
		MessageBox(_T("Empty filename is not legal!"), _T("ERROR"), 0);
		GetDlgItem(IDC_MESSAGE)->EnableWindow(true);
		GetDlgItem(IDC_DOWLOAD)->EnableWindow(true);
		return 0;
	}
	char namefile[MAXNAMEFILE];
	// Phần viết chọn file
	//MessageBox(_SelectNameFile, _T("ERROR"), 0);
	wcstombs(namefile, _SelectNameFile, _SelectNameFile.GetLength() + 1);

	if (chooseNameFile(namefile) == false)
	{
		MessageBox(_T("Choose a unavailable file!"), _T("ERROR"), 0);
		GetDlgItem(IDC_MESSAGE)->EnableWindow(true);
		GetDlgItem(IDC_DOWLOAD)->EnableWindow(true);
		return 0;
	}

	// nhận thông báo file có tìm thấy không
	int msgFromServer = requestFile(_sockClient, namefile);

	if (msgFromServer == SENDFILE)
	{
		// qua trinh nhan file
		if (nhanFile() == true)
		{
			MessageBox(_T("Download successfully!"), _T("FINISH"), 0);
		}
		else 	MessageBox(_T("Download unsuccessfully!"), _T("TEMINATE"), 0);
	}
	else
	{
		if (msgFromServer == ERROR_FINDFILE)
			MessageBox(_T("File not found!"), _T("ERROR"), 0);
		if (msgFromServer == 0 || msgFromServer == SOCKET_ERROR)
			ServerClose();
	}

	GetDlgItem(IDC_MESSAGE)->EnableWindow(true);
	GetDlgItem(IDC_DOWLOAD)->EnableWindow(true);

	return 0;
}

UINT __cdecl CSocketClientDlg::DownloadThreadHelper(LPVOID pParam)
{
	CSocketClientDlg *pYourClass = reinterpret_cast<CSocketClientDlg*>(pParam);
	pYourClass->DownloadThreadFunc();
	return 0;
}

void CSocketClientDlg::OnBnClickedMessage()
{
	// TODO: Add your control notification handler code here
	AfxSocketInit(NULL);

	MessageDlg msgdlg(_sockClient, _countedMessage);
	msgdlg.DoModal();
}
