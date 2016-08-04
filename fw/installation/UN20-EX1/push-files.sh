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
put ${1}/index.tar.gz index.tar.gz
put ${1}/install-index-un20 install-index-un20
close
bye
END_FTP

echo "Done."
echo ""
