#ifndef __CONN_FACTORY__
#define __CONN_FACTORY__

#include "base.h"
#include "Socket.h"
#include "ProtoConsumer.h"

class ConnFactory
    : public ServerConnFactory
    , public ClientConnFactory
{
public:
    ConnFactory()
        : m_bTcpNodelay(true)
    {}
    IConn *createConnection(const std::string& ip, uint32_t port, IProtoConsumer *proto, IConnEventHandler *eh, CreateCallback *call); 
    virtual IConn *createConnection(int fd, uint32_t ip, int port, IProtoConsumer *h, IConnEventHandler *eH, CreateCallback *); 
    void setConnTcpNodelay(bool flag)
    {
        m_bTcpNodelay = flag;
    } 

protected:
    bool m_bTcpNodelay;
};

template<class C>
class ServerSideConnFactory : public ServerConnFactory
{
public:
    ServerSideConnFactory ()
    {}
    virtual IConn *createConnection(int fd, uint32_t ip, int port,
            IProtoConsumer *proto, IConnEventHandler *eH, CreateCallback *call) 
    {
        C *conn = new C(fd, ip, port, proto, eH, true);
        conn->setProtoConsumer(proto);
        conn->setConnEventHandler(eH);
        if(call) {
            call->onConnCreate(conn);
        }
        conn->addEvent(0, EVENT_READ);
        return conn;
    }   
};  

template<class C>
class ClientSideConnFactory : public ClientConnFactory
{
public:
    ClientSideConnFactory()
    {}
    virtual IConn *creatConnect(const std::string &ip, uint32_t port,
            IProtoConsumer *proto, IConnEventHandler *eH, CreateCallback *call) 
    {
        C *conn = new C(ip, port, 2000, proto, eH, true);
        conn->setProtoConsumer(proto);
        conn->setConnEventHandler(eH);
        if(call)
            call->onConnCreate(conn);
        conn->addEvent(0, EVENT_READ);
        return conn;
    }   
};  

#endif
