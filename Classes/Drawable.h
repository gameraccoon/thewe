#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <memory>
#include "Vector2.h"

// dispose of necessity of cocos2d include
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
	/** Factory */
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