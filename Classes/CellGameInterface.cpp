#include "CellGameInterface.h"

#include "WorldMapLayer.h"

class CellTasksScreen : public cocos2d::Layer
{
public:
	CellTasksScreen(Cell::WeakPtr cell, CellMenuSelector *cellMenu)
		: _cell(cell)
		, _cellMenu(cellMenu)
	{
		init();
	}

	virtual bool init(void) override
	{
		if (!cocos2d::Layer::init())
		{
			return false;
		}
		
		/*
		cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		cocos2d::Point center = origin + screen / 2.0f;
		
		cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
		cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);

		cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

		_bkgDraw = cocos2d::Sprite::create("cell-tasks-menu.png");
		_bkgDraw->setPosition(center);

		cocos2d::Rect tex_rect = _bkgDraw->getTextureRect();
		float actual_w = tex_rect.size.width * _bkgDraw->getScaleX();
		float actual_h = tex_rect.size.height * _bkgDraw->getScaleY();

		float offset_x = 35.0f;
		float offset_y = 35.0f;
		
		_scrollView = cocos2d::extension::ScrollView::create(cocos2d::Size(actual_w - offset_x, actual_h - offset_y));
		//_scrollView->setContentSize(cocos2d::Size(actual_w - offset_x, 5999));
		_scrollView->setPosition(center.x - actual_w / 2.0f + offset_x/2, center.y - actual_h / 2.0f + offset_y/2);
		_scrollView->setDirection(cocos2d::extension::ScrollView::Direction::VERTICAL);
		//_scrollView->setContentOffset(cocos2d::Vec2(0.0f, -500.0f), true);
		//_scrollView->setContentOffsetInDuration(cocos2d::Vec2(0.0f, -500.0f), 0.8f);
			
		cocos2d::Sprite *s = cocos2d::Sprite::create("cell-tasks-menu-close-normal.png");

		cocos2d::Vec2 item_pos;
		item_pos.x = s->getContentSize().width / 2.0;
		item_pos.y = 0.0f;//_scrollView->getPosition().y + _scrollView->getViewSize().height - s->getContentSize().height * 2.0f;

		cocos2d::Vec2 item_offset = cocos2d::Vec2(0.0f, s->getContentSize().height * 1.1f);

		s->setPosition(item_pos);
		_scrollView->addChild(s);

		float total_content_size = 0.0f;
	
		for (int index = 1; index < 20; index++)
		{
			cocos2d::Sprite *item = cocos2d::Sprite::create("cell-tasks-menu-close-normal.png");			
			
			item->setPosition(item_pos + item_offset * index);
			_scrollView->addChild(item);

			total_content_size += item_offset.y;
		}

		_scrollView->setContentSize(cocos2d::Size(actual_w - offset_x, total_content_size));
		_scrollView->setContentOffset(_scrollView->minContainerOffset());*/
		
		//addChild(_bkgDraw, 0);
		//addChild(_scrollView, 1.0f);

		cocos2d::Point screen = cocos2d::Director::getInstance()->getVisibleSize();
		cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();
		cocos2d::Point center = origin + screen / 2.0f;

		cocos2d::MenuItemImage *closeButton;
		{
			using namespace cocos2d;
			closeButton = MenuItemImage::create("cell-tasks-menu-close-normal.png",
				"cell-tasks-menu-close-pressed.png", CC_CALLBACK_1(CellTasksScreen::_OnCloseCallback, this));
		}

		cocos2d::Menu *menu = cocos2d::Menu::create(closeButton, nullptr);
		menu->setPosition(center);
		cocos2d::Sprite *menuBackground = cocos2d::Sprite::create("cell-tasks-menu.png");
		menuBackground->setPosition(center);

		float close_x = closeButton->getContentSize().width / 2.0f;
		float close_y = menuBackground->getContentSize().height / 2.0f - closeButton->getContentSize().height / 2.0f - 17;
		closeButton->setPosition(0.0f, close_y);

		cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.8f, 1.0f, 1.0f);
		cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.5f);
		cocos2d::EaseElasticOut *elastic_scale = cocos2d::EaseElasticOut::create(scale, 5.0f);

		setScale(0.01f);
		setOpacity(0);
		runAction(elastic_scale);
		runAction(fade);
		addChild(menuBackground);
		addChild(menu);
		
		return true;
	}
	
private:
	void _OnCloseCallback(cocos2d::Ref *sender)
	{
		cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.2f, 0.01f);
		cocos2d::EaseElasticIn *elastic_scale = cocos2d::EaseElasticIn::create(scale, 5.0f);
		cocos2d::CallFunc *func = cocos2d::CallFunc::create(CC_CALLBACK_0(CellMenuSelector::OnCellMenuClosed, _cellMenu));

		runAction(cocos2d::Sequence::create(elastic_scale, func, nullptr));
	}

	void _InitScrollMenu(const cocos2d::Vec2 &menuAnchor, const cocos2d::Size &visibleSize)
	{
	}

	void _AddTaskMenuItem(const std::string text, float morale, float difficult)
	{
	}

	Cell::WeakPtr _cell;
	CellMenuSelector *_cellMenu;
	cocos2d::extension::ScrollView *_scrollTasksView;
};

CellMenuSelector::CellMenuSelector(MapProjector *proj, WorldMapLayer *map)
	: _projector(proj)
	, _worldMapLayer(map)
	, _isDisappearing(false)
{
	init();
}

