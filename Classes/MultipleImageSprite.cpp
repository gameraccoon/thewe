#include "MultipleImageSprite.h"

#include "Log.h"

MultipleImageSprite::MultipleImageSprite()
{
}

MultipleImageSprite* MultipleImageSprite::create()
{
	MultipleImageSprite* ret = new MultipleImageSprite();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

bool MultipleImageSprite::init(void)
{
	return true;
}

void MultipleImageSprite::setOpacity(GLubyte opacity)
{
	for (std::map<std::string, cocos2d::Sprite *>::iterator it = _images.begin(); it != _images.end(); ++it) {
		(*it).second->setOpacity(opacity);
	}

	cocos2d::Node::setOpacity(opacity);
}

void MultipleImageSprite::AddImage(const std::string &id, const std::string &filename)
{
	cocos2d::Sprite *s = cocos2d::Sprite::create(filename);
	std::pair<std::string, cocos2d::Sprite *> item(id, s);
	_images.insert(item);
	addChild(s);
}

void MultipleImageSprite::SetCurrentImage(const std::string &id)
{
	if (_images.find(id) == _images.end()) {
		Log::Instance().writeWarning("MultipleImageSprite: Trying to access to unexisted sprite ("+id+")");
		return;
	}

	_curImageId = id;
	for (std::map<std::string, cocos2d::Sprite *>::iterator it = _images.begin(); it != _images.end(); ++it) {
		(*it).second->setVisible((*it).first == id);
	}
}

cocos2d::Sprite* MultipleImageSprite::GetImageByIndex(const std::string &id) const
{
	std::map<std::string, cocos2d::Sprite *>::const_iterator it;
	it = _images.find(id);
	return it != _images.end() ? (*it).second : nullptr;
}

cocos2d::Sprite* MultipleImageSprite::GetCurrentImage(void) const
{
	return GetImageByIndex(_curImageId);
}
