#include "TransitionZoomFade.h"


TransitionZoomFade::TransitionZoomFade(float t, Scene *scene)
{
	cocos2d::TransitionScene::initWithDuration(t, scene);
}

TransitionZoomFade* TransitionZoomFade::create(float t, Scene* scene)
{
	TransitionZoomFade* newScene = new TransitionZoomFade(t, scene);
	if(newScene)
	{
		newScene->autorelease();
		return newScene;
	}
	CC_SAFE_DELETE(newScene);
	return nullptr;
}

void TransitionZoomFade::onEnter()
{
	TransitionScene::onEnter();
	cocos2d::Size s = cocos2d::Director::getInstance()->getWinSize();

	_inScene->setScale(0.5f);
	_inScene->setPosition(cocos2d::Vec2(s.width, 0));
	_inScene->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	_outScene->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));

	cocos2d::ActionInterval *jump = cocos2d::MoveBy::create(_duration/4, cocos2d::Vec2(-s.width, 0.0f));
	cocos2d::ActionInterval *scaleIn = cocos2d::ScaleTo::create(_duration/4, 1.0f);
	cocos2d::ActionInterval *scaleOut = cocos2d::ScaleTo::create(_duration/4, 0.5f);

	cocos2d::ActionInterval *jumpZoomOut = (cocos2d::ActionInterval*)(cocos2d::Sequence::create(scaleOut, jump, nullptr));
	cocos2d::ActionInterval *jumpZoomIn = (cocos2d::ActionInterval*)(cocos2d::Sequence::create(jump, scaleIn, nullptr));

	cocos2d::ActionInterval *delay = cocos2d::DelayTime::create(_duration/2);

	_outScene->runAction(jumpZoomOut);
	_inScene->runAction
	(
		cocos2d::Sequence::create
		(
			delay,
			jumpZoomIn,
			cocos2d::CallFunc::create(CC_CALLBACK_0(TransitionScene::finish,this)),
			nullptr
		)
	);
}