bool CellMenuSelector::init()
{
	if (!cocos2d::Layer::init())
	{
		return false;
	}

	{
		using namespace cocos2d;

		_button.resize(CELL_NUM_TAGS);
		_button[CELL_OPEN_TASKS] = MenuItemImage::create("1_norm.png", "1_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_INFO] = MenuItemImage::create("2_norm.png", "2_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));
		_button[CELL_OPEN_SPINOFF] = MenuItemImage::create("3_norm.png", "3_press.png", CC_CALLBACK_1(CellMenuSelector::_MenuInputListener, this));

		_button[CELL_OPEN_TASKS]->setTag(CELL_OPEN_TASKS);
		_button[CELL_OPEN_INFO]->setTag(CELL_OPEN_INFO);
		_button[CELL_OPEN_SPINOFF]->setTag(CELL_OPEN_SPINOFF);
	}
	
	_menu = cocos2d::Menu::create(_button[CELL_OPEN_TASKS],
		_button[CELL_OPEN_INFO], _button[CELL_OPEN_SPINOFF], nullptr);
	_menu->setPosition(_position);

	addChild(_menu, 0);
	scheduleUpdate();
	setVisible(false);

	return true;
}

void CellMenuSelector::update(float dt)
{
	if (_IsAnimationFinished() && _isDisappearing)
	{
		setVisible(false);
		_isDisappearing = false;
	}

	if (isVisible())
	{
		Vector2 initialPos = _cell.lock()->GetInfo().location;
		Vector2 updatedPos = _projector->ProjectOnScreen(initialPos);

		_menu->setPosition(updatedPos);
	}
}

bool CellMenuSelector::IsCursorOnMenu(const Vector2 &cursorPos) const
{
	for (ButtonsList::const_iterator it = _button.begin(); it != _button.end(); ++it)
	{
		if ((*it)->getBoundingBox().containsPoint(cursorPos))
		{
			return true;
		}
	}
	
	return false;
}
	
void CellMenuSelector::AppearImmediately(Cell::WeakPtr cell, const Vector2 &position)
{
	_cell = cell;
	_position = position;

	setVisible(true);
}

void CellMenuSelector::AppearWithAnimation(Cell::WeakPtr cell, const Vector2 &position)
{
	if (!_IsAnimationFinished())
	{
		return;
	}

	_cell = cell;
	_position = position;

	_menu->setPosition(_position);

	cocos2d::Vec2 dir(0.0f, 1.0f);
	const float dist = 45.0f;
	const float angle = 2.0f * 3.14159265f / (float)CELL_NUM_TAGS;

	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		cocos2d::MenuItemImage *item = (*it);

		// поворачиваем все векторы, кроме первого
		if (it != _button.begin())
		{
			dir = dir.rotateByAngle(cocos2d::Vec2::ZERO, angle);
			dir.normalize();
		}

		item->stopAllActions();
		_PrepearButtonToAppear(item, dir * dist);
	}

	setVisible(true);
}

void CellMenuSelector::DisappearImmedaitely(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		(*it)->stopAllActions();
	}

	setVisible(false);
}

void CellMenuSelector::DisappearWithAnimation(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		cocos2d::MenuItemImage *item = (*it);

		item->stopAllActions();
		_PrepearButtonToDisappear(item);
	}

	_isDisappearing = true;
}

void CellMenuSelector::OnCellMenuClosed(void)
{
	cocos2d::Node *menu = getChildByName("CellMenu");

	if (menu)
	{
		removeChild(menu, true);
		DisappearWithAnimation();

		_worldMapLayer->SetGuiEnabled(true);
		_worldMapLayer->SetMapInputEnabled(true);
	}
}

void CellMenuSelector::_PrepearButtonToAppear(cocos2d::MenuItemImage *item, Vector2 pos)
{
	if (!item)
	{
		assert(false);
		return;
	}

	item->setPosition(cocos2d::Vec2::ZERO);
	item->setScale(0.4f);
	item->setOpacity(0);

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(1.0f, pos);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(1.0f, 0.85f, 0.85f);
	cocos2d::FadeIn *fade = cocos2d::FadeIn::create(0.25f);

	cocos2d::EaseElasticOut *ease_move = cocos2d::EaseElasticOut::create(move, 0.35f);
	cocos2d::EaseElasticOut *ease_scale = cocos2d::EaseElasticOut::create(scale, 1.0f);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

void CellMenuSelector::_PrepearButtonToDisappear(cocos2d::MenuItemImage *item)
{
	if (!item)
	{
		assert(false);
		return;
	}
	
	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.35f, cocos2d::Vec2::ZERO);
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.35f, 0.4f, 0.4f);
	cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.25f);

	cocos2d::EaseBackInOut *ease_move = cocos2d::EaseBackInOut::create(move);
	cocos2d::EaseBackInOut *ease_scale = cocos2d::EaseBackInOut::create(scale);

	item->runAction(ease_move);
	item->runAction(ease_scale);
	item->runAction(fade);
}

void CellMenuSelector::_MenuInputListener(cocos2d::Ref *sender)
{
	if (getChildByName("CellMenu"))
	{
		return;
	}

	cocos2d::MenuItemImage *item = dynamic_cast<cocos2d::MenuItemImage *>(sender);
	cocos2d::Layer *menu;

	CELL_MENU_TAGS tag = (CELL_MENU_TAGS)item->getTag();

	switch (tag)
	{
	case CELL_OPEN_TASKS:
		menu = new CellTasksScreen(_cell, this);
		menu->autorelease();
		menu->setName("CellMenu");
		addChild(menu);

		_worldMapLayer->SetGuiEnabled(false);
		_worldMapLayer->SetMapInputEnabled(false);

		break;
	case CELL_OPEN_INFO:
		break;
		case CELL_OPEN_SPINOFF:
		break;
	default: break;
	};
}

bool CellMenuSelector::_IsAnimationFinished(void)
{
	for (ButtonsList::iterator it = _button.begin(); it != _button.end(); ++it)
	{
		if ((*it)->getNumberOfRunningActions() > 0)
		{
			return false;
		}
	}

	return true;
}