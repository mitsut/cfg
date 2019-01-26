#!/bin/bash

THIS_DIR=$(cd $(dirname $0); pwd)
XERCES_DIR=${THIS_DIR}/depend_libs/xerces-c

cd ${XERCES_DIR}
cmake -G "Unix Makefiles" -DCMAKE_INSTALL_PREFIX=${XERCES_DIR}/build -Dnetwork:BOOL=OFF -DBUILD_SHARED_LIBS:BOOL=OFF
make
make install
ln -s ${XERCES_DIR}/build/lib/libxerces-c-3.2.a ${XERCES_DIR}/build/lib/libxerces-c.a


cd ${THIS_DIR}
./configure \
        --with-libraries=/usr/lib/x86_64-linux-gnu \
        --with-xerces-headers=${XERCES_DIR}/build/include \
        --with-xerces-libraries=${XERCES_DIR}/build/lib \
        --with-xml
make

