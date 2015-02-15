#include "SpinoffDragAndDrop.h"

#include "WorldMapLayer.h"
#include "World.h"
#include "MessageManager.h"
#include "Log.h"

SpinoffDragAndDrop::SpinoffDragAndDrop(WorldMapLayer *worldMapLayer, MapProjector *proj,
									   Cell::WeakPtr cellFrom, const Vector2 &offsetToCenter)
	: _worldMapLayer(worldMapLayer)
	, _projector(proj)
	, _state(State::STAY)
	, _cellFrom(cellFrom)
	, _attractionDist(30.0f)
	, _isEnabled(true)
	, _isAttracting(false)
{
	if (_cellFrom.expired()) {
		WRITE_ERR("MapDragAndDropWidget: Failed to create with unexisted cell");
	}

	init();
}

bool SpinoffDragAndDrop::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(SpinoffDragAndDrop::TouchesBegin, this);
	listener->onTouchesMoved = CC_CALLBACK_2(SpinoffDragAndDrop::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(SpinoffDragAndDrop::TouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(SpinoffDragAndDrop::TouchesCancelled, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_texture = MultipleImageSprite::create();
	_texture->setPosition(0.0f, 0.0f);
	_texture->AddImage("normal", "ui/recrut_normal.png");
	_texture->AddImage("pressed", "ui/recrut_press.png");
	_texture->AddImage("disabled", "ui/recrut_disabled.png");
	_texture->SetCurrentImage("normal");

	_strip = StripEffect::create("effects/spinoff_drag_strip.png");
	_strip->setColor(cocos2d::Color4F(1.0f, 1.0f, 1.0f, 0.4f));
	_strip->setVisible(false);

	addChild(_strip);
	addChild(_texture);
	scheduleUpdate();

	return true;
}

void SpinoffDragAndDrop::update(float dt)
{
	if (_state != State::STAY)
	{
		cocos2d::Vec2 p0 = convertToNodeSpace(getParent()->getPosition());
		cocos2d::Vec2 p1 = _texture->getPosition();

		Utils::Spline<cocos2d::Vec3> spline;
		spline.AddKey(cocos2d::Vec3(p0.x, p0.y, 10.0f));
		spline.AddKey(cocos2d::Vec3(p1.x, p1.y, 70.0f));
		spline.CalculateGradient();

		_strip->setVisible(true);
		_strip->setStripGeometry(spline);
	
		if (!World::Instance().GetTutorialManager().IsTutorialStateAvailable("ReadyToCreateSpinoff"))
		{
			if (!_isAttracting) {
				float shift_dist = 50.0f;
				float shift_speed = 3.0f;
				cocos2d::Vec2 screen = cocos2d::Director::getInstance()->getVisibleSize();
				cocos2d::Vec2 origin = cocos2d::Director::getInstance()->getVisibleOrigin();
				cocos2d::Rect bounds;
				bounds.origin = origin + cocos2d::Vec2(shift_dist, shift_dist);
				bounds.size = screen - cocos2d::Vec2(shift_dist, shift_dist) * 2.0f;
				if (!bounds.containsPoint(_touchWorld)) {
					cocos2d::Vec2 p0 = origin+screen/2.0f;
					cocos2d::Vec2 p1 = _touchWorld;
					_projector->ShiftView((p0-p1).getNormalized() * shift_speed);
					_projector->Update();
					_worldMapLayer->UpdateMapElements();
					_texture->setPosition(convertToNodeSpace(_touchWorld));
				}
			}
		}
	}
	else
	{
		_strip->setVisible(false);
	}
}

void SpinoffDragAndDrop::setOpacity(GLubyte opacity)
{
	_texture->setOpacity(opacity);
	cocos2d::Node::setOpacity(opacity);
}

void SpinoffDragAndDrop::SetEnabled(bool flag)
{
	_isEnabled = flag;

	_texture->SetCurrentImage(_isEnabled ? "normal" : "disabled");
}

void SpinoffDragAndDrop::TouchesBegin(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isEnabled) {
		return;
	}

	_touchWorld = touches[0]->getLocation();
	_touchLocal = convertToNodeSpace(_touchWorld);

	if (_texture->GetCurrentImage()->getBoundingBox().containsPoint(convertTouchToNodeSpace(touches[0])))
	{
		Message msg = Message("SpinoffWithDragBegan");
		msg.variables.SetBool("SHOW_TOWNS", true);
		MessageManager::Instance().PutMessage(msg);
		_texture->SetCurrentImage("pressed");
		_state = State::DRAG;
		event->stopPropagation();
	}
}

void SpinoffDragAndDrop::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	if (!_isEnabled) {
		return;
	}

	if (_state == State::DRAG)
	{
		_touchWorld = touches[0]->getLocation();
		_touchLocal = convertToNodeSpace(_touchWorld);

		bool attach_to_touch = true;

		TownMapWidget *town_widget = _worldMapLayer->GetNearestTownWidget(_touchWorld, _attractionDist);
		if (town_widget)
		{
			Town::WeakPtr town_ptr = town_widget->GetTown();
			if (!town_ptr.lock()->IsCellPresented())
			{
				attach_to_touch = false;
				if (!_isAttracting)
				{
					_isAttracting = true;
					cocos2d::Vec2 pos = convertToNodeSpace(town_widget->getPosition());
					cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.08f, pos);
					cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&](){_isAttracting=false;});
					_texture->runAction(cocos2d::Sequence::create(move, func_end, nullptr));
				}
			}
		}
		
		_texture->SetCurrentImage("pressed");
		if (attach_to_touch) {
			_texture->setPosition(_touchLocal);
		}
	}
}

