#include <iostream>
#include "AbstractConn.h"

using namespace std;

AbstractConn::AbstractConn() : _timeout(-1) {}

void AbstractConn::notifyErr(int err, const char *msg){
    if(getConnEventHandler()){
       getConnEventHandler()->onError(err, msg, this);
    }else{
        //delete this;
    }
}

void AbstractConn::setTimeout(int tm){
    _timeout = tm;
}   

void *AbstractConn::getData(){
    return NULL;
}

void AbstractConn::send(Sender &resp) {
    resp.endPack();
    sendBin(resp.header(), resp.headerSize() + resp.bodySize(), resp.getUri());
}
