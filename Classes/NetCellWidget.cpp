#include "NetCellWidget.h"

#include "Vector2.h"
#include <algorithm>

NetCellWidget::NetCellWidget(Cell::WeakPtr cell)
	: _cell(cell)
{
}

NetCellWidget* NetCellWidget::create(Cell::WeakPtr cell)
{
	NetCellWidget* ret = new NetCellWidget(cell);
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
	if (!cocos2d::Node::init())
	{
		return false;
	}

	cocos2d::Sprite *background = cocos2d::Sprite::create("message_background.png");
	background->setPosition(0.0f, 0.0f);
	background->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	addChild(background);

	Cell::Ptr cell = _cell.lock();
	if (cell)
	{
		cocos2d::TTFConfig ttfConfig("arial.ttf", 18);
		cocos2d::Label *text = cocos2d::Label::createWithTTF(ttfConfig, cell->GetInfo().town.lock()->GetInfo().name, cocos2d::TextHAlignment::CENTER);
		cocos2d::Rect rect = background->getBoundingBox();
		Vector2 center = Vector2(0.0f, 0.0f);
		text->setPosition(center);
		addChild(text);
	}

	setContentSize(background->getContentSize());

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
