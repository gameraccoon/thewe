#ifndef BASE_EFFECT_H
#define BASE_EFFECT_H

#include <cocos2d.h>
#include <EffectsLayer.h>

namespace VisualEffects
{
	// Base classes declaration ///////////////////////////////////////////////
	class BaseEffectComposition;
	class BaseEffectProducer;
	class BaseEffectConductor;
	class BaseEffectPerformer;

	/**
	...
	*/
	class BaseEffectComposition : public Effect
	{
		friend class BaseEffectProducer;

	protected:
		BaseEffectProducer * _EffectProducer;

	public:	
		BaseEffectComposition (
			const std::string &name, 
			int zOrder,
			cocos2d::Node *relation = nullptr)
			: Effect(name, zOrder, relation)
		{
		}

		virtual bool init(void) override;
		virtual void update(float dt) override;
		virtual bool IsFinished(void) const;
	};

	// Base class for algorythms of effects and their ordering
	class BaseEffectProducer
	{
	public:
		virtual void update(float dt) = 0;
		virtual bool isFreezed(void) const = 0;

		inline bool isFinished(void) const
		{
			return _CurrentState == finished;
		}

	protected:

		enum FinishCheckingType { // cheking type of a effect's finish		
			overtime, endChanges
		} _CurrentFinishType;

		enum EffectStates { // work progresses states
			ready, working, paused, finished
		} _CurrentState;

		// a object influenced by -> this <- effect
		cocos2d::Node * _EffectComposition; 

		float _Continuance; // duration of -> this <- effect
		float _CurrentTimer; // current duration of working state

		virtual void checkFinish(void);

	public:
		BaseEffectProducer(cocos2d::Node * _effectComposition, float _continuance)
			: _Continuance(_continuance),
			_EffectComposition(_effectComposition),
			_CurrentFinishType(overtime),
			_CurrentState(ready),
			_CurrentTimer(0.0)
		{
		}

		BaseEffectProducer(BaseEffectComposition * _effectComposition, float _continuance)
			: _Continuance(_continuance),
			_EffectComposition(_effectComposition),
			_CurrentFinishType(overtime),
			_CurrentState(ready),
			_CurrentTimer(0.0)
		{
			_effectComposition->_EffectProducer = this;
		}


		BaseEffectProducer(const BaseEffectProducer & _otherCompositor)
		{
			_CurrentFinishType = _otherCompositor._CurrentFinishType;
			_CurrentState = _otherCompositor._CurrentState;
			_Continuance = _otherCompositor._Continuance;
			_CurrentTimer = 0.0;
			_EffectComposition = _otherCompositor._EffectComposition;
		}

		inline float getContinuance() { return _Continuance; }
		void setContinuance(float continuance) { _Continuance = continuance; }

		inline const cocos2d::Node * getComposition() const 
		{ return _EffectComposition; }

		inline virtual void beginEffect(void) {
			_CurrentState = working;
		}
		inline virtual void stopEffect(void) {
			_CurrentState = ready;
			_CurrentTimer = 0.0;
		}
		inline virtual void pauseEffect(void) {
			_CurrentState = paused;
		}
	};

	// дирижер всея эффектов //////////////////////////////////////////////////
	// Что умеет этот парень?
	// 1. Хранит указатели на все эффекты (фоновые и очередные)
	// 2. Реализует обновление всех эффектов в своей пожчинении
	// 3. Определяет во времени момент заедания или окончания пластинки
	class BaseEffectConductor : public BaseEffectProducer
	{		
	private:
		typedef std::list<BaseEffectPerformer*> EffectsQueue;
		typedef std::vector<BaseEffectPerformer*> BckgrndEffects;

		bool _infiniteRepeat = true;
		
		EffectsQueue _EffectsQueue, _tempQueue;
		BckgrndEffects _BckgrndEffects;

		BaseEffectPerformer * _currentEffect;		
		
	public:
		BaseEffectConductor (
			BaseEffectComposition * _effectComposition, 
			float _continuance = 1.0F)
			: BaseEffectProducer(_effectComposition, _continuance), 
			_currentEffect(nullptr)
		{
		}

		void update(float dt) override;
		bool isFreezed(void) const override;

		void beginEffect(void) override;
		void stopEffect(void) override;
		void pauseEffect(void) override;

		bool pushQueue(BaseEffectPerformer * _effectPerformer, float continuance = 0.0);
		bool pushBckgrnd(BaseEffectPerformer * _effectPerformer);
	};

	// Single effect-perfomer
	class BaseEffectPerformer : public BaseEffectProducer
	{
	protected:		
		double _targetValue;
		double _curValue;
		double _preValue;
		virtual void doEffect(float dt, cocos2d::Node * node = nullptr);
	public:
		virtual void update(float dt);
		virtual bool isFreezed(void) const;
		virtual void ChangeValue(double dt, cocos2d::Node * node) = 0;

	public:
		BaseEffectPerformer(cocos2d::Node * _effectComposition, float _continuance, double targetValue)
			:	BaseEffectProducer(_effectComposition, _continuance),
			_targetValue(targetValue)
		{
		}


	};


	class EffectScaler : public BaseEffectPerformer
	{
	public:
		EffectScaler(
			cocos2d::Node * _effectComposition,
			double _continuance, double _changeValue)
			: BaseEffectPerformer(_effectComposition, _continuance, _changeValue)
		{
		}


		void ChangeValue(double dt, cocos2d::Node * node) override;
		
	};

	class EffectRounder : public BaseEffectPerformer
	{
	public:
		EffectRounder(
			cocos2d::Node * _effectComposition,
			double _continuance, double _changeValue)
			: BaseEffectPerformer(_effectComposition, _continuance, _changeValue)
		{
		}

		void ChangeValue(double dt, cocos2d::Node * node) override;
	};

	class EffectFader : public BaseEffectPerformer
	{
	public:
		EffectFader(
			cocos2d::Node * _effectComposition,
			double _continuance, double _changeValue)
			: BaseEffectPerformer(_effectComposition, _continuance, _changeValue)
		{
		}

		void ChangeValue(double dt, cocos2d::Node * node) override;
	};


	class EffectMover : public BaseEffectPerformer
	{
	public:
		EffectMover(
			cocos2d::Node * _effectComposition,
			double _continuance, double x, double y)
			: BaseEffectPerformer(_effectComposition, _continuance, x)
		{
		}

		void ChangeValue(double dt, cocos2d::Node * node) override;

	private:
		double _targetValue1; // y
		double _curValue1;
		double _preValue1;
	};

}


#endif // BASE_EFFECT_H