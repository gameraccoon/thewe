#include "Drawable.h"

#include <cocos2d.h>

Drawable::Drawable(cocos2d::Node* node)
{
	// умный указатель вместо delete будет вызывать release
	std::function<void(cocos2d::Node*)> del = [](cocos2d::Node* nodeToDelete)
	{
		nodeToDelete->release();
	};

	_node = NodePtr(node, del);
}

Drawable::~Drawable()
{
}

Drawable::Ptr Drawable::CastFromCocos(cocos2d::Node *node)
{
	return std::make_shared<Drawable>(Drawable(node));
}

void Drawable::SetPosition(Vector2 location)
{
	_node->setPosition(location);
}

void Drawable::SetScale(float scale)
{
	_node->setScale(scale);
}