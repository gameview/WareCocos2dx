#include "main.h"
#include "MainGameApp.h"
#include "CCEGLView.h"


USING_NS_CC;

// uncomment below line, open debug console
#define USE_WIN32_CONSOLE
 
  
int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

#ifdef USE_WIN32_CONSOLE
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
#endif

	std::wstring  str = lpCmdLine;

	CMainGameApp app;
	
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
 
 	if (str == _T("hd"))
	{
		eglView->setFrameSize(1024, 768);
	}
	else
	{
		eglView->setFrameSize(960, 640);
	}

    int ret = CCApplication::sharedApplication()->run();


#ifdef USE_WIN32_CONSOLE
    FreeConsole();
#endif
	
    return ret;
}
