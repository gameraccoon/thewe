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
		Cell::Ptr cellFrom;
		Cell::Ptr cellTo;
		
		Utils::GameTime timeBegin;
		Utils::GameTime timeEnd;
		Utils::GameTime timeDuration;

		float progressPercentage;

		BranchBundle childBrunches;
	};

public:
	Investigator(Cell::WeakPtr investigationRoot);

	static Ptr Create(Cell::WeakPtr investigationRoot);

	void BeginInvestigation(void);
	void AbortInvestigation(void);
	void StayInvestigation(bool stay);
	void UpdateToTime(Utils::GameTime time);

	Cell::Ptr GetInvestigationRoot(void) const;
	const BranchBundle& GetRootBranchBundle(void);

	int GetUid(void) const;

private:
	void UpdateBranchesRecurcively(BranchBundle &bundle, Utils::GameTime time);

	Cell::WeakPtr _investigationRoot;

	BranchBundle _branchRoot;

	int _uid;
};

#endif