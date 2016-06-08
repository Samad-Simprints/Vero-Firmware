#!/bin/bash

#
# Assume that ftp-root of remote is set to "/data"
#

HOST="10.0.1.3"

echo ""
echo "copying installation files..."
ftp $HOST <<END_FTP
root
binary
put index.tar.gz index.tar.gz
put install-index-un20 install-index-un20
close
bye
END_FTP

echo "***"
echo "Making install script executable "
ssh root@$HOST "cd /data; chmod +x install-index-un20; sync; exit"

#echo "***"
#echo "Making them executable and copying to /usr/lib "
#ssh root@$HOST "cd /data/$TARGET_FOLDER; chmod +x *; ls -al $TARGET; cp /data/$TARGET_FOLDER/* /usr/lib; sync; exit"

echo "Done."
echo ""
 

