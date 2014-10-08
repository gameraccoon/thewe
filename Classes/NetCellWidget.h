#ifndef NET_CELL_WIDGET_H
#define NET_CELL_WIDGET_H

#include <cocos2d.h>
#include "Cell.h"

class NetCellWidget : public cocos2d::MenuItem
{
public:
	explicit NetCellWidget(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback);

	static NetCellWidget* create(Cell::WeakPtr cell, const cocos2d::ccMenuCallback& callback);

	bool IsOutdated() const;

	NetCellWidget* GetParent();
	std::vector<NetCellWidget*> GetChildren();
	void AddChild(NetCellWidget* child);
	void RemoveChild(NetCellWidget* child);
	void SetLevel(int level);
	int GetLevel();

private:
	virtual bool init() override;
	void SetParent(NetCellWidget* parent);

private:
	Cell::WeakPtr _cell;
	NetCellWidget* _parent;
	std::vector<NetCellWidget*> _children;
	int _level;
};
#endif // NET_CELL_WIDGET_H
