#include "InvestigatorMapWidget.h"

#include "CellMapWidget.h"
#include "WorldMapLayer.h"

InvestigatorMapWidget::InvestigatorMapWidget(Investigator::Ptr investigator, MapProjector *proj, WorldMapLayer *worldMapLayer)
	: _investigator(investigator)
	, _worldMapLayer(worldMapLayer)
	, _projector(proj)
	, _invesRootCellWidget(nullptr)
	, _projectorUid(-1)
	, _investigatorUid(investigator->GetUid())
{
	init();
}

bool InvestigatorMapWidget::init(void)
{	
	if (!_investigator) {
		return false;
	}

	Cell::WeakPtr cell = _investigator->GetInvestigationRoot();
	_invesRootCellWidget = _worldMapLayer->GetCellMapWidget(cell.lock());
	_lastState = _investigator->GetState();

	_investigationDrawer = cocos2d::DrawNode::create();

	addChild(_investigationDrawer, 0);
	scheduleUpdate();

	// pop up investigator catch button in the cell map widget
	if (_investigator->IsStateType(Investigator::State::START_CATCH_DELAY))
	{
		Cell::WeakPtr cell = _investigator->GetInvestigationRoot();
		_invesRootCellWidget = _worldMapLayer->GetCellMapWidget(cell.lock());
		_invesRootCellWidget->ShowInvestigatorLaunchButton(CC_CALLBACK_1(InvestigatorMapWidget::OnCatchInFirstCell, this));
	}

	return true;
}

float CalcInvestigationBranchDuration(const Investigator::Branch& branch)
{
	float timeElapsed = Utils::GetGameTime() - branch.timeBegin;
	return timeElapsed / branch.timeDuration;
}

void InvestigatorMapWidget::update(float dt)
{
	if (_investigator)
	{
		if (_investigator->IsStateType(Investigator::State::INVESTIGATION) && _lastState == Investigator::State::START_CATCH_DELAY)
		{
			// player don't catch investigator in first cell, so investigation will be continued
			_invesRootCellWidget->HideInvestigatorLaunchButton(true);
			_lastState = _investigator->GetState();
		}

		_investigationDrawer->clear();
		for (const Investigator::Branch &branch : _investigator->GetBranches())
		{
			Cell::Ptr cellFromPtr = branch.cellFrom.lock();
			Cell::Ptr cellToPtr = branch.cellTo.lock();

			if (cellFromPtr && cellToPtr)
			{	
				cocos2d::Vec2 from = cellFromPtr->GetInfo().location;
				cocos2d::Vec2 goal = cellToPtr->GetInfo().location;

				float dist = from.getDistance(goal) * CalcInvestigationBranchDuration(branch);

				cocos2d::Vec2 end = from + (goal - from).getNormalized() * dist;

				from = _projector->ProjectOnScreen(from);
				end = _projector->ProjectOnScreen(end);

				_investigationDrawer->drawSegment(from, end, 5.0f, cocos2d::Color4F(1.0f, 0.1f, 0.1f, 0.5f));
			}
		}
	}
}

void InvestigatorMapWidget::OnCatchInFirstCell(cocos2d::Ref *sender)
{
	_invesRootCellWidget->HideInvestigatorLaunchButton(false);
	_investigator->AbortInvestigation();

	if (World::Instance().GetTutorialManager().IsTutorialStateAvailable("WaitForCatchingFirstInvestigator"))
	{
		World::Instance().GetTutorialManager().RunTutorialFunction("FirstInvestigationCatched");
	}

	World::Instance().RemoveInvestigator(_investigator);

	_invesRootCellWidget->GetCell().lock()->BeginDestruction();
}

Investigator::Ptr InvestigatorMapWidget::GetInvestigator(void) const
{
	return _investigator;
}

void InvestigatorMapWidget::SetProjectorUid(int uid)
{
	_projectorUid = uid;
}

int InvestigatorMapWidget::GetProjectorUid(void) const
{
	return _projectorUid;
}

int InvestigatorMapWidget::GetInvestigatorUid(void) const
{
	return _investigatorUid;
}
