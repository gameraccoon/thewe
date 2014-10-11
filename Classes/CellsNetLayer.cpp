#include "CellsNetLayer.h"

#include "World.h"
#include "MessageManager.h"
#include "MapGuiLayer.h"
#include "WorldMapLayer.h"

CellsNetLayer::CellsNetLayer()
	: _currentLevel(-1)
{
	init();
}

bool CellsNetLayer::init(void)
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	// костылём добавляем клики объектам
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
	float distance = (item->getPosition() - pos).length();
	if (distance > 0)
	{
		cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
		cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, distance);
		item->stopAllActions();
		item->runAction(ease_move);
		item->SetNextPosition(pos);
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

	Vector2 basePoint = origin + Vector2(200.0f, screen.y / 2.0f);
	float indentX = 50.0f;

	HideAllLevels();

	auto levelIterators = _cells.equal_range(level);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		AddMovement(cellIterator->second, basePoint);
		basePoint.x += cellIterator->second->getBoundingBox().size.width + indentX;
	}

	levelIterators = _cells.equal_range(level - 1);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		float xLocation = 0.0f;
		if (cellIterator->second->GetChildren().size() > 0)
		{
			for (auto child : cellIterator->second->GetChildren())
			{
				xLocation += child->GetNextPosition().x;
			}
			xLocation /= cellIterator->second->GetChildren().size();
		}

		AddMovement(cellIterator->second, Vector2(xLocation, basePoint.y + 100.0f));
	}

	levelIterators = _cells.equal_range(level + 1);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		AddMovement(cellIterator->second, Vector2(cellIterator->second->GetParent()->GetNextPosition().x, basePoint.y - 100.0f));
	}

	_currentLevel = level;
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
		int level = netCellWidget->GetLevel();
		ShowLevel(level);
	}
}
