#include "ThreadUtils.h"

#include <thread>
#include <mutex>
#include <queue>

#include <cocos2d.h>

namespace Utils
{
	void RunInBackgroundThread(std::function<void()> task, std::function<void()> finishCallback)
	{
		std::thread thread([task, finishCallback]()
		{
			// start the work
			if (task)
			{
				task();
			}

			// after the work complete notify the main thread
			RunInMainThread(finishCallback);
		});

		// run as a background thread
		thread.detach();
	}

	void RunInBackgroundThread(std::function<void()> task)
	{
		std::thread thread(task);
		// run as a background thread
		thread.detach();
	}

	void RunInMainThread(std::function<void()> callback)
	{
		if (callback)
		{
			cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread(callback);
		}
	}
}
