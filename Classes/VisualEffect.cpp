#include <VisualEffect.h>

namespace VisualEffects
{

	// BASE_EFFECT_COMPOSITION ////////////////////////////////////////////////

	/**
	Initialization of effect composition:
	- initialization all childs
	- begin effect
	- start in-frame updating
	*/
	bool BaseEffectComposition::init(void)
	{
		for (auto it : this->getChildren()) {
			auto ef = dynamic_cast<BaseEffectComposition*>(it);
			if (ef) ef->init();
		}	

		if (_effectProducer) 
			_effectProducer->BeginEffect();			
		
		return Effect::init();
	}

	/**
	Update all components of -> this <- composition
	*/
	void BaseEffectComposition::update(float dt)
	{
		Effect::update(dt);

		if (IsFinished()) return;
		
		for (auto it : this->getChildren())
			it->update(dt);

		if (_effectProducer) _effectProducer->Update(dt);		
	}

	/**
	If all childrens effect compositions are finished
	and -> this <- too - return true
	*/
	bool BaseEffectComposition::IsFinished(void) const
	{
		for (auto it : this->getChildren())
		{
			auto effect = dynamic_cast<BaseEffectComposition*>(it);
			if (effect && !effect->IsFinished())
				return false;
		}
		if (_effectProducer && _effectProducer->IsFinished())
			return true;
		else
			return false;
	}

	// BASE_EFFECT_PRODUCER ///////////////////////////////////////////////////

	void BaseEffectProducer::checkFinish(void)
	{
		_curState = (_curTimer >= _continuance ? finished : _curState);
	}

	// BASE EFFECT CONDUCTOR //////////////////////////////////////////////////

	bool BaseEffectConductor::PushQueue(BaseEffectProducer * effectPerformer,
		float continuance)
	{
		if (!effectPerformer) return false;
		effectPerformer->SetContinuance(continuance);
		_effectsQueue.push_back(effectPerformer);
		return true;
	}
	bool BaseEffectConductor::PushBckgrnd(BaseEffectProducer * effectPerformer,
		float continuance)
	{
		if (!effectPerformer) return false;
		effectPerformer->SetContinuance(continuance);
		_bgEffects.push_back(effectPerformer);
		return true;
	}

	void BaseEffectConductor::Update(float dt)
	{
		if (_curState == working)
		{
			for (auto ef : _bgEffects)
			if (ef->IsFinished()) ef->BeginEffect();
			else ef->Update(dt);

			if (!_curEffect) {

				if (_workingQueue.size()) {
					_curEffect = _workingQueue.front();
					_curEffect->BeginEffect();
				}
				else _curTimer += dt;
			}
			else
			{
				if (!_curEffect->IsFinished())
					_curEffect->Update(dt);
				else {
					_workingQueue.pop_front();
					_curEffect = nullptr;
				}
			}
			checkFinish();
		}

		if (IsFinished()) {
			if (_infiniteRepeat){
				StopEffect();
				BeginEffect();
			}
		}
	}

	bool BaseEffectConductor::IsFreezed(void) const
	{
		if (!_curEffect ||
			(_curEffect && _curEffect->IsFreezed()))
		{
			for (auto ef : _bgEffects)
			if (!ef->IsFreezed()) return false;
		}

		return true;
	}

	void BaseEffectConductor::BeginEffect(void)
	{
		auto begin = _effectsQueue.begin();
		auto end = _effectsQueue.end();

		while (begin != end) {
			_workingQueue.push_back(*begin);
			begin++;
		}

		BaseEffectProducer::BeginEffect();
		for (auto ef : _bgEffects)
			ef->BeginEffect();
	}

	void BaseEffectConductor::StopEffect(void)
	{
		BaseEffectProducer::StopEffect();
		for (auto _ef : _bgEffects) _ef->StopEffect();
		if (_curEffect)
			_curEffect->StopEffect();
	}

	void BaseEffectConductor::PauseEffect(void)
	{
		BaseEffectProducer::PauseEffect();
		for (auto ef : _bgEffects) ef->PauseEffect();
		if (_curEffect)
			_curEffect->PauseEffect();
	}

	// BASE EFFECT PERFORMER //////////////////////////////////////////////////

	void BaseEffectPerformer::Update(float dt)
	{
		if (_curState == working) {

			DoEffect(dt);
			_curTimer += dt;

			switch (_curFinishType) {
			case overtime:
				if (_curTimer >= _continuance)_curState = finished;
				break;
			case endChanges:
				if (IsFreezed()) _curState = finished;
				break;
			}
		}
	}

	void BaseEffectPerformer::DoEffect(float dt, cocos2d::Node * node)
	{
		if (!node) node = _effectComposition;

		auto children = node->getChildren();
		for (cocos2d::Node* child_node : children) DoEffect(dt, child_node);

		ChangeValue(dt, node);
	}


	bool BaseEffectPerformer::IsFreezed(void) const
	{
		return _curValue == _preValue;
	}

	void EffectScaler::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaVaue = dt * _targetValue / _continuance;
		_curValue = node->getScale() + deltaVaue;
		node->setScale(_curValue);
	}

	void EffectRounder::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValue = dt * _targetValue / _continuance;
		_curValue = node->getRotation() + deltaValue;
		node->setRotation(_curValue);
	}

	void EffectFader::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValue = dt * _targetValue / _continuance;
		_curValue = node->getOpacity() + deltaValue;
		if (_curValue < 0) _curValue = 0;
		if (_curValue > 255) _curValue = 255;
		node->setOpacity((unsigned char)_curValue);
	}

	void EffectMover::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValueX = dt * _targetValue / _continuance;
		float deltaValueY = dt * _targetValue1 / _continuance;
		_curValue = node->getPosition().x + deltaValueX;
		_curValue1 = node->getPosition().y + deltaValueY;
		node->setPosition(_curValue, _curValue1);
	}


}