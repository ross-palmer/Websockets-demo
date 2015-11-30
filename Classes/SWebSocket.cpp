//
//  SWebSocket.cpp
//
//  Created by Yoshiaki Sugimoto on 2014/08/04.
//
//

#include "SWebSocket.h"

USING_NS_CC;
using namespace cocos2d::network;

SWebSocket* SWebSocket::create(std::string url)
{
    SWebSocket* ws = new SWebSocket(url);
    
    return ws;
}

SWebSocket::SWebSocket(std::string url)
{
    _url = url;	
};

void SWebSocket::connect()
{
    _websocket = new WebSocket();
    bool result = _websocket->init(*this, _url.c_str());
	if (!result)
	{
		// There was a problem connecting to the websocket...
		CCLOG("Could not connect to: %s", _url.c_str());
	}
}

void SWebSocket::close()
{
    if (_websocket->getReadyState() == WebSocket::State::OPEN)
    {
        _websocket->close();
    }
}

SWebSocket::~SWebSocket()
{
    _websocket->close();
}

void SWebSocket::send(std::string message)
{
    if (_websocket->getReadyState() == WebSocket::State::OPEN)
    {
        _websocket->send(message);
    }
}

void SWebSocket::onOpen(WebSocket* ws)
{
    CCLOG("WebSocket connection opened: %s", _url.c_str());
    if ( onConnectionOpened )
    {
        onConnectionOpened();
    }
}

void SWebSocket::onMessage(WebSocket* ws, const WebSocket::Data &data)
{	
    if ( onMessageReceived )
    {
        onMessageReceived(data.bytes);
    }    
}

void SWebSocket::onError(WebSocket* ws, const WebSocket::ErrorCode& error)
{
    if ( onErrorOccurred )
    {
        onErrorOccurred(error);
    }
}

void SWebSocket::onClose(WebSocket* ws)
{
    if ( onConnectionClosed )
    {
        onConnectionClosed();
    }
}