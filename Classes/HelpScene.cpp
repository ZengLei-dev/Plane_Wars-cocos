#include"HelpScene.h"

bool CHelpScene::init()
{
	if (!Layer::init())
	{
		return false;
	}
	auto spbk = Sprite::create("help.jpg");
	spbk->setPosition(Point::ZERO);
	spbk->setAnchorPoint(Point::ZERO);
	this->addChild(spbk);
	return true;

}

Scene* CHelpScene::creatScene()
{
	auto scene = Scene::create();
	auto layer = CHelpScene::create();
	scene->addChild(layer);
	return scene;
}