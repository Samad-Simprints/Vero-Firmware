#!/bin/bash

pushd lib; make install;popd
pushd runtimes; make install; popd
make install
adb push un20setup.sh /data

