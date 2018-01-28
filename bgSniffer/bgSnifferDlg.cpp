
// bgSnifferDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "bgSniffer.h"
#include "bgSnifferDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CbgSnifferDlg �Ի���




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


// CbgSnifferDlg ��Ϣ�������

BOOL CbgSnifferDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_cNetworkAdapters.InsertColumn(0, _T("��������"), 0, 200);
	m_cNetworkAdapters.InsertColumn(1, _T("IP��ַ"), 0, 200);
	m_cNetworkAdapters.InsertColumn(2, _T("����·��"), 0, 200);
	
	m_cNetworkAdapters.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	m_cStreamURLs.InsertColumn(0, _T("��·��"), 0, 500);
	m_cStreamURLs.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	m_cRecordInfos.InsertColumn(0, _T("�ļ���"), 0, 300);
	m_cRecordInfos.InsertColumn(1, _T("�����ش�С"), 0, 300);
	m_cRecordInfos.SetExtendedStyle(m_cNetworkAdapters.GetExtendedStyle() | LVS_EX_FULLROWSELECT  | LVS_EX_GRIDLINES);

	// �����ʼ�����绷����ö�ٳ�
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};
	int errCode = pcap_findalldevs(&alldevs, errbuf);
	if (errCode < 0)
	{
		MessageBox(_T("��ȡ������Ϣʧ�ܣ���رճ�������´򿪣�"), _T("����"), MB_OK|MB_ICONERROR);
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CbgSnifferDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CbgSnifferDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CbgSnifferDlg::OnBnClickedBtnStartCapture()
{
	
	char errbuf[PCAP_ERRBUF_SIZE + 1] = {0};

	// ȡ��ѡ�е�����·��
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
			// �ҵ����豸��׼����ʼץ��

			// �ȴ��豸
			pcap_device = pcap_open_live(devs->name, 65536, 1, 1000, errbuf);
			if (pcap_device == nullptr)
			{
				MessageBox(_T("������ʧ�ܣ�"), _T("����"), MB_OK|MB_ICONERROR);
				return ;
			}

			// ����ץ���̣߳�
			CreateThread(nullptr, 0, CaptureThread, this, 0, nullptr);
			
		}
	}
}


void CbgSnifferDlg::OnBnClickedBtnStopCapture()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

// �ο���http://bbs.csdn.net/topics/360090401
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
			// �õ���IP���������
		}

		pcap_dump((unsigned char *)dumpfile, header, pkt_data);
	}
	return 0;
}