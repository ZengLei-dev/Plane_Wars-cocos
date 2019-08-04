#include"GameScene.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
bool CGameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//添加背景
	auto spBackgroud = Sprite::create("background1.jpg");
	spBackgroud->setPosition(Point::ZERO);
	spBackgroud->setAnchorPoint(Point::ZERO);
	this->addChild(spBackgroud);
	spBackgroud->setTag(10);

	auto spBackgroud2 = Sprite::create("background1.jpg");
	spBackgroud2->setPosition(Point::ZERO);
	spBackgroud2->setAnchorPoint(Point::ZERO);
	spBackgroud2->setPositionY(spBackgroud->getPositionY() + 600);
	this->addChild(spBackgroud2);
	spBackgroud2->setTag(11);


	//飞机动画播放
	auto spPlane = Sprite::create();
	spPlane->setTag(110);
	spPlane->setPosition(Point(120, 60));
	this->addChild(spPlane);
	Vector<SpriteFrame*> allframe; //保存所有帧
	for (size_t i = 0; i < 4; i++)
	{
		SpriteFrame* sf = SpriteFrame::create("plane00.png", Rect(i * 57, 0, 57, 85));
		allframe.pushBack(sf);
	}
	Animation* ani = Animation::createWithSpriteFrames(allframe, 0.1);
	spPlane->runAction(RepeatForever::create(Animate::create(ani)));

	//触摸控制飞机移动
	EventListenerTouchOneByOne* pEvent = EventListenerTouchOneByOne::create();
	pEvent->setSwallowTouches(true);
	//setTouchEnabled(true);
	pEvent->onTouchBegan = CC_CALLBACK_2(CGameScene::onTouchBegan,this);
	pEvent->onTouchMoved = CC_CALLBACK_2(CGameScene::onTouchMoved, this);
	pEvent->onTouchEnded = CC_CALLBACK_2(CGameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(pEvent, this);

	//处理子弹
	this->schedule(schedule_selector(CGameScene::newBullet), 0.5);
	this->schedule(schedule_selector(CGameScene::moveBullet), 0.01);

	//处理敌机飞行
	this->schedule(schedule_selector(CGameScene::newEnemy), 0.7);
	this->schedule(schedule_selector(CGameScene::moveEnemy), 0.025);

	//背景滚动
	this->schedule(schedule_selector(CGameScene::MoveBackground), 0.01);

	//处理游戏逻辑和碰撞检测
	this->scheduleUpdate();
	return true;

}

void CGameScene::MoveBackground(float fValue)
{
	auto spBackground = this->getChildByTag(10);
	auto spBackground2 = this->getChildByTag(11);
	spBackground->setPositionY(spBackground->getPositionY() - 1);
	if (spBackground->getPositionY() < -600)
	{
		spBackground->setPositionY(0);
	}
	spBackground2->setPositionY(spBackground->getPositionY() + 600);

}
Scene* CGameScene::creatScene()
{
	auto scene = Scene::create();
	auto layer = CGameScene::create();
	scene->addChild(layer);
	return scene;
}

bool CGameScene::onTouchBegan(Touch *pTouch, Event *unusd_Event)
{
	px = pTouch->getLocation().x;
	py = pTouch->getLocation().y;
	return true;

}

void CGameScene::onTouchMoved(Touch *pTouch, Event *unusd_Event)
{
	int mx = (pTouch->getLocation().x - px);
	int my = (pTouch->getLocation().y - py);
	auto spPlane = this->getChildByTag(110);

	auto moveBy = MoveBy::create(0.0001, Vec2(mx, my));//时间就是不能为0，不知为什么
	spPlane->runAction(moveBy);

	//spPlane->runAction(MoveTo::create(0, Point(mx, my)));
	//spPlane->setPosition(Point(pTouch->getLocation().x + mx, pTouch->getLocation().y + my));
	px = pTouch->getLocation().x;
	py = pTouch->getLocation().y;
}

void CGameScene::onTouchEnded(Touch *pTouch, Event *unusd_Event)
{
}

void CGameScene::newBullet(float fValue)
{
	auto spPlane = this->getChildByTag(110);
	Sprite* bullet = Sprite::create("bullert1.png");
	bullet->setPosition(spPlane->getPosition());
	this->addChild(bullet);
	this->allBullet.pushBack(bullet);
}

