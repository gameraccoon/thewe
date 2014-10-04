#ifndef NET_CELL_WIDGET_H
#define NET_CELL_WIDGET_H

#include <cocos2d.h>
#include "Cell.h"

class NetCellWidget : public cocos2d::Node
{
public:
	explicit NetCellWidget(Cell::WeakPtr cell);

	static NetCellWidget* create(Cell::WeakPtr cell);

	bool IsOutdated() const;

	NetCellWidget* GetParent();
	std::vector<NetCellWidget*> GetChildren();
	void AddChild(NetCellWidget* child);
	void RemoveChild(NetCellWidget* child);

private:
	virtual bool init() override;
	void SetParent(NetCellWidget* parent);

private:
	Cell::WeakPtr _cell;
	NetCellWidget* _parent;
	std::vector<NetCellWidget*> _children;
};
#endif // NET_CELL_WIDGET_H
