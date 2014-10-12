#include "InvestigatorMapWidget.h"

InvestigatorMapWidget::InvestigatorMapWidget(Investigator::Ptr investigator, MapProjector *proj)
	: _investigator(investigator)
	, _projector(proj)
	, _projectorUid(-1)
{
	init();
}

bool InvestigatorMapWidget::init(void)
{
	_investigationDrawer = cocos2d::DrawNode::create();

	addChild(_investigationDrawer, 0);
	scheduleUpdate();

	return true;
}

void InvestigatorMapWidget::update(float dt)
{
	_investigationDrawer->clear();

	for (const Investigator::Branch &branch : _investigator->GetRootBranchBundle())
	{
		cocos2d::Vec2 from = branch.cellFrom->GetInfo().location;
		cocos2d::Vec2 goal = branch.cellTo->GetInfo().location;

		float dist = from.getDistance(goal) * branch.progressPercentage / 100.0f;

		cocos2d::Vec2 end = from + (goal - from).getNormalized() * dist;

		from = _projector->ProjectOnScreen(from);
		end = _projector->ProjectOnScreen(end);

		_investigationDrawer->drawSegment(from, end, 5.0f, cocos2d::Color4F(1.0f, 0.1f, 0.1f, 0.5f));
	}
}

void InvestigatorMapWidget::UpdateInvestigationMap(const Investigator::BranchBundle &bundle)
{
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