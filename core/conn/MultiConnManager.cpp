#include <iostream>
#include <stdio.h>

#include "logging.h"
#include "Sender.h"
#include "MultiConnManager.h"

using namespace std;

MultiConnManager::MultiConnManager()
    : cid(0)
{
}

MultiConnManager::~MultiConnManager()
{
    for (connect_t::iterator it = connects.begin(); it != connects.end(); ++it) {
        delete (*it).second;
    }
}

void MultiConnManager::eraseConnection(IConn *conn)
{
    connect_t::size_type sz =connects.erase(conn->getConnId());
    if (sz != 0) {
        delete conn;
    }
}

void MultiConnManager::eraseConnectionById(cid_t id)
{
    connect_t::iterator it = connects.find(id);
    if (it != connects.end()) {
        eraseConnection(it->second);
    }
}

IConn *MultiConnManager::getConnById(cid_t id)
{
    connect_t::iterator it = connects.find(id);
    return it == connects.end() ? NULL : it->second;
}

bool MultiConnManager::dispatchByIds(const std::set<cid_t> &ids, Sender &resp,  uint32_t exp)
{
    resp.endPack();

    for (std::set<cid_t>::const_iterator it = ids.begin(); it!= ids.end(); ++it) {
        if (exp != (uint32_t)-1 && exp == *it) {
            continue;
        }

        IConn *conn = *it < connects.size() ? connects[*it] : NULL;
        if (!conn) {
            return false;
        }

        try {
            conn->sendBin(resp.header(), resp.headerSize() + resp.bodySize(), resp.getUri());
        } catch(std::exception &se) {
            conn->setEnable(false);
            lazyDelIds.insert(*it);
            LOG_WARN << "SocketError in multi dispatch ids, err:" << se.what();
        }
    }

    return true;
}

bool MultiConnManager::dispatchById(cid_t cid, Sender &request)
{
    connect_t::iterator it = connects.find(cid);
    if (it != connects.end()) {
        try {
            (*it).second->send(request);
            return true;
        } catch (std::exception &se){
            LOG_WARN << "dispatch by cid:" << cid 
                << " err: " << se.what() 
                << " uri:" << request.getUri() 
                <<  " size:" << request.bodySize();
            (*it).second->setEnable(false);
            lazyDelIds.insert(cid);
        }
    } else {
        LOG_INFO << "could not dispatch response: " << cid;
    }
    
    return false;
}

void MultiConnManager::onConnCreate(IConn *conn)
{
    conn->setConnId(++cid);
    conn->setSerialId(cid);
    connects[conn->getConnId()] = conn;
    conn->setConnEventHandler(this);
}

