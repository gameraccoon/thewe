#ifndef LOG_H
#define LOG_H

#include <string>

/**
 * Класс, который отвечает за вывод логов на разных платформах
 * Реализуется посредством синглтона типа феникс
 */
class Log
{
public:
	static Log& Instance();

	void writeError(const std::string& text);
	void writeWarning(const std::string& text);
	void writeLog(const std::string& text);
	void writeInit(const std::string& text);

private:
	/** Вывести строку с штампом времени */
	void writeLine(const std::string& text);

	std::ofstream *logFileStream;

	/** Конструирует информацию о синглтоне */
	static void create();

	/** Функция выполняется при попытке обратиться к уже уничтоженному синглтону */
	static void onDeadReference();

	/** Уничтожить синглтон с возможностью затем восстановить его */
	static void killPhoenixSingletone();

	static Log* singleInstance;

	static bool isDestroyed;
	static bool isFirstLife;

	Log();
	~Log();
	Log(const Log&);
	void operator=(const Log&);
};

#endif