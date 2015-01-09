#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H

#include <functional>
#include <queue>
#include <thread>
#include <mutex>

namespace Utils
{
	/**
	 * @param task callback that will be runned in additional background thread
	 * @param finishCallback callback that will be runned in the main thread on finish all operations in task
	 */
	void RunInBackgroundThread(std::function<void()> task, std::function<void()> finishCallback);
	void RunInBackgroundThread(std::function<void()> task);

	void RunInMainThread(std::function<void()> callback);
}

#endif // THREAD_UTILS_H
