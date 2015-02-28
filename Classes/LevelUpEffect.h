#ifndef LEVEL_UP_EFFECT_H
#define LEVEL_UP_EFFECT_H

#include <ui/CocosGUI.h>
#include <cocos-ext.h>

#include <VisualEffect.h>

namespace VisualEffects
{
	class DefaultLevelUpEffect : public BaseEffectComposition
	{
	public:
		DefaultLevelUpEffect(
			const std::string &name,
			int zOrder,
			cocos2d::Node *relation = nullptr)
			: BaseEffectComposition(name, zOrder, relation)
			, _sun(nullptr)
			, _rays(nullptr)
			, _textLevelUp(nullptr)

		{}

		static DefaultLevelUpEffect* create(
			const std::string &sun,
			const std::string &rays,
			const std::string &textLevelUp,
			const std::string &font,
			int textSize,
			int zOrder,
			cocos2d::Node *relation = nullptr)
		{
			DefaultLevelUpEffect *ret = new DefaultLevelUpEffect
				("DefaultLevelUpEffect", zOrder, relation);

			if (ret != nullptr) {

				ret->_sun = cocos2d::Sprite::create(sun);
				ret->_rays = cocos2d::Sprite::create(rays);

				ret->_textLevelUp = cocos2d::ui::Text::create(
					textLevelUp, font, textSize);
				ret->_textLevelUp->setTextVerticalAlignment(
					cocos2d::TextVAlignment::CENTER);
				ret->_textLevelUp->setTextHorizontalAlignment(
					cocos2d::TextHAlignment::CENTER);
				ret->_textLevelUp->setPosition(cocos2d::Vec2(0.0f, 0.0f));
				ret->setScale(1.0f);
				ret->_textLevelUp->setOpacity(0);

				ret->addChild(ret->_rays);
				ret->addChild(ret->_sun);
				ret->addChild(ret->_textLevelUp);
				ret->autorelease();

				auto ep = new BaseEffectConductor(ret);
				auto es = new EffectScaler(ret->_sun, 5, 3);
				auto es1 = new EffectScaler(ret->_rays, 5, 3);
				auto er = new EffectRounder(ret->_rays, 5, 3);
				auto ef = new EffectFader(ret->_textLevelUp, 5, 5);

				ep->pushQueue(es);
				ep->pushQueue(es1);
				ep->pushQueue(er);
				ep->pushQueue(ef);

				return ret;
			}

			delete ret;
			return nullptr;


		}



	private:
		cocos2d::Sprite* _sun;
		cocos2d::Sprite* _rays;
		cocos2d::ui::Text* _textLevelUp;
	};


}

#endif // LEVEL_UP_EFFECT_H