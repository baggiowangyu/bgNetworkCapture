
// bgSnifferDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "bgSniffer.h"
#include "bgSnifferDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CbgSnifferDlg 对话框




CbgSnifferDlg::CbgSnifferDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CbgSnifferDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbgSnifferDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_NETWORK_ADAPTER, m_cNetworkAdapters);
	DDX_Control(pDX, IDC_LIST_STREAM_INFO, m_cStreamURLs);
	DDX_Control(pDX, IDC_LIST_RECORD_INFO, m_cRecordInfos);
}

BEGIN_MESSAGE_MAP(CbgSnifferDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_CAPTURE, &CbgSnifferDlg::OnBnClickedBtnStartCapture)
	ON_BN_CLICKED(IDC_BTN_STOP_CAPTURE, &CbgSnifferDlg::OnBnClickedBtnStopCapture)
END_MESSAGE_MAP()


// CbgSnifferDlg 消息处理程序

BOOL CbgSnifferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_cNetworkAdapters.InsertColumn(0, _T("网卡名称"), 0, 200);
	m_cNetworkAdapters.InsertColumn(1, _T("IP地址"), 0, 200);
	m_cNetworkAdapters.InsertColumn(2, _T("网卡路径"), 0, 200);
	
	m_cNetworkAdapters.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	m_cStreamURLs.InsertColumn(0, _T("流路径"), 0, 500);
	m_cStreamURLs.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	m_cRecordInfos.InsertColumn(0, _T("文件名"), 0, 300);
	m_cRecordInfos.InsertColumn(1, _T("已下载大小"), 0, 300);
	m_cRecordInfos.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	// 这里初始化网络环境，枚举出
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
	{
		MessageBox(_T("获取网卡信息失败！请关闭程序后重新打开！"), _T("错误"), MB_OK|MB_ICONERROR);
		return TRUE;
	}

	pcap_if_t *devs = nullptr;
	for(devs = alldevs; devs; devs = devs->next)
	{
		int item_count = m_cNetworkAdapters.GetItemCount();

		USES_CONVERSION;
		m_cNetworkAdapters.InsertItem(item_count, A2T(devs->description));
		m_cNetworkAdapters.SetItemText(item_count, 1, A2T(devs->name));

		struct in_addr net_ip_address;
		u_int32_t net_ip;  
		char *net_ip_string = nullptr;

		struct in_addr net_mask_address;  
		u_int32_t net_mask;  
		char *net_mask_string = nullptr; 

		pcap_lookupnet(devs->name,&net_ip,&net_mask,errbuf);  
		net_ip_address.s_addr = net_ip;  
		net_ip_string = inet_ntoa(net_ip_address);
		m_cNetworkAdapters.SetItemText(item_count, 2, A2T(net_ip_string));
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CbgSnifferDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CbgSnifferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CbgSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CbgSnifferDlg::OnBnClickedBtnStartCapture()
{
	
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};

	// 取出选中的网卡路径
	POSITION pos = m_cNetworkAdapters.GetFirstSelectedItemPosition();
	int index = m_cNetworkAdapters.GetNextSelectedItem(pos);
	CString network_name = m_cNetworkAdapters.GetItemText(index, 2);

	pcap_if_t *devs = nullptr;
	for(devs = alldevs; devs; devs = devs->next)
	{
		int item_count = m_cNetworkAdapters.GetItemCount();

		USES_CONVERSION;
		if (_tcsicmp(A2T(devs->name), network_name.GetString()) == 0)
		{
			// 找到了设备，准备开始抓包

			// 先打开设备
			pcap_device = pcap_open_live(devs->name, 65536, 1, 1000, errbuf);
			if (pcap_device == nullptr)
			{
				MessageBox(_T("打开网卡失败！"), _T("错误"), MB_OK|MB_ICONERROR);
				return ;
			}

			// 启动抓包线程！
			CreateThread(nullptr, 0, CaptureThread, this, 0, nullptr);
			
		}
	}
}


void CbgSnifferDlg::OnBnClickedBtnStopCapture()
{
	// TODO: 在此添加控件通知处理程序代码
}

// 参考：http://bbs.csdn.net/topics/360090401
DWORD CbgSnifferDlg::CaptureThread(LPVOID lpParam)
{
	CbgSnifferDlg *dlg = (CbgSnifferDlg *)lpParam;
	int errCode = 0;

	pcap_dumper_t *dumpfile = pcap_dump_open(dlg->pcap_device, "pkt_dmp.dat");

	struct pcap_pkthdr *header = nullptr;
	const u_char *pkt_data = nullptr;
	while ((errCode = pcap_next_ex(dlg->pcap_device, &header, &pkt_data)) >= 0)
	{
		if (errCode == 0)
			continue;
		//
		EtherHeader *eh = (EtherHeader*)pkt_data;

		if (ntohs(eh->ether_type) == 0x0800)
		{
			// 得到了IP包，解码包
		}

		pcap_dump((unsigned char *)dumpfile, header, pkt_data);
	}
	return 0;
}