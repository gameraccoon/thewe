#ifndef RESOURCE_CACHE_H
#define RESOURCE_CACHE_H

#include <map>
#include <queue>
#include <mutex>
#include <memory>

#include "Log.h"

template <typename T>
class ResourceCacheQueue
{
public:
	typedef std::shared_ptr<ResourceCacheQueue<T>> Ptr;

public:
	static Ptr Create()
	{
		return std::make_shared<ResourceCacheQueue<T>>();
	}

	void AddResource(const T& resource)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		cachedResources.push(resource);
	}

	T PopFrontResource()
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		T temp = cachedResources.front();
		cachedResources.pop();
		return temp;
	}

	bool IsEmpty() const
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		return cachedResources.empty();
	}

private:
	std::queue<T> cachedResources;
	mutable std::mutex resourcesMutex;
};

template <typename T>
class ResourceCacheMap
{
public:
	typedef std::shared_ptr<ResourceCacheMap<T>> Ptr;

public:
	static Ptr Create()
	{
		return std::make_shared<ResourceCacheMap<T>>();
	}

	void AddResource(const std::string& name, const T& resource)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		cachedResources.insert(std::pair<const std::string, const T>(name, resource));
	}

	T GetResource(const std::string& name) const
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		auto iter = cachedResources.find(name);

		if (iter != cachedResources.end())
		{
			return iter->second;
		}
		else
		{
			WRITE_WARN("Resource by name not found: " + name);
			return T();
		}
	}

	void RemoveResource(const std::string& name)
	{
		std::lock_guard<std::mutex> lock(resourcesMutex);
		cachedResources.erase(name);
	}

private:
	std::map<const std::string, const T> cachedResources;
	mutable std::mutex resourcesMutex;
};

#endif // RESOURCE_CACHE_H
