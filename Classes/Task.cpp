#include "Task.h"

Task::Task(const Task::Info &info)
	: _info(info)
	, _startTime(0)
	, _endTime(0)
{
}

Task::Ptr Task::Create(const Task::Info &info)
{
	return std::make_shared<Task>(info);
}

bool Task::CheckCompleteness(Utils::GameTime worldTime)
{
	return worldTime >= _endTime;
}

void Task::Start(Utils::GameTime startTime)
{
	_startTime = startTime;
	_endTime = startTime + _info.duration;
}

const Task::Info& Task::GetInfo() const
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

void Task::AddExecutant(Member::Ptr executant)
{
	int count_requred = 0;
	int count_present = 0;

	for (const Task::Executant &e : _info.members) {
		if (executant->IsSpecial(e.special)) {
			count_requred = e.count;
		}
	}

	if (count_requred > 0) {
		for (Member::Ptr m : _executants) {
			if (executant->IsSpecial(m->getSpecialization())) {
				++count_present;
			}
		}

		if (count_present < count_requred) {
			executant->SetState(Member::State::MISSION);
			_executants.push_back(executant);
		}
	}
}

void Task::SwapExecutant(Member::Ptr executant, Member::Ptr replaced)
{
	for (std::size_t i=0;i<_executants.size();++i) {
		if (replaced == _executants[i]) {
			_executants[i] = executant;
		}
	}
}

void Task::ReleaseExecutant(void)
{
	for (Member::Ptr m : _executants) {
		m->SetState(Member::State::NORMAL);
	}
	_executants.clear();
}