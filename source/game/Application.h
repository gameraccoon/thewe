#ifndef APPLICATION_H
#define APPLICATION_H

#include "cocos2d.h"

class AppDelegate : private cocos2d::CCApplication
{
public:
	AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching();
    virtual void applicationDidEnterBackground();
    virtual void applicationWillEnterForeground();
};

#endif