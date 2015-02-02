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

		_invesRootCellWidget->ShowInvestigatorLaunchButton([&](){
				this->OnCatchInFirstCell();
			}, [&](){
				this->OnUncachedInFirstCell();
			});
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
		_investigationDrawer->clear();
		for (const Investigator::Branch &branch : _investigator->GetBranches())
		{
			Cell::Ptr cellFromPtr = branch.cellFrom.lock();
			Cell::Ptr cellToPtr = branch.cellTo.lock();

			if (cellFromPtr && cellToPtr)
			{
				cocos2d::Vec2 from = cellFromPtr->GetLocation();
				cocos2d::Vec2 goal = cellToPtr->GetLocation();

				float dist = from.getDistance(goal) * CalcInvestigationBranchDuration(branch);

				cocos2d::Vec2 end = from + (goal - from).getNormalized() * dist;

				from = _projector->ProjectOnScreen(from);
				end = _projector->ProjectOnScreen(end);

				_investigationDrawer->drawSegment(from, end, 5.0f, cocos2d::Color4F(1.0f, 0.1f, 0.1f, 0.5f));
			}
		}
	}
}

void InvestigatorMapWidget::OnCatchInFirstCell()
{
	_invesRootCellWidget->HideInvestigatorLaunchButton();
	_investigator->AbortInvestigation();

	World::Instance().RemoveInvestigator(_investigator);

	_invesRootCellWidget->GetCell().lock()->ReturnToNormalState();
}

void InvestigatorMapWidget::OnUncachedInFirstCell()
{
	_invesRootCellWidget->HideInvestigatorLaunchButton();
	_lastState = _investigator->GetState();
	_investigator->BeginInvestigation();
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
