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

	Cell::WeakPtr cell = World::Instance().GetCellsNetwork().GetCellByInfo(_investigator->GetInvestigationRoot()->GetInfo());
	_invesRootCellWidget = _worldMapLayer->GetCellMapWidget(cell.lock());
	_lastState = _investigator->GetState();

	_investigationDrawer = cocos2d::DrawNode::create();

	addChild(_investigationDrawer, 0);
	scheduleUpdate();

	// pop up investigator catch button in the cell map widget
	if (_investigator->IsStateType(Investigator::START_CATCH_DELAY))
	{
		Cell::WeakPtr cell = World::Instance().GetCellsNetwork().GetCellByInfo(_investigator->GetInvestigationRoot()->GetInfo());
		_invesRootCellWidget = _worldMapLayer->GetCellMapWidget(cell.lock());
		_invesRootCellWidget->ShowInvestigatorLaunchButton(CC_CALLBACK_1(InvestigatorMapWidget::OnCatchInFirstCell, this));
	}

	return true;
}

void InvestigatorMapWidget::update(float dt)
{
	if (_investigator)
	{
		if (_investigator->IsStateType(Investigator::INVESTIGATION) && _lastState == Investigator::START_CATCH_DELAY)
		{
			// player don't catch investigator in first cell, so investigation will be continued
			_invesRootCellWidget->HideInvestigatorLaunchButton(true);
			_lastState = _investigator->GetState();
		}

		_investigationDrawer->clear();
		UpdateInvestigationMap(_investigator->GetRootBranchBundle());
	}
}

void InvestigatorMapWidget::UpdateInvestigationMap(const Investigator::BranchBundle &bundle)
{
	for (const Investigator::Branch &branch : bundle)
	{
		cocos2d::Vec2 from = branch.cellFrom->GetInfo().location;
		cocos2d::Vec2 goal = branch.cellTo->GetInfo().location;

		float dist = from.getDistance(goal) * branch.progressPercentage / 100.0f;

		cocos2d::Vec2 end = from + (goal - from).getNormalized() * dist;

		from = _projector->ProjectOnScreen(from);
		end = _projector->ProjectOnScreen(end);

		_investigationDrawer->drawSegment(from, end, 5.0f, cocos2d::Color4F(1.0f, 0.1f, 0.1f, 0.5f));

		UpdateInvestigationMap(branch.childBrunches);
	}
}

void InvestigatorMapWidget::OnCatchInFirstCell(cocos2d::Ref *sender)
{
	_invesRootCellWidget->HideInvestigatorLaunchButton(false);
	_investigator->AbortInvestigation();

	if (World::Instance().GetTutorialState() == "WaitForCatchingFirstInvestigator")
	{
		World::Instance().RunTutorialFunction("FirstInvestigationCatched");
	}

	World::Instance().RemoveInvestigator(_investigator);
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