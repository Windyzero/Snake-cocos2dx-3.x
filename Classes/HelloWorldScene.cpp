#include "HelloWorldScene.h"
#include "CCEventDispatcher.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	Constant::score = 0;

	auto labelStart = Label::create("StartGame", "Arial", 24);
	auto labelHelp = Label::create("GameHelp", "Arial", 24);
	auto labelExit = Label::create("ExitGame", "Arial", 24);

	auto startItem = MenuItemLabel::create(labelStart, CC_CALLBACK_1(HelloWorld::menuCallback, this));
	startItem->setTag(START);
	startItem->setPosition(Point(100, 200));

	auto helpItem = MenuItemLabel::create(labelHelp, CC_CALLBACK_1(HelloWorld::menuCallback, this));
	helpItem->setTag(HELP);
	helpItem->setPosition(Point(100, 150));

	auto exitItem = MenuItemLabel::create(labelExit, CC_CALLBACK_1(HelloWorld::menuCallback, this));
	exitItem->setTag(EXIT);
	exitItem->setPosition(Point(100, 50));

    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCallback, this));
    
	closeItem->setPosition(Point(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));
	closeItem->setTag(EXIT);
    auto menu = Menu::create(startItem, helpItem, exitItem, closeItem, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1);

    auto label = Label::create("Snake", "Arial", 24);
    label->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
	label->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->addChild(label, 1);

    return true;
}


void HelloWorld::menuCallback(Ref* pSender)
{
	switch (((Node*)pSender)->getTag())
	{
	case START:
		CCLOG("go to game");
		Director::getInstance()->replaceScene(GameScene::createScene());
		break;
	case  HELP:
		CCLOG("go to help");
		Director::getInstance()->replaceScene(GameHelp::createScene());
		break;
	case EXIT:
		Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		exit(0);
#endif
	}
}

bool GameHelp::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	Size visibleSize = Director::getInstance()->getVisibleSize();

	auto labelHelp = Label::create("please touch screen to play the game", "Arial", 15);
	labelHelp->setPosition(Point(visibleSize.width / 2, 280));
	labelHelp->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(labelHelp);

	auto labelBack = Label::create("Main Menu", "Arial", 15);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameHelp::menuBackToMain, this));
	itemBack->setPosition(Point(visibleSize.width / 2, 160));

	auto menu = Menu::create(itemBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	return true;
}

Scene* GameHelp::createScene()
{
	auto scene = Scene::create();
	auto layer = GameHelp::create();
	scene->addChild(layer);
	return scene;
}

void GameHelp::menuBackToMain( cocos2d::Ref* pSender )
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

bool GameScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Constant::visibleSize = visibleSize;

	_headSpeed = 50;

	_labelScore = Label::create("Score: 0", "Arial", 15);
	_labelScore->setAnchorPoint(Point::ANCHOR_TOP_RIGHT);
	_labelScore->setPosition(Point(visibleSize.width, visibleSize.height));
	this->addChild(_labelScore, 2);

	_labelWarning = Label::create("Warning: The snake can't go back", "Arial", 20);
	_labelWarning->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_labelWarning->setPosition(Point(visibleSize.width / 2 , 280));
	_labelWarning->setColor(Color3B(255, 0, 0));
	_labelWarning->setOpacity(0);
	_labelWarning->setVisible(false);
	this->addChild(_labelWarning, 2);

	_labelSpeedUp = Label::create("Speed Up !", "Arial", 20);
	_labelSpeedUp->setAnchorPoint(Point::ANCHOR_MIDDLE);
	_labelSpeedUp->setPosition(Point(visibleSize.width / 2 , 240));
	_labelSpeedUp->setColor(Color3B(255, 0, 0));
	_labelSpeedUp->setOpacity(0);
	_labelSpeedUp->setVisible(false);
	this->addChild(_labelSpeedUp, 2);

	auto labelBack = Label::create("Main Menu", "Arial", 15);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameScene::menuBackToMain, this));
	itemBack->setPosition(Point(visibleSize.width, 0));
	itemBack->setAnchorPoint(Point::ANCHOR_BOTTOM_RIGHT);

	auto menu = Menu::create(itemBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu, 2);

	sHead = new SnakeNode();
	sHead->preNode = NULL;
	sHead->position = Point(rand() % (int)(visibleSize.width - 32)  + 16, rand() % 288 + 16);
	sHead->dir = Point::ZERO;

	sFood = new SnakeNode();
	sFood->position = Point(rand() % (int)(visibleSize.width - 32) + 16, rand() % 288 + 16);

	auto snakeNode = Sprite::create("node_snake.png");
	snakeNode->setAnchorPoint(Point::ANCHOR_MIDDLE);
	snakeNode->setPosition(sHead->position);
	sHead->nodeSprite = snakeNode;
	this->addChild(snakeNode);

	auto foodNode = Sprite::create("node_food.png");
	foodNode->setAnchorPoint(Point::ANCHOR_MIDDLE);
	foodNode->setPosition(sFood->position);
	sFood->nodeSprite = foodNode;
	this->addChild(foodNode, 1);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);

	touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	this->scheduleUpdate();
	return true;
}

Scene* GameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void GameScene::menuBackToMain( cocos2d::Ref* pSender )
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

bool GameScene::onTouchBegan( Touch* touch, Event* event )
{
	return ajustDir(touch);
}

