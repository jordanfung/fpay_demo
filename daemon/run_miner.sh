#!/bin/sh

./daemon_server -conf ../conf/miner_server.conf -exec ./node_server -port 9080 -workers 1
