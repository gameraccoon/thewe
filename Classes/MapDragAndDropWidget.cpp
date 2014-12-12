#include "MapDragAndDropWidget.h"

#include "WorldMapLayer.h"
#include "MessageManager.h"
#include "Log.h"

MapDragAndDropWidget::MapDragAndDropWidget(const Settings &settings, WorldMapLayer *worldMapLayer,
										   Cell::WeakPtr cellFrom, const Vector2 &offsetToCenter)
	: _settings(settings)
	, _worldMapLayer(worldMapLayer)
	, _state(State::STAY)
	, _cellFrom(cellFrom)
	, _offsetToCenter(offsetToCenter)
	, _attractionDist(30.0f)
	, _isEnabled(true)
{
	if (_cellFrom.expired()) {
		Log::Instance().writeError("MapDragAndDropWidget: Failed to create with unexisted cell");
	}

	init();
}

bool MapDragAndDropWidget::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(MapDragAndDropWidget::TouchesBegin, this);
	listener->onTouchesMoved = CC_CALLBACK_2(MapDragAndDropWidget::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(MapDragAndDropWidget::TouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(MapDragAndDropWidget::TouchesCancelled, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_texture = new MultipleImageSprite();
	_texture->setPosition(0.0f, 0.0f);
	_texture->autorelease();
	_texture->AddImage("normal", _settings.normalImage);
	_texture->AddImage("pressed", _settings.pressedImage);
	_texture->AddImage("disabled", _settings.disabledImage);
	_texture->SetCurrentImage("normal");

	addChild(_texture);
	scheduleUpdate();

	return true;
}

void MapDragAndDropWidget::update(float dt)
{
}

void MapDragAndDropWidget::setOpacity(GLubyte opacity)
{
	_texture->setOpacity(opacity);
	cocos2d::Node::setOpacity(opacity);
}

void MapDragAndDropWidget::SetEnabled(bool flag)
{
	_isEnabled = flag;

	_texture->SetCurrentImage(_isEnabled ? "normal" : "disabled");
}

void MapDragAndDropWidget::TouchesBegin(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isEnabled) {
		return;
	}

	if (_texture->GetCurrentImage()->getBoundingBox().containsPoint(convertTouchToNodeSpace(touches[0])))
	{
		Message msg = Message("DragOnMapBegan");
		msg.variables.SetBool("SHOW_TOWNS", true);
		MessageManager::Instance().PutMessage(msg);
		_texture->SetCurrentImage("pressed");
		_state = State::DRAG;
		event->stopPropagation();
	}
}

void MapDragAndDropWidget::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isEnabled) {
		return;
	}

	if (_state == State::DRAG)
	{
		Vector2 touch_world = touches[0]->getLocation();
		Vector2 touch_local = convertToNodeSpace(touch_world);

		_texture->setPosition(touch_local);
		_texture->SetCurrentImage("pressed");

		TownMapWidget *town_widget = _worldMapLayer->GetNearestTownWidget(touch_world, _attractionDist);
		if (town_widget) {
			Town::WeakPtr town_ptr = town_widget->GetTown();
			if (!town_ptr.lock()->IsCellPresented()) {
				Vector2 town_pos = town_widget->getPosition();
				_texture->setPosition(convertToNodeSpace(town_pos));
			}
		}
	}
}

void MapDragAndDropWidget::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isEnabled) {
		return;
	}

	if (_state == State::DRAG)
	{
		Vector2 touch_world = touches[0]->getLocation();
		Vector2 touch_local = convertToNodeSpace(touch_world);

		_texture->SetCurrentImage("normal");
		_state = State::DROP;
		_isEnabled = false;
		event->stopPropagation();

		float create_cell = false;

		TownMapWidget *town_widget = _worldMapLayer->GetNearestTownWidget(touch_world, _attractionDist);
		if (town_widget) {
			Town::WeakPtr town_ptr = town_widget->GetTown();
			if (!town_ptr.lock()->IsCellPresented()) {
				create_cell = true;
			}
		}

		if (create_cell)
		{
			Message message("CreateCell");
			message.variables.SetString("TOWN_NAME", town_widget->GetTown()->GetName());
			message.variables.SetInt("PARENT_UID", _cellFrom.lock()->GetUid());
			MessageManager::Instance().PutMessage(message);

			Message msg = Message("DragOnMapEnded");
			msg.variables.SetBool("SHOW_TOWNS", false);
			MessageManager::Instance().PutMessage(msg);
		}
		else
		{
			cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]()
			{
				Message msg = Message("DragOnMapEnded");
				msg.variables.SetBool("SHOW_TOWNS", false);
				MessageManager::Instance().PutMessage(msg);
			});
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.2f, cocos2d::Vec2(0.0f, 0.0f) + _offsetToCenter);
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.5f, 0.5f, 1.0f);
			cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.2f);
			cocos2d::Spawn *stage = cocos2d::Spawn::create(move, scale, fade, nullptr);
			cocos2d::Sequence *seq = cocos2d::Sequence::create(stage, func_end, nullptr);

			_texture->runAction(seq);
		}
	}
}

void MapDragAndDropWidget::TouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	MessageManager::Instance().PutMessage(Message("EnableMapMovements"));
	_texture->SetCurrentImage("normal");
	_state = State::STAY;
}