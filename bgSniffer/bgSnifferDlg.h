
// bgSnifferDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"

#include "base.h"


// CbgSnifferDlg 对话框
class CbgSnifferDlg : public CDialogEx
{
// 构造
public:
	CbgSnifferDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_BGSNIFFER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