void GameScene::update( float t )
{
	for (int i = allbody.size() - 1; i >= 0; i--)
	{
		auto sn = allbody.at(i);
		if (sHead->collisionWithSnakeNode(sn) && i != 0)
		{
			this->unscheduleUpdate();
			sn->nodeSprite->runAction(Sequence::createWithTwoActions(
				Blink::create(3.0f, 6),
				CallFunc::create(CC_CALLBACK_0(GameScene::die, this))));
		}
		sn->dir = sn->preNode->position - sn->position;
		sn->position += sn->dir * t * _headSpeed / 32;
		sn->nodeSprite->setPosition(sn->position);
	}
	if (sHead->dir.getLength() != 0)
	{
		sHead->position += sHead->dir.normalize() * _headSpeed * t;
		if (!Rect(0, 0, Constant::visibleSize.width, 320).containsPoint(sHead->position))
		{
			this->unscheduleUpdate();
			sHead->nodeSprite->runAction(Sequence::createWithTwoActions(
				Blink::create(3.0f, 6),
				CallFunc::create(CC_CALLBACK_0(GameScene::die, this))));
		}		
		sHead->nodeSprite->setPosition(sHead->position);
	}
	if (sHead->collisionWithSnakeNode(sFood))
	{
		Constant::score += 100;
		char s[100] = {0};
		sprintf(s, "Score: %d", Constant::score);
		_labelScore->setString(std::string(s));

		if (Constant::score % 1000 == 0)
		{
			speadUp();
		}

		sFood->position = Point(rand() % (int)(Constant::visibleSize.width - 32) + 16, rand() % 288 + 16);;
		sFood->nodeSprite->setPosition(sFood->position);
		auto snakeNode = new SnakeNode();
		if (allbody.empty())
		{
			snakeNode->preNode = sHead;
		}
		else
		{
			snakeNode->preNode = allbody.back();
		}

		snakeNode->position = snakeNode->preNode->position;

		auto nodeSprite = Sprite::create("node_snake.png");
		nodeSprite->setPosition(snakeNode->position);
		nodeSprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
		snakeNode->nodeSprite = nodeSprite;
		this->addChild(nodeSprite);

		allbody.pushBack(snakeNode);
	}
}

void GameScene::die()
{
	Director::getInstance()->replaceScene(GameOver::createScene());
}

void GameScene::speadUp()
{
	_headSpeed *= 1.5;
	_labelSpeedUp->setVisible(true);
	_labelSpeedUp->runAction(Sequence::createWithTwoActions(
		Repeat::create(Sequence::createWithTwoActions(FadeIn::create(0.5f), FadeOut::create(0.5f)), 3),
		CallFunc::create([&](){_labelSpeedUp->setVisible(false);})));
}

void GameScene::onTouchMoved( cocos2d::Touch* touch, cocos2d::Event* event )
{
	ajustDir(touch);
}

bool GameScene::ajustDir( cocos2d::Touch* touch )
{
	Point touchPoint = this->convertTouchToNodeSpace(touch);

	Point dest_dir = touchPoint - sHead->position;

	if (sHead->dir.getLength() != 0)
	{
		if (fabs(sHead->dir.getAngle(dest_dir)) > M_PI_4 * 3.0)
		{
			if (!_labelWarning->isVisible())
			{
				_labelWarning->setVisible(true);
				_labelWarning->runAction(Sequence::createWithTwoActions(
					Repeat::create(Sequence::createWithTwoActions(FadeIn::create(0.5f), FadeOut::create(0.5f)), 3),
					CallFunc::create([&](){_labelWarning->setVisible(false);})));
			}			
			return false;
		}

	}

	sHead->dir = Point(dest_dir);
	return true;
}

GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(sHead);
	CC_SAFE_RELEASE_NULL(sFood);
	for (auto sn : allbody)
	{
		CC_SAFE_RELEASE_NULL(sn);
	}
}




bool SnakeNode::collisionWithSnakeNode( SnakeNode* snakeNode )
{
	return (abs(position.x - snakeNode->position.x) < 16 && abs(position.y - snakeNode->position.y) < 16);
}

bool GameOver::init()
{
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();

	char s[100] = {0};
	sprintf(s, "Your score is %d", Constant::score);

	auto labelHelp = Label::create("Game Over", "Arial", 40);
	labelHelp->setPosition(Point(visibleSize.width / 2, 280));
	labelHelp->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(labelHelp);

	auto labelScore = Label::create(s, "Arial", 20);
	labelScore->setPosition(Point(visibleSize.width / 2, visibleSize.height / 2));
	labelScore->setAnchorPoint(Point::ANCHOR_MIDDLE);
	this->addChild(labelScore);

	auto labelBack = Label::create("Main Menu", "Arial", 15);
	auto itemBack = MenuItemLabel::create(labelBack, CC_CALLBACK_1(GameOver::menuBackToMain, this));
	itemBack->setPosition(Point(visibleSize.width / 2, 40));

	auto menu = Menu::create(itemBack, NULL);
	menu->setPosition(Point(0, 0));
	this->addChild(menu);

	return true;
}

Scene* GameOver::createScene()
{
	auto scene = Scene::create();
	auto layer = GameOver::create();
	scene->addChild(layer);
	return scene;
}

void GameOver::menuBackToMain( cocos2d::Ref* pSender )
{
	Director::getInstance()->replaceScene(HelloWorld::createScene());
}

int Constant::score = 0;
Size Constant::visibleSize = Size::ZERO;
