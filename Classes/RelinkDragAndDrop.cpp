#include "RelinkDragAndDrop.h"

#include "WorldMapLayer.h"

const float RelinkDragAndDrop::DRAG_WAIT_DELAY = 0.25f;
const float RelinkDragAndDrop::DRAG_WAIT_DISTANCE = 50.0f;
const float RelinkDragAndDrop::ATTRACTION_DISTANCE = 30.0f;
const Vector2 RelinkDragAndDrop::MARK_POSITION = Vector2(0.0f, 155.0f);

RelinkDragAndDrop::RelinkDragAndDrop(CellMapWidget *widget, WorldMapLayer *worldMapLayer, MapProjector *projector, Cell::WeakPtr cell)
	: Effect("RelinkDragAndDrop", 0, widget)
	, _worldMapLayer(worldMapLayer)
	, _projector(projector)
	, _cell(cell)
	, _state(State::IDLE)
	, _timeLocal(0.0f)
	, _markAngle(0.0f)
	, _isAttracting(false)
	, _isRelinkFinished(false)
{
	init();
}

bool RelinkDragAndDrop::init(void)
{
	cocos2d::EventListenerTouchAllAtOnce *listener = cocos2d::EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(RelinkDragAndDrop::TouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(RelinkDragAndDrop::TouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(RelinkDragAndDrop::TouchesEnded, this);
	listener->onTouchesCancelled = CC_CALLBACK_2(RelinkDragAndDrop::TouchesCancelled, this);
	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	_sprite = new MultipleImageSprite();
	_sprite->autorelease();
	_sprite->setPosition(0.0f, 0.0f);
	_sprite->setScale(1.0f);
	_sprite->AddImage("Drag", "relink-drag.png");
	_sprite->SetCurrentImage("Drag");
	_sprite->setOpacity(0);

	_strip = StripEffect::create("spinoff_drag_strip.png");
	_strip->setPosition(0.0f, 0.0f);

	_mark = cocos2d::Sprite::create("relink-mark.png");
	_mark->setPosition(MARK_POSITION);
	_mark->setScale(1.0f);
	_sprite->addChild(_mark);
	
	addChild(_strip, 0);
	addChild(_sprite, 1);
	scheduleUpdate();

	return true;
}

void RelinkDragAndDrop::update(float dt)
{
	Effect::update(dt);

	if (_state == State::WAIT) {
		_timeLocal += dt;
		if ((_touchFirst - _touchWorld).Size() >= DRAG_WAIT_DISTANCE) {
			_timeLocal = 0.0f;
			_state = State::IDLE;
		}
		if (_timeLocal >= DRAG_WAIT_DELAY) {
			_state = State::DRAG;
			_timeLocal = 0.0f;
			_sprite->setPosition(0.0f, 0.0f);
			_sprite->runAction(cocos2d::FadeIn::create(0.3f));
 
			cocos2d::MoveTo *mark_move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(6.0f, 3.0f));
			cocos2d::ScaleTo *mark_scale = cocos2d::ScaleTo::create(0.5f, 0.8f, 0.8f, 0.8f);
			_mark->runAction(cocos2d::Spawn::create(mark_move, mark_scale, nullptr));

			Message message("RelinkWithDragBegan");
			message.variables.SetBool("SHOW_TOWNS", false);
			MessageManager::Instance().PutMessage(message);
		}
	}

	if (_sprite->getPosition().getLength() > 1.0f) {
		Utils::Spline<cocos2d::Vec3> spline;
		spline.AddKey(cocos2d::Vec3(0.0f, 0.0f, 20.0f));
		spline.AddKey(cocos2d::Vec3(_sprite->getPosition().x, _sprite->getPosition().y, 130.0f));
		spline.CalculateGradient();
		_strip->setVisible(true);
		_strip->setStripGeometry(spline);
	} else {
		_strip->setVisible(false);
	}

	if (_state == State::DRAG && !_isAttracting) {
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
				_sprite->setPosition(convertToNodeSpace(_touchWorld));
			}
		}


	_markAngle += 180.0f * dt;
	_mark->setRotation3D(cocos2d::Vec3(0.0f, _markAngle, 0.0f));
}

