#include "Task.h"

Task::Task(const Task::Info* info, float startTime)
	: _info(info)
	, _startTime(startTime)
	, _endTime(startTime + info->duration)
	, _isAborted(false)
	, _isFastFinished(false)
{
}

Task::Ptr Task::Create(const Task::Info* info, float startTime)
{
	return std::make_shared<Task>(info, startTime);
}

bool Task::CheckCompleteness(float worldTime)
{
	return (worldTime >= _endTime || _isAborted || _isFastFinished);
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

float Task::CalculateProgress(float worldTime) const
{
	return  1.0f - ((_endTime - worldTime) / (_endTime - _startTime));
}

bool Task::IsAborted() const
{
	return _isAborted;
}

bool Task::IsFastFinished() const 
{
	return _isFastFinished;
}

void Task::Abort()
{
	_isAborted = true;
}

void Task::FastFinish()
{
	_isFastFinished = true;
}
