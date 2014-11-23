#ifndef APPLICATION_H
#define APPLICATION_H

#include <cocos2d.h>

class AppDelegate : private cocos2d::Application
{
public:
	AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching() override;
    virtual void applicationDidEnterBackground() override;
	virtual void applicationWillEnterForeground() override;
};

#endif
