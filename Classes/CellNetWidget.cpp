#include "CellNetWidget.h"

#include "Vector2.h"
#include <algorithm>

CellNetWidget::CellNetWidget(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback)
	: _cell(cell)
{
	MenuItem::initWithCallback(callback);
}

CellNetWidget* CellNetWidget::create(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback)
{
	CellNetWidget* ret = new CellNetWidget(cell, callback);
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool CellNetWidget::init()
{
	cocos2d::Sprite *background = cocos2d::Sprite::create("message_background.png");
	background->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
	background->setPosition(0.0f, 0.0f);
	addChild(background);

	Vector2 size = background->getContentSize();
	Cell::Ptr cell = _cell.lock();
	if (cell)
	{
		cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
		cocos2d::Label *text = cocos2d::Label::createWithTTF(ttfConfig, cell->GetInfo().town.lock()->GetInfo().name, cocos2d::TextHAlignment::CENTER);
		Vector2 center = size / 2;
		text->setPosition(center);
		addChild(text);
	}

	setContentSize(size);
	setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

	return true;
}

bool CellNetWidget::IsOutdated() const
{
	return _cell.expired();
}

CellNetWidget* CellNetWidget::GetParent()
{
	return _parent;
}

void CellNetWidget::SetParent(CellNetWidget* parent)
{
	_parent = parent;
}

std::vector<CellNetWidget*> CellNetWidget::GetChildren()
{
	return _children;
}

void CellNetWidget::AddChild(CellNetWidget* child)
{
	_children.push_back(child);
	child->SetParent(this);
}

void CellNetWidget::RemoveChild(CellNetWidget* child)
{
	_children.erase(std::find(_children.begin(), _children.end(), child));
	child->setParent(nullptr);
}

void CellNetWidget::SetLevel(int level)
{
	_level = level;
}

int CellNetWidget::GetLevel()
{
	return _level;
}

void CellNetWidget::SetNextPosition(const Vector2& nextPos)
{
	_nextPosition = nextPos;
}

Vector2 CellNetWidget::GetNextPosition() const
{
	return _nextPosition;
}
