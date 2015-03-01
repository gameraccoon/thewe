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
		BaseEffectConductor * _effectProducer;

	public:
		BaseEffectComposition(
			const std::string &name,
			int zOrder,
			cocos2d::Node *relation = nullptr)
			: Effect(name, zOrder, relation)
			, _effectProducer(nullptr)
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

		typedef std::list<BaseEffectProducer*> EffectsQueue;
		typedef std::vector<BaseEffectProducer*> BckgrndEffects;

		enum FinishCheckingType {	
			overtime, endChanges
		} 
		_curFinishType;

		enum EffectStates {
			ready, working, paused, finished
		} 
		_curState;

		bool _infiniteRepeat;
		bool _isNeverending;

	public:
		virtual void Update(float dt) = 0;

		virtual bool IsFreezed(void) const = 0;

		inline bool IsFinished(void) const {
			return _curState == finished;
		}

		inline bool SetParent(BaseEffectProducer *composite) {
			if (composite->IsComposite()) _parent = composite;
		}

		virtual bool IsComposite(void) const {
			return false;
		}	

	public:
		BaseEffectProducer(cocos2d::Node *effectComposition, float continuance)
			: _continuance(continuance)
			, _effectComposition(effectComposition)
			, _curFinishType(overtime)
			, _curState(ready)
			, _curTimer(0.0f)
			, _infiniteRepeat(false)
			, _isNeverending(false)
		{}

		BaseEffectProducer(cocos2d::Node *effectComposition)
			: _continuance(0.0f)
			, _effectComposition(effectComposition)
			, _curFinishType(overtime)
			, _curState(ready)
			, _curTimer(0.0f)
			, _infiniteRepeat(false)
			, _isNeverending(false)
		{}

		BaseEffectProducer(const BaseEffectProducer & otherCompositor)
			: _continuance(otherCompositor._continuance)
			, _effectComposition(otherCompositor._effectComposition)
			, _curFinishType(otherCompositor._curFinishType)
			, _curState(ready)
			, _curTimer(0.0f)
			, _infiniteRepeat(otherCompositor._infiniteRepeat)
		{}

		float GetContinuance() const { 
			return _continuance; }

		void SetContinuance(float continuance) { 
			_continuance = continuance; }

		inline cocos2d::Node * GetComposition(void) const {
			return _effectComposition;
		}

		inline virtual void BeginEffect(void) {
			if (IsFinished()) _curTimer = 0.0f;
			_curState = working;
		}
		inline virtual void StopEffect(void) {
			_curState = ready;
			_curTimer = 0.0;
		}
		inline virtual void PauseEffect(void) {
			_curState = paused;
		}

		virtual bool PushQueue(BaseEffectProducer * effect, 
			float continuance) {
			return false;
		}
		virtual bool PushBckgrnd(BaseEffectProducer * effect, 
			float continuance) {
			return false;
		}

	protected:
		// a object influenced by -> this <- effect
		cocos2d::Node * _effectComposition;

		BaseEffectProducer *_parent;

		float _continuance; // duration of -> this <- effect
		float _curTimer; // actual duration of working state

		virtual void checkFinish(void);
	};

	// дирижер всея эффектов //////////////////////////////////////////////////
	// Что умеет этот парень?
	// 1. Хранит указатели на все эффекты (фоновые и очередные)
	// 2. Реализует обновление всех эффектов в своей поlчинении
	// 3. Определяет во времени момент заедания или окончания пластинки
	class BaseEffectConductor : public BaseEffectProducer
	{
	private:
		EffectsQueue _effectsQueue, _workingQueue;
		BckgrndEffects _bgEffects;

		BaseEffectProducer * _curEffect;

	public:
		BaseEffectConductor (cocos2d::Node *_effectComposition,
			float continuance = 1.0f)
			: BaseEffectProducer(_effectComposition, continuance)
			, _curEffect(nullptr)
		{}

		void Update(float dt) override;
		bool IsFreezed(void) const override;

		bool IsComposite(void) const override {
			return true;
		}

		void BeginEffect(void) override;
		void StopEffect(void) override;
		void PauseEffect(void) override;

		bool PushQueue(BaseEffectProducer * _effectPerformer, 
			float continuance) override;
		bool PushBckgrnd(BaseEffectProducer * _effectPerformer,
			float continuance) override;
	};

	// Single effect-perfomer
	class BaseEffectPerformer : public BaseEffectProducer
	{
	public:
		void Update(float dt) override;
		bool IsFreezed(void) const override;

		virtual void ChangeValue(double dt, cocos2d::Node * node) = 0;

	public:
		BaseEffectPerformer(cocos2d::Node * effectComposition, double targetValue)
			: BaseEffectProducer(effectComposition),
			_targetValue(targetValue)
		{}

	protected:
		double _targetValue;
		double _curValue;
		double _preValue;
		virtual void DoEffect(float dt, cocos2d::Node * node = nullptr);

	private:
		BaseEffectProducer* _parent;
	};

	class EffectNothing : public BaseEffectPerformer
	{
	public:
		EffectNothing(cocos2d::Node * effectComposition)
			: BaseEffectPerformer(effectComposition, 0.0f)
		{}

		void ChangeValue(double dt, cocos2d::Node * node) override
		{}
	};

	class EffectScaler : public BaseEffectPerformer
	{
	public:
		EffectScaler (cocos2d::Node * effectComposition, double changeValue)
			: BaseEffectPerformer(effectComposition, changeValue)
		{}

		void ChangeValue(double dt, cocos2d::Node * node) override;
	};

	class EffectRounder : public BaseEffectPerformer
	{
	public:
		EffectRounder (cocos2d::Node * effectComposition, double changeValue)
			: BaseEffectPerformer(effectComposition, changeValue)
		{}

		void ChangeValue(double dt, cocos2d::Node * node) override;
	};

	class EffectFader : public BaseEffectPerformer
	{
	public:
		EffectFader(cocos2d::Node * effectComposition, double changeValue)
			: BaseEffectPerformer(effectComposition, changeValue)
		{}

		void ChangeValue(double dt, cocos2d::Node * node) override;
	};

	class EffectMover : public BaseEffectPerformer
	{
	public:
		EffectMover (cocos2d::Node * effectComposition, double x, double y)
			: BaseEffectPerformer(effectComposition, x)
			, _targetValue1(y)
		{}

		void ChangeValue(double dt, cocos2d::Node * node) override;

	private:
		double _targetValue1; // y
		double _curValue1;
		double _preValue1;
	};

}


#endif // BASE_EFFECT_H