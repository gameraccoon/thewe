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
		typedef std::weak_ptr<Branch> WeakPtr;

		Cell::WeakPtr cellFrom;
		Cell::WeakPtr cellTo;
		
		Utils::GameTime timeBegin;
		Utils::GameTime timeDuration;
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

	static Ptr Create(Cell::WeakPtr investigationRoot);

	void InitInvestigator(const Branches &branches);
	void BeginCatchTime(float time);
	void BeginInvestigation(void);
	void AbortInvestigation(void);
	void StayInvestigation(bool stay);
	void UpdateToTime(Utils::GameTime time);
	
	bool IsStateType(Investigator::State state) const;
	bool IsCellUnderInvestigation(Cell::WeakPtr cell) const;

	void CancelInvestigationTo(Cell::WeakPtr cell);
	
	Cell::WeakPtr GetInvestigationRoot(void) const;
	const Branches& GetBranches(void) const;
	Investigator::State GetState(void) const;

	int GetUid(void) const;

private:
	Investigator(unsigned int uid);
	void CaptureCell(Cell::WeakPtr cellTarget, Cell::WeakPtr cellFrom);

	Cell::WeakPtr _investigationRoot;

	Branches _activeBranches;

	Utils::GameTime _catchTimeBegin;
	Utils::GameTime _catchTimeEnd;
	Investigator::State _state;
	
	unsigned int _uid;
};

#endif
