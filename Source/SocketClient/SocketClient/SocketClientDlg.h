
// SocketClientDlg.h : header file
//

#pragma once


// CSocketClientDlg dialog
class CSocketClientDlg : public CDialogEx
{
// Construction
public:
	CSocketClientDlg(CWnd* pParent = nullptr);	// standard constructor
	~CSocketClientDlg();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SOCKETCLIENT_DIALOG };
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
public: // Các biến
	SOCKET _sockClient;
	CString _IPServer;
	CString _Client_name, _Client_pass, _Client_name2;
	
	int _countedMessage;
	bool _isDownloading;
public:
	CComboBox _Select;
	CString _SelectNameFile;
	//CString _List;

	CProgressCtrl _progress;

	void ProcessDlg();
	void DeProcessDlg();


public:
	void errorSocket();
	bool readListToCString();

	bool nhanFile();
	void ServerClose();


	static UINT __cdecl DownloadThreadHelper(LPVOID pParam);
	UINT DownloadThreadFunc();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedRegister();
	afx_msg void OnBnClickedCheck();

	afx_msg void OnBnClickedDowload();
	afx_msg void OnBnClickedLogout();

	afx_msg void OnCbnSelchangeSelectfile();

	afx_msg void OnBnClickedMessage();
};
