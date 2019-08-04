#ifndef GAME_SENCE
#define GAME_SENCE
#include"cocos2d.h"
using namespace cocos2d;

class CGameScene:Layer
{
public:
	bool init();
	CREATE_FUNC(CGameScene);
	static Scene* creatScene();

	void MoveBackground(float fValue);//背景滚动

	virtual bool onTouchBegan(Touch *pTouch, Event *unusd_Event);
	virtual void onTouchMoved(Touch *pTouch, Event *unusd_Event);
	virtual void onTouchEnded(Touch *pTouch, Event *unusd_Event);

	int px, py;  //飞机坐标

	Vector<Sprite*> allBullet; //所有子弹
	void newBullet(float fValue);
	void moveBullet(float fTime);

	Vector<Sprite*>allEnemy;//所有敌机
	void newEnemy(float fValue);
	void moveEnemy(float fTime);

	void update(float fValue);

	void NewBomb(int x, int y); //爆炸效果
	void KillMe(Node* pSender); //删除自己

	void jumpMenu();
};


#endif