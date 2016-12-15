#!/bin/bash

#
# Assume that ftp-root of remote is set to "/data"
#

HOST="10.0.1.3"

echo ""
echo "clearing files from un20..."
ssh root@$HOST "rm /data/*; rm -r /data/lib; rm -r /data/runtimes; rm -r /data/un20-lib;  sync; exit"

echo "Done."
echo ""
 

