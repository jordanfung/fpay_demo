CC32 = gcc

ARRU = ar -ru

DEFINE_REACTOR=-DHAVE_EPOLL

DEF_PROTO=-D_JSON_PROTOCOL

LDFLAGS = -pthread -lrt -lssl
CFLAGS =  -g -O3
CXXFLAGS = -DTIXML_USE_STL -fpermissive -g \
        -I../cache \
        -I../core/base \
        -I../core/conn \
        -I../core/proto \
        -I../core/writer \
        -I../core/dispatcher \
        -I../core/xnet 

COMM_LIB = \
       ../cache/libcache.a \
       ../core/lib/dispatcher.a \
       ../core/lib/writer.a \
       ../core/lib/conn.a \
       ../core/lib/proto.a \
       ../core/lib/base.a \
       ../core/lib/xnet.a 
