#ifndef INVESTIGATOR_H
#define INVESTIGATOR_H

#include "Cell.h"
#include "MiscUtils.h"

class Investigator
{
public:
	struct Brunch
	{
		Brunch *parentBrunch;
		Cell::Ptr cellFrom;
		
		Utils::GameTime timeBegin;
		Utils::GameTime timeEnd;

		std::vector<Brunch> _childBrunches;
	};

	typedef std::vector<Brunch> BrunchBundle;
	typedef std::shared_ptr<Investigator> Ptr;
	typedef std::weak_ptr<Investigator> WeakPtr;

public:
	Investigator(Cell::WeakPtr investigationRoot);

	static Ptr Create(Cell::WeakPtr investigationRoot);

	void BeginInvestigation(void);
	void AbortInvestigation(void);
	void StayInvestigation(bool stay);
	void UpdateToTime(Utils::GameTime time);

	Cell::Ptr GetInvestigationRoot(void) const;
	BrunchBundle GetRootBrunchBundle(void);

private:
	Cell::WeakPtr _investigationRoot;

	BrunchBundle _brunchRoot;
};

#endif