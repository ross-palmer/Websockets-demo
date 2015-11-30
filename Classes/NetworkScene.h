#pragma once

#include "cocos2d.h"
#include <ui\CocosGUI.h>
#include "SWebSocket.h"
#include "json\document.h"
#include <string>

using namespace std::placeholders;

class NetworkScene : public cocos2d::Layer
{
private:
	static std::string const JSON_MESSAGE;
	rapidjson::Document m_document;	// A container for a JSON document
	std::string m_data;	
	std::string m_type;

	cocos2d::ui::Text * m_text; 
	std::string const TEXT_TAG = "PARSED_TEXT";	// Identifier for text widget.

	SWebSocket* mSocket;
	void parseJSONMessage(std::string message);

public:
	static cocos2d::Scene* createScene();
	virtual bool init();

	void handleTouch(Ref *pSender);
	void onReceive(std::string message);

	CREATE_FUNC(NetworkScene);
};