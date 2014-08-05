#include "Drawable.h"

Drawable::Drawable(cocos2d::CCNode* node)
{
	// умный указатель вместо delete будет вызывать release
	std::function<void(cocos2d::CCNode*)> del = [](cocos2d::CCNode* nodeToDelete)
	{
		nodeToDelete->release();
	};

	_node = NodePtr(node, del);
}

Drawable::~Drawable()
{
}

Drawable::Ptr Drawable::CastFromCocos(cocos2d::CCNode *node)
{
	return std::make_shared<Drawable>(Drawable(node));
}

void Drawable::SetPosition(Point location)
{
	_node->setPosition(location);
}

void Drawable::SetScale(float scale)
{
	_node->setScale(scale);
}