void SpinoffDragAndDrop::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
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
			Message msg1("CreateCell");
			msg1.variables.SetString("TOWN_NAME", town_widget->GetTown()->GetName());
			msg1.variables.SetInt("PARENT_UID", _cellFrom.lock()->GetUid());
			MessageManager::Instance().PutMessage(msg1);

			Message msg2 = Message("SpinoffWithDragEnded");
			msg2.variables.SetBool("SHOW_TOWNS", false);
			MessageManager::Instance().PutMessage(msg2);
		}
		else
		{
			cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]()
			{
				Message msg = Message("SpinoffWithDragEnded");
				msg.variables.SetBool("SHOW_TOWNS", false);
				MessageManager::Instance().PutMessage(msg);
			});
			
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.2f, convertToNodeSpace(getParent()->getPosition()));
			cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.5f, 0.5f, 1.0f);
			cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.2f);
			cocos2d::Spawn *stage = cocos2d::Spawn::create(move, scale, fade, nullptr);
			cocos2d::Sequence *seq = cocos2d::Sequence::create(stage, func_end, nullptr);

			_texture->stopAllActions();
			_texture->runAction(seq);
			_strip->runAction(fade);
		}
	}
}

void SpinoffDragAndDrop::TouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&]()
	{
		Message msg = Message("SpinoffWithDragEnded");
		msg.variables.SetBool("SHOW_TOWNS", false);
		MessageManager::Instance().PutMessage(msg);
	});

	cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.2f, convertToNodeSpace(getParent()->getPosition()));
	cocos2d::ScaleTo *scale = cocos2d::ScaleTo::create(0.2f, 0.5f, 0.5f, 1.0f);
	cocos2d::FadeOut *fade = cocos2d::FadeOut::create(0.2f);
	cocos2d::Spawn *stage = cocos2d::Spawn::create(move, scale, fade, nullptr);
	cocos2d::Sequence *seq = cocos2d::Sequence::create(stage, func_end, nullptr);
	
	_strip->runAction(fade);
	_texture->stopAllActions();
	_texture->runAction(seq);
	_texture->SetCurrentImage("normal");
	_state = State::STAY;
}
