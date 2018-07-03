#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

#include "logging.h"
#include "MasterConn.h"
#include "TcpServerMaster.h"

using namespace std;

void MasterAcceptor::onAccept(int so, uint32_t ip, int port){
    DLOG_TRACE;
    master->onAccept(so, ip, port);
}

void MasterAcceptor::start(){
    DLOG_TRACE;
    addEvent(0, EVENT_READ);
}


TcpServerMaster::TcpServerMaster()
    : m_bOpenAllow(false)
{
}

TcpServerMaster::TcpServerMaster(unsigned short port, uint8_t numWorker, const char* exec, const char * workerConf)
    : IWorkerManager(numWorker, exec, workerConf), _listenPort(port), m_bOpenAllow(false)
{
}

void TcpServerMaster::onAccept(int so, uint32_t ip, int port)
{
    DLOG_TRACE;
    if (m_bOpenAllow)
    {
        if(!allowIps.empty() && allowIps.find(ip) == allowIps.end()) {
            LOG_WARN << "deny connection from :" << addr_ntoa(ip);
            close(so);
            return;
        }           
    }           
    LOG_INFO << "Accept from from :" << addr_ntoa(ip) << ":" << port;
    MasterConn *conn = (MasterConn*)_connManager->createServerSideConn(so, ip, port, getProtoConsumer(), getConnEventHandler());
    conn->setWorkerManager(this);
}

TcpServerMaster::~TcpServerMaster()
{
    delete _acceptor;
    _acceptor = NULL;
}

MasterAcceptor *TcpServerMaster::createAcceptor(const char *ip, uint16_t port)
{
    DLOG_TRACE;
    try {
        Socket sock; 
        sock.socket();
        sock.setreuse();
        sock.setblocking(false);
        sock.setnodelay();
        sock.bind(ip, port);
        MasterAcceptor *acceptor = new MasterAcceptor(this, sock.detach());
        LOG_INFO << "create listen port " << _listenPort << " success";
        return acceptor;
    } catch(socketExn se) {
        LOG_WARN << "alloc port conflict port:" << _listenPort;
    }

    return NULL;
}

void TcpServerMaster::setAllowIPs( const std::set<uint32_t> &setIPs )
{
    allowIps = setIPs;
}

void TcpServerMaster::start()
{
    if (_listenPort != 0) {
        _acceptor = createAcceptor(NULL, _listenPort);
        if (!_acceptor) {
            LOG_WARN << "acceptor not available";
            return;
        }

        _acceptor->start();
    }

    startWorker();
}
