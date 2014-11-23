#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <string>
#include <memory>

class Tutorial
{
public:
	typedef std::shared_ptr<Tutorial> Ptr;
	typedef std::weak_ptr<Tutorial> WeakPtr;

public:
	Tutorial(std::string text, std::string btnText, std::string luaCallback)
		: text(text)
		, buttonText(btnText)
		, luaCallback(luaCallback)
	{}

	Tutorial(std::string text, std::string btnText)
		: text(text)
		, buttonText(btnText)
		, luaCallback("")
	{}

	std::string text;
	std::string buttonText;
	std::string luaCallback;
};

#endif // TUTORIAL_H
