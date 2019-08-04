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

	void MoveBackground(float fValue);//��������

	virtual bool onTouchBegan(Touch *pTouch, Event *unusd_Event);
	virtual void onTouchMoved(Touch *pTouch, Event *unusd_Event);
	virtual void onTouchEnded(Touch *pTouch, Event *unusd_Event);

	int px, py;  //�ɻ�����

	Vector<Sprite*> allBullet; //�����ӵ�
	void newBullet(float fValue);
	void moveBullet(float fTime);

	Vector<Sprite*>allEnemy;//���ел�
	void newEnemy(float fValue);
	void moveEnemy(float fTime);

	void update(float fValue);

	void NewBomb(int x, int y); //��ըЧ��
	void KillMe(Node* pSender); //ɾ���Լ�

	void jumpMenu();
};


#endif