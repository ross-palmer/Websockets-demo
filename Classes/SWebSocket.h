//
//  SWebSocket.h
//
//  Created by Yoshiaki Sugimoto on 2014/08/04.
//
//

#ifndef __SWebSocket_H__
#define __SWebSocket_H__

#include "cocos2d.h"
#include "network/WebSocket.h"

class SWebSocket: public cocos2d::network::WebSocket::Delegate
{
private:
    std::string _url;
    cocos2d::network::WebSocket* _websocket;
public:
    std::function<void(std::string message)> onMessageReceived;
    std::function<void()> onConnectionClosed;
    std::function<void(const cocos2d::network::WebSocket::ErrorCode &error)> onErrorOccurred;
    std::function<void()> onConnectionOpened;
    
    SWebSocket(std::string url);
    
    void connect();
    
    static SWebSocket* create(std::string url);
    
    virtual void onOpen(cocos2d::network::WebSocket* ws);
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data);
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error);
    virtual void onClose(cocos2d::network::WebSocket* ws);
    
    void close();
    void send(std::string mesage);
    
    ~SWebSocket();
};

#endif /* defined(__SWebSocket_H__) */