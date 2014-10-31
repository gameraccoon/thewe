#ifndef INVESTIGATOR_H
#define INVESTIGATOR_H

#include "Cell.h"
#include "MiscUtils.h"

class Investigator
{
public:
	struct Branch;
	typedef std::vector<Branch> Branches;
	typedef std::shared_ptr<Investigator> Ptr;
	typedef std::weak_ptr<Investigator> WeakPtr;

	struct Branch
	{
		Branch *parentBranch;
		Cell *cellFrom;
		Cell *cellTo;
		
		Utils::GameTime timeBegin;
		Utils::GameTime timeEnd;
		Utils::GameTime timeDuration;

		float progressPercentage;
	};

	enum class State
	{
		START_CATCH_DELAY,
		INVESTIGATION,
		STAYING,
		ABORTED
	};

public:
	Investigator(Cell::WeakPtr investigationRoot);
	Investigator(Cell::WeakPtr investigationRoot, const Branches &branches);

	static Ptr Create(Cell::WeakPtr investigationRoot);
	static Ptr Create(Cell::WeakPtr investigationRoot, const Branches &branches);

	void InitInvestigator(const Branches &branches);
	void BeginCatchTime(float time);
	void BeginInvestigation(void);
	void AbortInvestigation(void);
	void StayInvestigation(bool stay);
	void UpdateToTime(Utils::GameTime time);
	
	bool IsStateType(Investigator::State state) const;
	bool IsCellUnderInvestigation(Cell::Ptr cell) const;

	void CancelInvestigationTo(Cell::Ptr cell);
	
	Cell::Ptr GetInvestigationRoot(void) const;
	const Branches& GetBranches(void) const;
	Investigator::State GetState(void) const;

	int GetUid(void) const;

private:
	void CaptureCell(Cell *cellTarget, Cell *cellFrom);

	Cell::WeakPtr _investigationRoot;

	Branches _activeBranches;

	Utils::GameTime _catchTimeBegin;
	Utils::GameTime _catchTimeEnd;
	Investigator::State _state;
	
	int _uid;
};

#endif