void CGameScene::moveBullet(float fTime)
{
	for (size_t i = 0; i < allBullet.size(); i++)
	{
		auto nowbullet = allBullet.at(i);
		nowbullet->setPositionY(nowbullet->getPositionY() + 3);
		if (nowbullet->getPositionY() > Director::getInstance()->getWinSize().height)
		{
			nowbullet->removeFromParent();
			allBullet.eraseObject(nowbullet);
			i--;
		}
	}

}

void CGameScene::newEnemy(float fValue)
{
	Sprite * enemy = nullptr;
	int num = rand() % 8;
	if (num < 4)
	{
		enemy = Sprite::create("enemy1.png");
	}
	else if (num>=4&&num<=7)
	{
		enemy = Sprite::create("enemy3.png");
	}
	//else if(num >= 8 && num <= 9)
	//{
	//	enemy = Sprite::create("boss0.png");
	//	
	//}
	enemy->setPosition(Point(rand() % 280 + 20, 600));
	this->addChild(enemy);
	this->allEnemy.pushBack(enemy);
}

void CGameScene::moveEnemy(float fTime)
{
	for (size_t i = 0; i < allEnemy.size(); i++)
	{
		auto nowEnemy = allEnemy.at(i);
		nowEnemy->setPositionY(nowEnemy->getPositionY() - 3);
		if (nowEnemy->getPositionY() < 0)
		{
			nowEnemy->removeFromParent();
			allEnemy.eraseObject(nowEnemy);
			i--;
		}
	}

}

void CGameScene::update(float fValue)
{
	//碰撞检测
	auto spPlane = this->getChildByTag(110);
	Rect rp(spPlane->getPositionX(), spPlane->getPositionY(), 40, 35);
	for (size_t i = 0; i < allEnemy.size(); i++)
	{
		auto nowEnemy = allEnemy.at(i);
		Rect er(nowEnemy->getPositionX(), nowEnemy->getPositionY(), 40, 35);
		//和飞机碰撞检测
		if (rp.intersectsRect(er))
		{
			//爆照效果
			NewBomb(spPlane->getPositionX(), spPlane->getPositionY());
			//NewBomb(nowEnemy->getPositionX(), nowEnemy->getPositionY());
			//敌机消失
			nowEnemy->removeFromParent();
			allEnemy.eraseObject(nowEnemy);
			i--;
			//跳转到gameover
			Director::getInstance()->getActionManager()->pauseAllRunningActions();
			this->pauseSchedulerAndActions();
			auto spover = Sprite::create("duck.png");
			spover->setPosition(Point(50,100));
			spover->setAnchorPoint(Point::ZERO);
			this->addChild(spover);
			auto act = Sequence::create(
				DelayTime::create(2),
				CallFunc::create(this,callfunc_selector(CGameScene::jumpMenu)),
				NULL
				);
			this->runAction(act);
		}
		for (size_t j = 0; j < allBullet.size(); j++)
		{
			auto nowbullet = allBullet.at(j);
			Rect br(nowbullet->getPositionX(), nowbullet->getPositionY(), 20, 30);
			if (er.intersectsRect(br)) //子弹碰到敌机
			{
				//爆炸效果
				NewBomb(nowbullet->getPositionX(), nowbullet->getPositionY());

				//子弹消失
				nowbullet->removeFromParent();
				allBullet.eraseObject(nowbullet);

				//敌机消失
				nowEnemy->removeFromParent();
				allEnemy.eraseObject(nowEnemy);
				i--;
				break;

				
			}
		}
	}
}

void CGameScene::NewBomb(int x, int y) //爆炸效果
{
	Vector<SpriteFrame*> allframe;
	for (size_t i = 0; i < 4; i++)
	{
		SpriteFrame* sf = SpriteFrame::create("bomb1.png", Rect(i * 36, 0, 36, 36));
		allframe.pushBack(sf);
	}
	Animation* ani = Animation::createWithSpriteFrames(allframe, 0.08);
	auto sprite = Sprite::create();
	Action* act = Sequence::create(
		Animate::create(ani), //动画
		CCCallFuncN::create(sprite, callfuncN_selector(CGameScene::KillMe)),//删除自己
		NULL);
	this->addChild(sprite);
	sprite->setPosition(Point(x, y));
	sprite->runAction(act);
	
}
void CGameScene::KillMe(Node* pSender) //删除自己
{
	pSender->setOpacity(0);
	pSender->removeAllChildrenWithCleanup(true);
	
}

void CGameScene::jumpMenu() //跳转到主菜单
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}