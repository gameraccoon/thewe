#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <string>

struct Tutorial
{
	Tutorial(std::string text, std::string btnText)
		: text(text)
		, buttonText(btnText)
	{}
	std::string text;
	std::string buttonText;
};

#endif // TUTORIAL_H
