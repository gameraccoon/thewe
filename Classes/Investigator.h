#ifndef INVESTIGATOR_H
#define INVESTIGATOR_H

#include "Cell.h"
#include "MiscUtils.h"

class Investigator
{
public:
	struct Branch;
	typedef std::vector<Branch> BranchBundle;
	typedef std::shared_ptr<Investigator> Ptr;
	typedef std::weak_ptr<Investigator> WeakPtr;

	struct Branch
	{
		Branch *parentBrunch;
		Cell *cellFrom;
		Cell *cellTo;
		
		Utils::GameTime timeBegin;
		Utils::GameTime timeEnd;
		Utils::GameTime timeDuration;

		float progressPercentage;

		BranchBundle childBrunches;
	};

	enum State
	{
		START_CATCH_DELAY,
		INVESTIGATION,
		STAYING,
		ABORTED
	};

public:
	Investigator(Cell::WeakPtr investigationRoot);
	Investigator(const Investigator::BranchBundle &rootBranchBudle);

	static Ptr Create(Cell::WeakPtr investigationRoot);
	static Ptr Create(const Investigator::BranchBundle &rootBranchBudle);

	void InitInvestigator(const Investigator::BranchBundle &rootBranchBudle);
	void BeginCatchTime(float time);
	void BeginInvestigation(void);
	void AbortInvestigation(void);
	void StayInvestigation(bool stay);
	void UpdateToTime(Utils::GameTime time);
	
	bool IsStateType(Investigator::State state) const;
	
	Cell::Ptr GetInvestigationRoot(void) const;
	const BranchBundle& GetRootBranchBundle(void);
	Investigator::State GetState(void) const;

	int GetUid(void) const;

private:
	void UpdateBranchesRecurcively(BranchBundle &bundle, Utils::GameTime time);

	Cell::WeakPtr _investigationRoot;

	BranchBundle _branchRoot;

	Utils::GameTime _catchTimeBegin;
	Utils::GameTime _catchTimeEnd;
	Investigator::State _state;
	
	int _uid;
};

#endif