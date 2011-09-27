#include "StdAfx.h"
#include "videocapture.h"

void CVideoCapture::InitVideoFilters(void)
{
	HRESULT hr;
	CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void **)&m_pGraphBuilder);

	CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC, IID_ICreateDevEnum, (void **)&m_pDevEnum);

	m_pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &m_pEnumMoniker, 0);

	if (m_pEnumMoniker->Next(1, &m_pMoniker, &m_cFetched) == S_OK){
    // 最初のモニカをフィルタオブジェクトにバインドする
		m_pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void **)&m_pBaseFilter);
		m_pMoniker->Release();
	}

	m_pGraphBuilder->AddFilter(m_pBaseFilter, L"Video Capture");
	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow, (void**)&m_pVideoWindow);
}

void CVideoCapture::StartRender(HWND hWnd)
{
	HRESULT hr;
	CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void **)&m_pCaptureGraphBuilder2);
	m_pCaptureGraphBuilder2->SetFiltergraph(m_pGraphBuilder);

	m_pCaptureGraphBuilder2->RenderStream( &PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, m_pBaseFilter, NULL, NULL );

	if(hWnd != NULL) {
		hr = m_pVideoWindow->put_Owner((OAHWND)hWnd);
		hr = m_pVideoWindow->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
		hr = m_pVideoWindow->put_MessageDrain((OAHWND)hWnd);
		long Width, Height;
		RECT rect;
		GetClientRect(hWnd, &rect);
		Width = rect.right;
		Height = rect.bottom;
		SetWindowSize(Width, Height);
	}

	m_pGraphBuilder->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
	m_pMediaControl->Run();
}

void CVideoCapture::SetWindowSize(long Width, long Height)
{
	HRESULT hr;
	hr = m_pVideoWindow->SetWindowPosition(0, 0, Width, Height);
}

CVideoCapture::CVideoCapture(HWND hWnd /*=NULL*/)
{
	CoInitialize(NULL);
//	HRESULT hr;

	InitVideoFilters();

	IPin *pPin = GetPin(m_pBaseFilter, PINDIR_OUTPUT);
	ShowFilterProperty(pPin);
	StartRender(hWnd);
}

CVideoCapture::~CVideoCapture(void)
{
	m_pVideoWindow->Release();
	m_pBaseFilter->Release();
	m_pMediaControl->Release();
	m_pCaptureGraphBuilder2->Release();
	m_pEnumMoniker->Release();
	m_pDevEnum->Release();
	m_pGraphBuilder->Release();

	CoUninitialize();
}

void CVideoCapture::ShowFilterProperty(IUnknown *pFilter)
{
	HRESULT hr;
	ISpecifyPropertyPages *pSpecify;
	hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pSpecify);
	if (SUCCEEDED(hr)) {
		//FILTER_INFO FilterInfo;
		//pFilter->QueryFilterInfo(&FilterInfo);
		CAUUID caGUID;
		pSpecify->GetPages(&caGUID);
		pSpecify->Release();

        OleCreatePropertyFrame(
			NULL,                   // 親ウィンドウ
			0,                      // x (予約済み)
			0,                      // y (予約済み)
			NULL,//FilterInfo.achName,     // ダイアログ ボックスのキャプション
			1,                      // フィルタの数
			(IUnknown **)&pFilter,  // フィルタへのポインタ
			caGUID.cElems,          // プロパティ ページの数
			caGUID.pElems,          // プロパティ ページ CLSID へのポインタ
			0,                      // ロケール識別子
			0,                      // 予約済み
			NULL                    // 予約済み
			);
		CoTaskMemFree(caGUID.pElems);
		//FilterInfo.pGraph->Release();
	}
}

IPin* CVideoCapture::GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir) 
{
	BOOL       bFound = FALSE;
	IEnumPins  *pEnum;
	IPin       *pPin;

	pFilter->EnumPins(&pEnum);
	while(pEnum->Next(1, &pPin, 0) == S_OK) {
		PIN_DIRECTION PinDirThis; 
		pPin->QueryDirection(&PinDirThis);
		if (bFound = (PinDir == PinDirThis)) // 引数で指定した方向のピンならbreak
			break;
		pPin->Release();
	}
	pEnum->Release();
	return (bFound ? pPin : 0);
}

