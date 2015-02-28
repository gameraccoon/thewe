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
		for (auto it : this->getChildren())
		if (dynamic_cast<BaseEffectComposition*>(it))
			dynamic_cast<BaseEffectComposition*>(it)->init();

		_EffectProducer->beginEffect();
		this->scheduleUpdate();
		return Effect::init();
	}

	/**
	Update all components of -> this <- composition
	*/
	void BaseEffectComposition::update(float dt)
	{
		if (IsFinished()) return;

		Effect::update(dt);
		for (auto it : this->getChildren())
			it->update(dt);

		_EffectProducer->update(dt);
	}

	/**
	If all chuldrens effect compositions are finished
	and -> this <- too - return true
	*/
	bool BaseEffectComposition::IsFinished(void) const
	{
		for (auto it : this->getChildren())
		{
			auto _effect = dynamic_cast<BaseEffectComposition*>(it);
			if (_effect && !_effect->IsFinished())
				return false;
		}
		if (_EffectProducer->isFinished())
			return true;
		else 
			return false;
	}


	// BASE_EFFECT_PRODUCER ///////////////////////////////////////////////////

	void BaseEffectProducer::checkFinish(void)
	{
		_CurrentState
			= (_CurrentTimer >= _Continuance ? finished : _CurrentState);		
	}

	// BASE EFFECT CONDUCTOR //////////////////////////////////////////////////

	void BaseEffectConductor::update(float dt)
	{
		if (_CurrentState == working) 
		{
			for (auto _ef : _BckgrndEffects) 
				_ef->update(dt);

			if (!_currentEffect) 
			{
				if (_tempQueue.size()) {
					_currentEffect = _tempQueue.front();
					_currentEffect->beginEffect();
				}
				else _CurrentTimer += dt;
			}
			else 
			{
				if (!_currentEffect->isFinished()) 
					_currentEffect->update(dt);
				else {
					_tempQueue.pop_front();
					_currentEffect = nullptr;
				}
			}			
			checkFinish();
		}

		if (isFinished())
		{
			_CurrentTimer = 0.0F;
			stopEffect();
			if (_infiniteRepeat) beginEffect();
		}
	}

	bool BaseEffectConductor::isFreezed(void) const
	{
		if (!_currentEffect ||
			(_currentEffect && _currentEffect->isFreezed()))
		{
			for (auto _ef : _BckgrndEffects)
			if (!_ef->isFreezed()) return false;
		}

		return true;
	}

	void BaseEffectConductor::beginEffect(void)
	{
		EffectsQueue::iterator begin = _EffectsQueue.begin();
		EffectsQueue::iterator end = _EffectsQueue.end();
		
		while (begin != end) {
			_tempQueue.push_back(*begin);
			begin++;
		}

		BaseEffectProducer::beginEffect();
		for (auto _ef : _BckgrndEffects) 
			_ef->beginEffect();
	}

	void BaseEffectConductor::stopEffect(void)
	{
		BaseEffectProducer::stopEffect();
		for (auto _ef : _BckgrndEffects) _ef->stopEffect();
		if (_currentEffect) 
			_currentEffect->stopEffect();
	}

	void BaseEffectConductor::pauseEffect(void)
	{
		BaseEffectProducer::pauseEffect();
		for (auto _ef : _BckgrndEffects) _ef->pauseEffect();
			_currentEffect->pauseEffect();
	}

	bool BaseEffectConductor::pushQueue(BaseEffectPerformer * _effectPerformer, float continuance)
	{
		if (!_effectPerformer) return false;
		_effectPerformer->setContinuance(continuance);
		_EffectsQueue.push_back(_effectPerformer);
		return true;
	}
	bool BaseEffectConductor::pushBckgrnd(BaseEffectPerformer * _effectPerformer)
	{
		if (!_effectPerformer) return false;
		_BckgrndEffects.push_back(_effectPerformer);
		return true;
	}

	// end ////////////////////////////////////////////////////////////////////

	// BASE EFFECT PERFORMER  - begin /////////////////////////////////////////

	void BaseEffectPerformer::update(float dt)
	{
		if (_CurrentState == working)
		{
			doEffect(dt);
			_CurrentTimer += dt;

			switch (_CurrentFinishType)
			{
			case overtime:
				if (_CurrentTimer >= _Continuance)
					_CurrentState = finished;
				break;
			case endChanges:
				if (isFreezed())
					_CurrentState = finished;
				break;
			}
		}
	}

	void BaseEffectPerformer::doEffect(float dt, cocos2d::Node * node)
	{
		if (!node) node = _EffectComposition;

		auto children = node->getChildren();
		for (cocos2d::Node* child_node : children) doEffect(dt, child_node);

		ChangeValue(dt, node);
	}


	bool BaseEffectPerformer::isFreezed(void) const
	{
		return _curValue == _preValue;
	}

	// end ////////////////////////////////////////////////////////////////////

	// EFFECT SCALER  - begin /////////////////////////////////////////////////

	void EffectScaler::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaVaue = dt * _targetValue / _Continuance;
		_curValue = node->getScale() + deltaVaue;
		node->setScale(_curValue);		
	}

	// end ////////////////////////////////////////////////////////////////////

	// EFFECT ROUNDER  - begin ////////////////////////////////////////////////

	void EffectRounder::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValue = dt * _targetValue / _Continuance;
		_curValue = node->getRotation() + deltaValue;
		node->setRotation(_curValue);
	}

	// end ////////////////////////////////////////////////////////////////////

	// EFFECT FADER  - begin ////////////////////////////////////////////////

	void EffectFader::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValue = dt * _targetValue / _Continuance;
		_curValue = node->getOpacity() + deltaValue;
		node->setOpacity((unsigned char)_curValue);
	}

	// end ////////////////////////////////////////////////////////////////////

	// EFFECT MOVER  - begin ////////////////////////////////////////////////

	void EffectMover::ChangeValue(double dt, cocos2d::Node * node)
	{
		float deltaValueX = dt * _targetValue / _Continuance;
		float deltaValueY = dt * _targetValue1 / _Continuance;
		_curValue = node->getPosition().x + deltaValueX;
		_curValue1 = node->getPosition().y + deltaValueY;
		node->setPosition(_curValue, _curValue1);
	}

	// end ////////////////////////////////////////////////////////////////////
}