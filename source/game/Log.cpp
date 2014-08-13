#include "Log.h"

#include <fstream>
#include <ctime>

Log* Log::singleInstance = nullptr;
bool Log::isDestroyed = false;
bool Log::isFirstLife = true;

Log::Log()
{
	const std::string LOG_FILE = "./log.txt";

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
	this->writeLine(std::string(" Error: ").append(text));
}

void Log::writeWarning(const std::string& text)
{
	this->writeLine(std::string(" Warning: ").append(text));
}

void Log::writeLog(const std::string& text)
{
	this->writeLine(std::string(" Log: ").append(text));
}

void Log::writeInit(const std::string& text)
{
	this->writeLine(std::string(" Init: ").append(text));
}

void Log::writeLine(const std::string& text)
{
	if (this->logFileStream->is_open())
	{
		char dateStr[9];
		char timeStr[9];
		_strdate_s(dateStr);
		*this->logFileStream << dateStr << " ";
		_strtime_s(timeStr);
		*this->logFileStream << timeStr;
		*this->logFileStream << text << std::endl;
	}
}
