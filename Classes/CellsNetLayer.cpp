#include "CellsNetLayer.h"

#include "World.h"

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

	for (auto cell : World::Instance().GetCells())
	{
		if (cell->GetParent() == nullptr)
		{
			RecurcivelyCreateCellsNetwork(cell, 0);
			break;
		}
	}

	ShowLevel(1);

	setKeyboardEnabled(true);

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
	Vector2 center = origin + screen / 2.0f;

	Vector2 basePoint = origin + Vector2(100.0f, screen.y / 2.0f);

	auto levelIterators = _cells.equal_range(level);
	for (auto cellIterator = levelIterators.first; cellIterator != levelIterators.second; ++cellIterator)
	{
		cellIterator->second->setVisible(true);
		cellIterator->second->setPosition(basePoint);
		basePoint.x += cellIterator->second->getBoundingBox().size.width;
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

NetCellWidget* CellsNetLayer::RecurcivelyCreateCellsNetwork(Cell::Ptr rootCell, int deepness)
{
	NetCellWidget* rootCellWidget = NetCellWidget::create(rootCell);
	addChild(rootCellWidget);
	rootCellWidget->setVisible(false);
	_cells.insert(std::pair<int, NetCellWidget*>(deepness, rootCellWidget));

	std::vector<Cell::Ptr> children = rootCell->GetChildren();
	for (auto cell : children)
	{
		// recursive call
		NetCellWidget* cellWidget = RecurcivelyCreateCellsNetwork(cell, deepness + 1);

		rootCellWidget->AddChild(cellWidget);
	}

	return rootCellWidget;
}
