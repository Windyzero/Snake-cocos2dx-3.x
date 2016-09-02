#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

typedef enum {
	START=10,
	HELP,
	EXIT
}TAG_MENU;

class SnakeNode :public cocos2d::Ref
{
public:
	cocos2d::Point position;
	cocos2d::Point dir;
	SnakeNode* preNode;
	cocos2d::Sprite* nodeSprite;
	bool collisionWithSnakeNode(SnakeNode* snakeNode);
};

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();  
    void menuCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(HelloWorld);
};

class GameHelp : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	void menuBackToMain(cocos2d::Ref* pSender);
	CREATE_FUNC(GameHelp);
};

class GameScene : public cocos2d::Layer
{
	~GameScene();
public:
	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void update(float t);
	static cocos2d::Scene* createScene();
	virtual bool init();  
	void menuBackToMain(cocos2d::Ref* pSender);
	void die();
	void speadUp();
	CREATE_FUNC(GameScene);
	
protected:
	SnakeNode* sHead;
	SnakeNode* sFood;
	cocos2d::Vector<SnakeNode*> allbody;
	cocos2d::Label* _labelScore;
	cocos2d::Label* _labelWarning;
	cocos2d::Label* _labelSpeedUp;
	float _headSpeed;
	bool ajustDir(cocos2d::Touch* touch);
};

class GameOver : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();  
	void menuBackToMain(cocos2d::Ref* pSender);
	CREATE_FUNC(GameOver);
};

class Constant
{
public:
	static int score;
	static cocos2d::Size visibleSize;
};
#endif // __HELLOWORLD_SCENE_H__
