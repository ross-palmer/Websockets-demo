#include "NetworkScene.h"

USING_NS_CC;

std::string const NetworkScene::JSON_MESSAGE("{\"type\": \"join\", \"pid\" : \"Android client\"}");


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
	auto button = ui::Button::create("Connect_Button.png",
		"Connect_Button(Click).png");
	button->setPosition(Vec2(200, 200));

	button->addTouchEventListener(CC_CALLBACK_1(NetworkScene::handleTouch, this));
	this->addChild(button);
	
	mSocket = SWebSocket::create("ws://localhost:8080/wstest");

	mSocket->onMessageReceived = std::bind(&NetworkScene::onReceive, this, std::placeholders::_1);
	
	mSocket->connect();

	return true;
}

void NetworkScene::handleTouch(Ref *pSender)
{
	mSocket->send(JSON_MESSAGE);
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

void NetworkScene::parseJSONMessage(std::string message)
{	
	m_document.Parse<0>(message.c_str());
	auto it1 = m_document.MemberonBegin();
	// Get value of "data" 
	m_data = it1->value.GetString();
	// Get value of "type" 
	it1++;
	m_type = it1->value.GetString();
}
