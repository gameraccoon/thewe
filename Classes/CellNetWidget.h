#ifndef CELL_NET_WIDGET_H
#define CELL_NET_WIDGET_H

#include <cocos2d.h>
#include "Cell.h"

class CellNetWidget : public cocos2d::MenuItem
{
public:
	explicit CellNetWidget(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback);

	static CellNetWidget* create(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback);

	bool IsOutdated() const;

	CellNetWidget* GetParent();
	std::vector<CellNetWidget*> GetChildren();
	void AddChild(CellNetWidget* child);
	void RemoveChild(CellNetWidget* child);
	void SetLevel(int level);
	int GetLevel();
	void SetNextPosition(const Vector2& nextPos);
	Vector2 GetNextPosition() const;

private:
	virtual bool init() override;
	void SetParent(CellNetWidget* parent);

private:
	Cell::WeakPtr _cell;
	CellNetWidget* _parent;
	std::vector<CellNetWidget*> _children;
	int _level;
	Vector2 _nextPosition;
};
#endif // CELL_NET_WIDGET_H
