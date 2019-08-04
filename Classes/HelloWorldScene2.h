#pragma once

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld2 : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld2);
	Sprite* m_pDuck;
};
