#include "CellsNetLayer.h"

#include "World.h"
#include "MessageManager.h"

CellsNetLayer::CellsNetLayer()
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

	ShowLevel(2);

	cocos2d::Menu *menu = cocos2d::Menu::createWithArray(menuItems);
	menu->setPosition(0.0f, 0.0f);
	addChild(menu);

	setKeyboardEnabled(true);
	setTouchEnabled(true);

	return true;
}

void CellsNetLayer::onKeyReleased(cocos2d::EventKeyboard::KeyCode key, cocos2d::Event *event)
{
	if (key == cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE)
	{
		// Close self
	}
}

void CellsNetLayer::ShowLevel(int level)
{
	Vector2 screen = cocos2d::Director::getInstance()->getVisibleSize();
	Vector2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	Vector2 basePoint = origin + Vector2(200.0f, screen.y / 2.0f);
	float indentX = 50.0f;

	HideAllLevels();

	auto levelIterators = _cells.equal_range(level);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		cellIterator->second->setPosition(basePoint);
		basePoint.x += cellIterator->second->getBoundingBox().size.width + indentX;
	}

	levelIterators = _cells.equal_range(level - 1);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		float xLocation = 0.0f;
		for (auto child : cellIterator->second->GetChildren())
		{
			xLocation += child->getPosition().x;
		}
		xLocation /= cellIterator->second->GetChildren().size();

		cellIterator->second->setPosition(Vector2(xLocation, basePoint.y + 100.0f));
		basePoint.x += cellIterator->second->getBoundingBox().size.width;
	}

	levelIterators = _cells.equal_range(level + 1);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		cellIterator->second->setPosition(Vector2(cellIterator->second->GetParent()->getPosition().x, basePoint.y - 100.0f));
		basePoint.x += cellIterator->second->getBoundingBox().size.width;
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
	//addChild(rootCellWidget);
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
