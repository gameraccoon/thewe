#include "PopUpTextWithIcon.h"

class TextWithIcon : public cocos2d::Node
{
public:
	static TextWithIcon* create(const std::string &icon, const std::string &text,
		const std::string &font, float iconScale, int textSize)
	{
		TextWithIcon *ret = new TextWithIcon();
		if (ret && ret->init(icon, text, font, iconScale, textSize))
		{
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}

	void setOpacity(GLubyte opacity) override
	{
		cocos2d::Node::setOpacity(opacity);
		_text->setOpacity(opacity);
		if (_icon) {
			_icon->setOpacity(opacity);
		}
	}

protected:
	TextWithIcon::TextWithIcon(void)
		: _text(nullptr)
		, _icon(nullptr)
	{}

	bool TextWithIcon::init(const std::string &icon, const std::string &text,
		const std::string &font, float iconScale, int textSize)
	{	
		_text = cocos2d::ui::Text::create(text, font, textSize);
		_text->setTextVerticalAlignment(cocos2d::TextVAlignment::CENTER);
		_text->setTextHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
		_text->setPosition(cocos2d::Vec2(0.0f, 0.0f));
		_text->setScale(1.0f);

		if (!icon.empty())
		{
			_icon = cocos2d::Sprite::create(icon);
			_icon->setPosition(_text->getLeftBoundary(), 0.0f);
			_icon->setScale(iconScale);
			_text->setPositionX(_text->getPositionX() + _icon->getContentSize().width*iconScale/2.0f);
			addChild(_icon);
		}

		addChild(_text);
		return true;
	}

private:
	cocos2d::Sprite *_icon;
	cocos2d::ui::Text *_text;
};

PopUpTextWithIcon::PopUpTextWithIcon(const ConstructionInfo &info, MapProjector *projector, int zOrder)
	: Effect("PopUpTextWithIcon", zOrder, nullptr)
	, _projector(projector)
	, _descriptor(info)
	, _isFinished(false)
{
	init();
}

bool PopUpTextWithIcon::init(void)
{
	Effect::init();

	Vector2 start_p = _projector->ProjectOnScreen(_descriptor.position);
	Vector2 finish_p;

	_textWithIcon = TextWithIcon::create(_descriptor.icon, _descriptor.text,
		_descriptor.font, _descriptor.iconScale, _descriptor.fontSize);
	_textWithIcon->setPosition(0.0f, 0.0f);
	_textWithIcon->setScale(0.1f);

	cocos2d::CallFunc *finalize = cocos2d::CallFunc::create([&](){_isFinished=true;});
	
	cocos2d::MoveBy *move1 = cocos2d::MoveBy::create(0.2f, cocos2d::Vec2(0.0f, 20.0f));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 1.0f, 1.0f, 1.0f);
	cocos2d::MoveBy *move2 = cocos2d::MoveBy::create(0.7f, cocos2d::Vec2(0.0f,  20.0f));
	cocos2d::MoveBy *move3 = cocos2d::MoveBy::create(0.3f, cocos2d::Vec2(0.0f, 30.0f));
	cocos2d::FadeOut *fadeout = cocos2d::FadeOut::create(0.28f);
	cocos2d::Spawn *start = cocos2d::Spawn::create(move1, scale, nullptr);
	cocos2d::Spawn *finish = cocos2d::Spawn::create(move3, fadeout, nullptr);
	cocos2d::Sequence *seq = cocos2d::Sequence::create(start, move2, finish, finalize, nullptr);

	_textWithIcon->runAction(seq);

	setPosition(start_p);
	setScale(_descriptor.overralScale);
	addChild(_textWithIcon);

	return true;
}

void PopUpTextWithIcon::update(float dt)
{
	Effect::update(dt);
	setPosition(_projector->ProjectOnScreen(_descriptor.position));
}

bool PopUpTextWithIcon::IsFinished(void) const
{
	return _isFinished;
}