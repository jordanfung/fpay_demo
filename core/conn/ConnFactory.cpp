#include <iostream>

#include "logging.h"
#include "Connection.h"
#include "ConnFactory.h"

using namespace std;

#define CONN_TIMEOUT   600000 //10 minutes
#define CONNECT_TIMEOUT  2000 //2 seconds

IConn *ConnFactory::createConnection(const std::string& ip, uint32_t port,
        IProtoConsumer *proto, IConnEventHandler *eh, CreateCallback *cc)
{
    DLOG_TRACE;
    try {
        Connection *conn = new Connection(ip, port, CONNECT_TIMEOUT, proto, eh, m_bTcpNodelay);

        conn->setProtoConsumer(proto);
        conn->setConnEventHandler(eh);
        conn->setTimeout(CONN_TIMEOUT);

        if (cc) { 
            cc->onConnCreate(conn);
        } 
        conn->addEvent(0, EVENT_CONNECTING);
        return conn;
    } catch (socketExn & se) {   
    }       

    return NULL; 
}     

IConn *ConnFactory::createConnection(int so, uint32_t ip, int port,
        IProtoConsumer *proto, IConnEventHandler *eH, CreateCallback *cc)
{
    DLOG_TRACE;
    Connection *conn = new Connection(so, ip, port, proto, eH, m_bTcpNodelay);

    conn->setProtoConsumer(proto);
    conn->setConnEventHandler(eH);
    conn->setTimeout(CONN_TIMEOUT);

    if (cc) {
        cc->onConnCreate(conn);
    }
    conn->addEvent(0, EVENT_READ);

    return conn;
}
