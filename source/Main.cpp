#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <tchar.h>
#endif

#include "CCStdC.h"
#include "CCEGLView.h"
#include "game\Application.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
#elif
#error unknown target platform
#endif
{
    AppDelegate app;
   
	cocos2d::CCEGLView* eglView = cocos2d::CCEGLView::sharedOpenGLView();
	eglView->setViewName("Codename: The We");
    eglView->setFrameSize(2048, 1536);
    eglView->setFrameZoomFactor(0.4f);
    
	return cocos2d::CCApplication::sharedApplication()->run();
}