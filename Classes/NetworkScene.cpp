#include "NetworkScene.h"

USING_NS_CC;

std::string const NetworkScene::JSON_MESSAGE("{\"type\": \"join\"}");

Scene* NetworkScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();
	auto layer = NetworkScene::create();

	scene->addChild(layer);

	return scene;
}

bool NetworkScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Create a label.
	auto label = Label::createWithSystemFont("Received: ", "Arial", 16);
	label->setPosition(Vec2(150, 100));
	this->addChild(label, 1);
	
	
	// Create a button
	auto button = ui::Button::create("Join_Button.png",
		"Join_Button(Click).png");
	button->setPosition(Vec2(200, 200));
	
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type){
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED:
			handleTouch(this);
			break;
		default:
			break;
		}
	});

	button->setName("JoinButton");
	this->addChild(button);	
	
	addEvents();	// Add scene touch handler

	mSocket = SWebSocket::create("ws://localhost:8081/test");

	mSocket->onMessageReceived = std::bind(&NetworkScene::onReceive, this, std::placeholders::_1);
	
	mSocket->connect();

	
	return true;
}

void NetworkScene::addEvents()
{
	//Create a "one by one" touch event listener (processes one touch at a time)
	auto listener1 = EventListenerTouchOneByOne::create();
	// When "swallow touches" is true, then returning 'true' from the onTouchBegan method will "swallow" the touch event, preventing other listeners from using it.
	listener1->setSwallowTouches(true);

	// Example of using a lambda expression to implement onTouchBegan event callback function
	listener1->onTouchBegan = [this](Touch* touch, Event* event){
		this->m_pos = touch->getLocation();
		updateState();
		return true;
	};

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener1, 30);
}


void NetworkScene::handleTouch(Ref *pSender)
{
	mSocket->send(JSON_MESSAGE);
	// Check if the join button is still visible.
	auto buttonNode = this->getChildByName("JoinButton");	
	if (buttonNode != nullptr)
	{
		// Remove the join button
		this->removeChildByName("JoinButton");
	}
}

void NetworkScene::onReceive(std::string message)
{	
	parseJSONMessage(message);
	std::string output = "Data: " + m_data + " Type: " + m_type;
	auto textNode = Director::getInstance()->getRunningScene()->getChildByName(TEXT_TAG);	
	if (textNode != nullptr)
	{
		m_text->setText(output);
	}
	else
	{
		m_text = ui::Text::create(output, "Arial", 16);
		m_text->setAnchorPoint(Vec2(0, 0));
		m_text->setText(output);
		m_text->setPosition(Vec2(100, 50));
		Director::getInstance()->getRunningScene()->addChild(m_text, 1, TEXT_TAG);					
	}
}

void NetworkScene::updateState()
{	
	CCLOG("Touch x,y pos = : %d, %d", this->m_pos.x, this->m_pos.y);	
	std::string touchX = std::to_string(this->m_pos.x);
	std::string touchY = std::to_string(this->m_pos.y);	
	std::string jsonMessage = "{\"type\": \"updateState\", \"data\" : {\"x\":" + touchX + ", \"y\":" + touchY + "}}";	
	mSocket->send(jsonMessage);
}


void NetworkScene::parseJSONMessage(std::string message)
{	
	m_document.Parse<0>(message.c_str());
	auto it1 = m_document.MemberonBegin();
	// Hold a reference to the "data" field which may be a number of a string (depending on the "type" field)
	auto & data = it1->value;
	it1++;
	// Get the "type" field which is always a string
	m_type = it1->value.GetString();	
	if (m_type == "gameState")
	{
		// get a number: 0 = WAITING_FOR_PLAYERS, 1 = STARTING_GAME
		int num = data.GetUint();		
		m_data = std::to_string(num);
	}
	else if (m_type == "error")
	{
		m_data = data.GetString();
	}
	else if (m_type == "updateState")
	{
		// Retrieve the touch position as two separate x,y values 	
		double x = data["x"].GetDouble();
		double y = data["y"].GetDouble();
		m_data = "x: " + std::to_string(x) + ", y: " + std::to_string(y);
	}
	
}
