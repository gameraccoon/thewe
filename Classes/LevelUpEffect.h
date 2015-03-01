#ifndef LEVEL_UP_EFFECT_H
#define LEVEL_UP_EFFECT_H

#include <ui/CocosGUI.h>
#include <cocos-ext.h>
#include <MapProjector.h>

#include <VisualEffect.h>

namespace VisualEffects
{
	class LevelUpWidget : public cocos2d::Node
	{

	public:
		static LevelUpWidget* create(const std::string &sun, 
			const std::string &rays, const std::string &text, 
			const std::string &font, int textSize)
		{
			LevelUpWidget *ret = new LevelUpWidget();

			if (ret != nullptr && ret->init(sun, rays, text, font, textSize)) {

				ret->autorelease();
				return ret;
			}
			delete ret;
			return nullptr;
		}
	protected:
		LevelUpWidget(void)
			: _sun(nullptr)
			, _rays(nullptr)
			, _textLevelUp(nullptr)
		{}

		bool init(const std::string &sun, const std::string &rays,
			const std::string &text, const std::string &font, int textSize)
		{
			_sun = cocos2d::Sprite::create(sun);			
			_rays = cocos2d::Sprite::create(rays);
			

			_textLevelUp = cocos2d::ui::Text::create(
				text, font, textSize);
			_textLevelUp->setTextVerticalAlignment(
				cocos2d::TextVAlignment::CENTER);
			_textLevelUp->setTextHorizontalAlignment(
				cocos2d::TextHAlignment::CENTER);
			_textLevelUp->setPosition(cocos2d::Vec2(0.0f, 0.0f));
			setScale(1.0f);

			addChild(_rays, 1);
			addChild(_sun, 0);
			addChild(_textLevelUp, 2);


			return true;
		}

	private:
		friend class DefaultLevelUpEffect;

	private:
		cocos2d::Sprite* _sun;
		cocos2d::Sprite* _rays;
		cocos2d::ui::Text* _textLevelUp;
	};

	class DefaultLevelUpEffect : public BaseEffectComposition
	{
	public:
		struct LUpEffectInfo
		{
			std::string sun;
			std::string rays;
			std::string text;
			std::string font;
			Vector2 position;
			int textSize;
			
		};

	public:
		DefaultLevelUpEffect(
			const LUpEffectInfo &info,
			MapProjector *projector,
			const std::string &name,
			int zOrder,
			cocos2d::Node *relation = nullptr)
			: BaseEffectComposition(name, zOrder, relation)
			, _descriptor(info)
			, _projector(projector)
		{
			init();
		}

		bool init(void)
		{
			LevelUpWidget* _levelUpWidget = LevelUpWidget::create
				(_descriptor.sun, _descriptor.rays, _descriptor.text
				, _descriptor.font, _descriptor.textSize);
			_levelUpWidget->setPosition(0.0f, 0.0f);
			_levelUpWidget->setScale(1.0f);

			_levelUpWidget->_sun->setScale(0.0f);
			_levelUpWidget->_rays->setScale(0.0f);
			_levelUpWidget->_textLevelUp->setOpacity(0);

			_effectProducer = new BaseEffectConductor(_levelUpWidget);			

			// prologue
			auto es = new EffectScaler(_levelUpWidget->_sun, 0.5f);
			auto es1 = new EffectScaler(_levelUpWidget->_rays, 0.6f);
			auto er = new EffectRounder(_levelUpWidget->_rays, 90);

			// epilogue
			auto ec = new BaseEffectConductor(_levelUpWidget->_textLevelUp, 3);
			auto ef = new EffectFader(_levelUpWidget->_textLevelUp, 256);
			auto em = new EffectMover(_levelUpWidget->_textLevelUp, 0, 50);			
			ec->PushBckgrnd(ef, 2);
			ec->PushBckgrnd(em, 2);		

			auto en = new EffectNothing(_levelUpWidget);
			auto ef1 = new EffectFader(_levelUpWidget, -256);

			_effectProducer->PushQueue(es, 2);
			_effectProducer->PushQueue(es1, 2);
			_effectProducer->PushBckgrnd(er, 2);
			_effectProducer->PushQueue(ec, 2);
			_effectProducer->PushQueue(en, 2);
			_effectProducer->PushQueue(ef1, 2);

			addChild(_levelUpWidget);
			BaseEffectComposition::init();
			return true;
		}

		void update(float dt)
		{
			BaseEffectComposition::update(dt);

			setPosition(_projector->ProjectOnScreen(_descriptor.position));
		}

	private:
		LevelUpWidget *_levelUpWidget;
		LUpEffectInfo _descriptor;
		MapProjector *_projector;




	};


}

#endif // LEVEL_UP_EFFECT_H