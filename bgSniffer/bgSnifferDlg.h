
// bgSnifferDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"

#include "base.h"


// CbgSnifferDlg �Ի���
class CbgSnifferDlg : public CDialogEx
{
// ����
public:
	CbgSnifferDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_BGSNIFFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_cNetworkAdapters;
	CListCtrl m_cStreamURLs;
	CListCtrl m_cRecordInfos;

public:
	pcap_if_t *alldevs;
	pcap_t *pcap_device;
	afx_msg void OnBnClickedBtnStartCapture();
	afx_msg void OnBnClickedBtnStopCapture();

public:
	static DWORD WINAPI CaptureThread(LPVOID lpParam);
};
