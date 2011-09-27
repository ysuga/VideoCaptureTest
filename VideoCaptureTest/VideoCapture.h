#pragma once

#include <dshow.h>

class CVideoCapture
{
private:
	IGraphBuilder			*m_pGraphBuilder;
	ICreateDevEnum			*m_pDevEnum;
	IEnumMoniker			*m_pEnumMoniker;
	IMoniker				*m_pMoniker;
	ULONG					m_cFetched;
	IBaseFilter				*m_pBaseFilter;
	ICaptureGraphBuilder2	*m_pCaptureGraphBuilder2;
	IMediaControl			*m_pMediaControl;
	IVideoWindow			*m_pVideoWindow;

private:
	IPin* GetPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir);
	void InitVideoFilters(void);
	void StartRender(HWND hWnd);
	void ShowFilterProperty(IUnknown* pFilter);

public:
	CVideoCapture(HWND hWnd = NULL);
	~CVideoCapture(void);

	void SetWindowSize(long width, long height);
};
