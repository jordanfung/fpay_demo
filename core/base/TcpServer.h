#ifndef __TCP_SERVER_H__
#define __TCP_SERVER_H__

#include <vector>
#include <set>

#include "TcpAcceptor.h"
#include "IAbstractServer.h"

class TcpServer;

class StandaloneTcpAcceptor: public TcpAcceptor
{
protected:
    TcpServer *impl;

public:
    StandaloneTcpAcceptor(TcpServer *server, int fd)
        : TcpAcceptor(fd)
        , impl(server)
    {}
    virtual ~StandaloneTcpAcceptor()
    {}
    virtual void onAccept(int client, uint32_t ip, int port);
    void start();
};  

class TcpServer: public IAbstractServer
{
protected:
    StandaloneTcpAcceptor* _acceptor;

    friend class StandaloneTcpAcceptor;

    bool _bOpenAllow;
    uint16_t _listenPort;
    std::string _ip;
    std::set<uint32_t> _allowIps;

public:
    TcpServer(uint16_t port)
        : _bOpenAllow(false)
        , _listenPort(port)
    {}
    virtual ~TcpServer()
    {
        delete _acceptor;
        _acceptor = NULL;
    }
    void onAccept(int fd, uint32_t ip, uint16_t port);

    virtual StandaloneTcpAcceptor* createAcceptor(const char *ip, uint16_t port);

    void setAllowIPs(const std::set<uint32_t> &ips)
    {
        _allowIps = ips;
    }

    virtual std::string getIp()
    {
        return _ip;
    }

    virtual void setPort(uint16_t port)
    {
        _listenPort = port;
    }

    virtual uint16_t getPort()
    {
        return _listenPort;
    }

    virtual void start();
};

#endif