bool RelinkDragAndDrop::IsFinished(void) const
{
	return _isRelinkFinished;
}

void RelinkDragAndDrop::TouchesBegan(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	_touchFirst = touches[0]->getLocation();
	_touchWorld = touches[0]->getLocation();
	_touchLocal = convertToNodeSpace(_touchWorld);

	if (_sprite->GetCurrentImage()->getBoundingBox().containsPoint(_touchLocal))
	{
		if (_state == State::IDLE) {
			_state = State::WAIT;
		}

		if (_state == State::DRAG)
		{
			_state = State::DRAG;
			event->stopPropagation();
		}
	}
}

void RelinkDragAndDrop::TouchesMoved(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	_touchWorld = touches[0]->getLocation();
	_touchLocal = convertToNodeSpace(_touchWorld);

	if (_state == State::DRAG)
	{
		bool attach_to_cell = false;

		CellMapWidget *cell_widget = _worldMapLayer->GetNearestCellWidget(_touchWorld, ATTRACTION_DISTANCE);
		if (cell_widget) {
			Cell::WeakPtr cell_ptr = cell_widget->GetCell();
			if (cell_ptr.lock()->IsState(Cell::State::READY)) {
				attach_to_cell = true;
				if (!_isAttracting) {
					_isAttracting = true;
					cocos2d::Vec2 pos = convertToNodeSpace(cell_widget->getPosition());
					cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.08f, pos);
					cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&](){_isAttracting=false;});
					_sprite->runAction(cocos2d::Sequence::create(move, func_end, nullptr));
				}
			}
		}

		if (!attach_to_cell) {
			_sprite->setPosition(_touchLocal);
		}
	}
}

void RelinkDragAndDrop::TouchesEnded(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	_touchWorld = touches[0]->getLocation();
	_touchLocal = convertToNodeSpace(_touchWorld);

	if (_state == State::DRAG)
	{
		bool relink = false;

		Cell::WeakPtr cell_pointer;
		CellMapWidget *cell_widget = _worldMapLayer->GetNearestCellWidget(_touchWorld, ATTRACTION_DISTANCE);
		if (cell_widget) {
			cell_pointer = cell_widget->GetCell();
			if (cell_pointer.lock()->IsState(Cell::State::READY)) {
				relink = true;
			}
		}

		if (relink)
		{
			Message message1("RelinkCell");
			message1.variables.SetInt("RELINK_CELL_UID", _cell.lock()->GetUid());
			message1.variables.SetInt("PARENT_CELL_UID", cell_pointer.lock()->GetUid());
			MessageManager::Instance().PutMessage(message1);

			Message message2("RelinkWithDragEnded");
			message2.variables.SetBool("SHOW_TOWNS", false);
			MessageManager::Instance().PutMessage(message2);

			_isRelinkFinished = true;
		}
		else
		{
			Message message("RelinkWithDragEnded");
			message.variables.SetBool("SHOW_TOWNS", false);
			MessageManager::Instance().PutMessage(message);

			cocos2d::CallFunc *func_end = cocos2d::CallFunc::create([&](){
				_sprite->runAction(cocos2d::FadeOut::create(0.2f));
			});
			cocos2d::MoveTo *move = cocos2d::MoveTo::create(0.5f, cocos2d::Vec2(0.0f, 0.0f));
			cocos2d::EaseCubicActionOut *move_ease = cocos2d::EaseCubicActionOut::create(move);
			_sprite->runAction(cocos2d::Sequence::create(move_ease, func_end, nullptr));

			cocos2d::MoveTo *mark_move = cocos2d::MoveTo::create(0.5f, MARK_POSITION);
			cocos2d::ScaleTo *mark_scale = cocos2d::ScaleTo::create(0.5f, 1.0f, 1.0f, 1.0f);
			_mark->runAction(cocos2d::Spawn::create(mark_move, mark_scale, nullptr));
		}
	}

	if (_sprite->GetCurrentImage()->getBoundingBox().containsPoint(_touchLocal)) {
		event->stopPropagation();
	}

	_state = State::IDLE;
}

void RelinkDragAndDrop::TouchesCancelled(const std::vector<cocos2d::Touch *> &touches, cocos2d::Event *event)
{
	_state = State::IDLE;
}