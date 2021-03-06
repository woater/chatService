#ifndef CHATSERVER_MULTI_THREAD_SERVER_HPP__
#define CHATSERVER_MULTI_THREAD_SERVER_HPP__

#include "muduo/net/TcpServer.h"
#include "muduo/base/Logging.h"
#include <memory>
#include <muduo/net/Callbacks.h>
#include "../lengthCodec/lengthCodec.hpp"
#include <muduo/net/TcpConnection.h>
#include <muduo/base/ThreadLocalSingleton.h>
#include <mutex>

#include <set>

class ChatServer {
public:
    // 构造函数
    ChatServer(muduo::net::EventLoop* loop,
            const muduo::net::InetAddress& listenAddr);
    // 不可复制、拷贝
    ChatServer(const ChatServer&) = delete;
    ChatServer(const ChatServer&&) = delete;
    ChatServer& operator=(const ChatServer&) = delete;
    ChatServer& operator=(const ChatServer&&) = delete;

    // 处理连接（新连接或断开）
    void onConnection(const muduo::net::TcpConnectionPtr& connection);
    // bahavior on new message
    void onStringMessage(const muduo::net::TcpConnectionPtr& connection,
                        const std::string& msg,
                        muduo::Timestamp time);

    void setThreadNum(int numThreads);
    void start();
    void distributeMessage(const std::string& message);
    void threadInit(muduo::net::EventLoop* loop);

private:
    typedef std::set<muduo::net::TcpConnectionPtr> ConnectionSet;
    typedef std::shared_ptr<ConnectionSet> ConnectionSetPtr;

    // static thread_local ConnectionSet localConnections_;
    typedef muduo::ThreadLocalSingleton<ConnectionSet> localConnections_;
    
    std::mutex mutOfLoops_;
    std::set<muduo::net::EventLoop*> loops_;

    muduo::net::TcpServer server_;
    LengthCodec codec_;
};

#endif