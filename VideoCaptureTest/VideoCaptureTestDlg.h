// VideoCaptureTestDlg.h : ヘッダー ファイル
//

#pragma once

#include "VideoCapture.h"
#include "afxwin.h"


// CVideoCaptureTestDlg ダイアログ
class CVideoCaptureTestDlg : public CDialog
{
// コンストラクション
public:
	CVideoCaptureTestDlg(CWnd* pParent = NULL);	// 標準コンストラクタ

// ダイアログ データ
	enum { IDD = IDD_VIDEOCAPTURETEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	CVideoCapture *m_pVideoCapture;

	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_Picture1;
};
