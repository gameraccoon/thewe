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
	Tutorial(const std::string& widgetName,
			 const std::string& text,
			 const std::string& btnText,
			 const std::string& luaCallback)
		: widgetName(widgetName)
		, text(text)
		, buttonText(btnText)
		, luaCallback(luaCallback)
	{}

	Tutorial(const std::string& widgetName,
			 const std::string& text,
			 const std::string& btnText)
		: widgetName(widgetName)
		, text(text)
		, buttonText(btnText)
		, luaCallback("")
	{}

	Tutorial(const std::string& widgetName,
			 const std::string& text)
		: widgetName(widgetName)
		, text(text)
		, buttonText("")
		, luaCallback("")
	{}

	Tutorial(const std::string& widgetName)
		: widgetName(widgetName)
		, text("")
		, buttonText("")
		, luaCallback("")
	{}

	/**
	 * Name of tutorial widget
	 */
	std::string widgetName;

	/**
	 * Main text that will be shown, if widget support it
	 */
	std::string text;

	/**
	 * Button caption that will be shown, if widget support it
	 */
	std::string buttonText;

	/**
	 * Some callback that will be called on some action with tutorial widget.
	 * It can be button or touch callback.
	 */
	std::string luaCallback;
};

#endif // TUTORIAL_H
