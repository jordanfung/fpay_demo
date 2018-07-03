#!/bin/sh

./daemon_server -conf ../conf/root_server.conf -exec ./node_server -port 9090 -workers 1
