#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <string>

struct Tutorial
{
	Tutorial(std::string text, std::string btnText, std::string luaCallback)
		: text(text)
		, buttonText(btnText)
		, luaCallback(luaCallback)
	{}
	std::string text;
	std::string buttonText;
	std::string luaCallback;
};

#endif // TUTORIAL_H
