#ifndef MULTIPLE_IMAGE_SPRITE_H
#define MULTIPLE_IMAGE_SPRITE_H

#include <cocos2d.h>
#include <string>
#include <map>

class MultipleImageSprite : public cocos2d::Node
{
public:
	MultipleImageSprite(void);

	bool init(void) override;
	void setOpacity(GLubyte opacity) override;

	void AddImage(const std::string &id, const std::string &filename);
	void SetCurrentImage(const std::string &id);
	cocos2d::Sprite* GetImageByIndex(const std::string &id) const;
	cocos2d::Sprite* GetCurrentImage(void) const;

private:
	std::map<std::string, cocos2d::Sprite *> _images;
	std::string _curImageId;
};

#endif