#include "NetCellWidget.h"

#include "Vector2.h"
#include <algorithm>

NetCellWidget::NetCellWidget(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback)
	: _cell(cell)
{
	MenuItem::initWithCallback(callback);
}

NetCellWidget* NetCellWidget::create(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback)
{
	NetCellWidget* ret = new NetCellWidget(cell, callback);
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

bool NetCellWidget::init()
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

bool NetCellWidget::IsOutdated() const
{
	return _cell.expired();
}

NetCellWidget* NetCellWidget::GetParent()
{
	return _parent;
}

void NetCellWidget::SetParent(NetCellWidget* parent)
{
	_parent = parent;
}

std::vector<NetCellWidget*> NetCellWidget::GetChildren()
{
	return _children;
}

void NetCellWidget::AddChild(NetCellWidget* child)
{
	_children.push_back(child);
	child->SetParent(this);
}

void NetCellWidget::RemoveChild(NetCellWidget* child)
{
	_children.erase(std::find(_children.begin(), _children.end(), child));
	child->setParent(nullptr);
}

void NetCellWidget::SetLevel(int level)
{
	_level = level;
}

int NetCellWidget::GetLevel()
{
	return _level;
}
