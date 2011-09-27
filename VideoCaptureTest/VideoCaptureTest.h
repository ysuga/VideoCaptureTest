// VideoCaptureTest.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CVideoCaptureTestApp:
// このクラスの実装については、VideoCaptureTest.cpp を参照してください。
//

class CVideoCaptureTestApp : public CWinApp
{
public:
	CVideoCaptureTestApp();

// オーバーライド
	public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CVideoCaptureTestApp theApp;