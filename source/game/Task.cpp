#include "Task.h"

Task::Task(const Task::Info* info, float startTime)
{
	_info = info;
	_startTime = startTime;
	_endTime = startTime + info->duration;
}
Task::Ptr Task::Create(const Task::Info* info, float startTime)
{
	return std::make_shared<Task>(Task(info, startTime));
}

bool Task::CheckComleteness(float worldTime)
{
	return (worldTime < _endTime);
}

const Task::Info* Task::GetInfo() const
{
	return _info;
}

float Task::GetStartTime() const
{
	return _startTime;
}

float Task::GetEndTime() const
{
	return _endTime;
}
