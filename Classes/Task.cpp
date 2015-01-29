#include "Task.h"

Task::Task(const Task::Info* info, Utils::GameTime startTime)
	: _info(info)
	, _startTime(startTime)
	, _endTime(startTime + info->duration)
{
}

Task::Ptr Task::Create(const Task::Info* info, Utils::GameTime startTime)
{
	return std::make_shared<Task>(info, startTime);
}

bool Task::CheckCompleteness(Utils::GameTime worldTime)
{
	return worldTime >= _endTime;
}

const Task::Info* Task::GetInfo() const
{
	return _info;
}

Utils::GameTime Task::GetStartTime() const
{
	return _startTime;
}

Utils::GameTime Task::GetEndTime() const
{
	return _endTime;
}

float Task::CalculateProgress(Utils::GameTime worldTime) const
{
	if (worldTime < _endTime)
	{
		float allTime = _endTime - _startTime;
		float eta = _endTime - worldTime;
		return  1.0f - eta / allTime;
	}
	else
	{
		return 1.0f;
	}
}

