#include "Log.h"

#include <fstream>
#include <ctime>
#include <assert.h>

#include "FileUtils.h"

Log* Log::singleInstance = nullptr;
bool Log::isDestroyed = false;
bool Log::isFirstLife = true;

Log::Log()
{
	const std::string LOG_FILE = Utils::GetWritablePath() + "log.htm";

	if (this->isFirstLife)
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::trunc);
		this->writeInit("Log file created");
	}
	else
	{
		this->logFileStream = new std::ofstream(LOG_FILE, std::ios_base::app);
	}
}

Log::~Log()
{
	this->logFileStream->close();
	delete this->logFileStream;

	Log::singleInstance = nullptr;
	Log::isDestroyed = true;
	Log::isFirstLife = false;
}

Log& Log::Instance()
{
	if (Log::singleInstance == nullptr)
	{
		if (isDestroyed)
		{
			Log::onDeadReference();
		}
		else
		{
			Log::create();
		}
	}

	return *Log::singleInstance;
}

void Log::create()
{
	static Log theInstance;
	Log::singleInstance = &theInstance;
}

void Log::onDeadReference()
{
	// Получаем "пепел" (расположение старого синглтона)
	Log::create();
	// Создаём на его месте новый синглтон
	new (Log::singleInstance) Log;
	// Помещаем его в очередь на удаление
	atexit(killPhoenixSingletone);
	// Убираем флаг уничтоженности
	Log::isDestroyed = false;
}

void Log::killPhoenixSingletone()
{
	Log::singleInstance->~Log();
	operator delete(Log::singleInstance);
}

void Log::writeError(const std::string& text)
{
	this->writeLine(std::string("<font color=\"red\"><b>Error</b>: ").append(text).append("</font><br/>"));

#ifdef _DEBUG
	assert(false);
#endif
}

void Log::writeWarning(const std::string& text)
{
	this->writeLine(std::string("<font color=\"yellow\"><b>Warning</b>: ").append(text).append("</font><br/>"));

#ifdef _DEBUG
	assert(false);
#endif
}

void Log::writeLog(const std::string& text)
{
	this->writeLine(std::string("<b>Log</b>: ").append(text).append("<br/>"));
}

void Log::writeInit(const std::string& text)
{
	this->writeLine(std::string("<font color=\"green\"><b>Init</b>: ").append(text).append("</font><br/>"));
}

void Log::writeLine(const std::string& text)
{
	if (this->logFileStream->is_open())
	{
		*this->logFileStream << text << std::endl;
	}
}
