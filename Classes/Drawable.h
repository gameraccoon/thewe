#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include "Vector2.h"

// избавляемся от необходимости включать все заголовки cocos2d
namespace cocos2d
{
	class Node;
}

class Drawable
{
public:
	typedef std::shared_ptr<Drawable> Ptr;
public:
	~Drawable();
	/** Фабрика */
	static Drawable::Ptr CastFromCocos(cocos2d::Node *node);

	void SetPosition(Vector2 location);
	void SetScale(float scale);

private:
	typedef std::shared_ptr<cocos2d::Node> NodePtr;
private:
	Drawable(cocos2d::Node *node);

	NodePtr _node;
};

#endif // DRAWABLE_H