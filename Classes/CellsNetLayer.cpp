#include "CellsNetLayer.h"

#include "World.h"
#include "MessageManager.h"
#include "MapGuiLayer.h"
#include "WorldMapLayer.h"

CellsNetLayer::CellsNetLayer()
	: _currentLevel(-1)
	, levelHeight(200.0f)
{
	init();
}

bool CellsNetLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	// adding click events to cell widgets
	cocos2d::Vector<cocos2d::MenuItem*> menuItems;

	for (auto cell : World::Instance().GetCells())
	{
		if (cell->GetParent() == nullptr)
		{
			RecursivelyCreateCellsNetwork(cell, &menuItems, 0);
			break;
		}
	}

	ShowLevel(1);

	cocos2d::Menu *menu = cocos2d::Menu::createWithArray(menuItems);
	menu->setPosition(0.0f, 0.0f);
	addChild(menu);

	setKeyboardEnabled(true);
	setTouchEnabled(true);

	scheduleUpdate();

	_isInMooving = false;

	return true;
}

void CellsNetLayer::update(float dt)
{
	// some updates
	cocos2d::Layer::update(dt);
}

void CellsNetLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		// ToDo: improve this code
		dynamic_cast<WorldMapLayer*>(getParent()->getParent())->SetMapInputEnabled(true);
		this->removeFromParent();
	}
}

void AddMovement(NetCellWidget *item, const cocos2d::Vec2& pos)
{
	float realDistance = (item->getPosition() - pos).length();
	float distance = (item->GetNextPosition() - pos).Size();
	if (distance > 0)
	{
		item->stopAllActions();
		item->SetNextPosition(pos);
		if (realDistance > 0)
		{
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
			cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, distance);
			item->runAction(ease_move);
		}
	}
}

void CellsNetLayer::ShowLevel(int level)
{
	if (level == _currentLevel)
	{
		return;
	}

	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	Vector2 basePoint = origin + Vector2(screen.x / 2, screen.y / 2.0f);
	float indentX = 50.0f;

	HideAllLevels();

	// central part
	auto levelIterators = _cells.equal_range(level);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		AddMovement(cellIterator->second, basePoint);
		basePoint.x += cellIterator->second->getBoundingBox().size.width + indentX;
	}

	// top part
	std::list<NetCellWidget*> childless;
	levelIterators = _cells.equal_range(level - 1);
	float xLocation = 0.0f;
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		// if has children align to center of all his chldren
		if (cellIterator->second->GetChildren().size() > 0)
		{
			xLocation = 0.0f;
			for (auto child : cellIterator->second->GetChildren())
			{
				xLocation += child->GetNextPosition().x;
			}
			xLocation /= cellIterator->second->GetChildren().size();
			AddMovement(cellIterator->second, Vector2(xLocation, basePoint.y + levelHeight));
		}
		else
		{
			childless.push_back(cellIterator->second);
		}
	}
	for (auto cell : childless)
	{
		xLocation += cell->getBoundingBox().size.width + indentX;
		AddMovement(cell, Vector2(xLocation, basePoint.y + levelHeight));
	}

	// bottom part
	levelIterators = _cells.equal_range(level);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		float parentLocationX = cellIterator->second->GetNextPosition().x;
		int childCount = cellIterator->second->GetChildren().size();
		if (childCount > 1)
		{
			int childIndex = 0;
			for (auto cell : cellIterator->second->GetChildren())
			{
				cell->setVisible(true);
				AddMovement(cell, Vector2((parentLocationX - indentX / 4.0f + (indentX * 2.0f * childIndex / (2.0f * childCount))),
														  basePoint.y - levelHeight));
				childIndex++;
			}
		}
		else if (childCount == 1)
		{
			for (auto cell : cellIterator->second->GetChildren())
			{
				cell->setVisible(true);
				AddMovement(cell, Vector2(parentLocationX, basePoint.y - levelHeight));
			}
		}
	}

	_currentLevel = level;
}

void CellsNetLayer::ShowCell(NetCellWidget* cell)
{
	ShowLevel(cell->GetLevel());

	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	Vector2 basePoint = origin + Vector2(screen.x / 2, screen.y / 2.0f);

	Vector2 shift = Vector2(cell->GetNextPosition().x - basePoint.x, 0.0f);

	for (auto cellIterator : _cells)
	{
		if (cellIterator.second->isVisible())
		{
			Vector2 lastPos = cellIterator.second->GetNextPosition();
			AddMovement(cellIterator.second, lastPos - shift);
		}
	}
}

void CellsNetLayer::HideAllLevels()
{
	for (auto cellIterator : _cells)
	{
		cellIterator.second->setVisible(false);
	}
}

NetCellWidget* CellsNetLayer::RecursivelyCreateCellsNetwork(Cell::Ptr rootCell, cocos2d::Vector<cocos2d::MenuItem*>* menuItems, int deepness)
{
	NetCellWidget* rootCellWidget = NetCellWidget::create(rootCell, CC_CALLBACK_1(CellsNetLayer::_CellTouchInputListener, this));
	menuItems->pushBack(rootCellWidget);
	rootCellWidget->setVisible(false);
	rootCellWidget->SetLevel(deepness);
	_cells.insert(std::pair<int, NetCellWidget*>(deepness, rootCellWidget));

	std::vector<Cell::Ptr> children = rootCell->GetChildren();
	for (auto cell : children)
	{
		// recursive call
		NetCellWidget* cellWidget = RecursivelyCreateCellsNetwork(cell, menuItems, deepness + 1);

		rootCellWidget->AddChild(cellWidget);
	}

	return rootCellWidget;
}

void CellsNetLayer::_CellTouchInputListener(cocos2d::Ref *sender)
{
	NetCellWidget* netCellWidget = dynamic_cast<NetCellWidget*>(sender);
	if (netCellWidget != nullptr)
	{
		ShowCell(netCellWidget);
	}
}
