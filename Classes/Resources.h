#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <map>
#include <vector>

struct Resource
{
	typedef int Count;
	typedef std::map<const std::string, Resource> Map;
	typedef std::vector<Resource> Vector;

	Resource(std::string name, Count count)
		: name(name)
		, count(count)
	{}

	Resource(const Resource& resource)
		: name(resource.name)
		, count(resource.count)
	{}

	const std::string name;
	Count count;
};

#endif // RESOURCES_H
