#!/bin/bash
#
# build.sh
#
# スタティックリンクの実行ファイルビルド用スクリプト
#
# 動作条件
#     Ubuntu 18.04 での実行を想定しています。
#     Ubuntu 16.04 で実行する場合には、環境変数 CPPFLAGS="-std=c++11" を指定してください。
#
# 事前に導入が必要な必要パッケージ
#     - make
#     - wget
#     - tar
#     - bzip2
#     - g++
#     - libboost-dev
#     - libboost-regex-dev
#     - libboost-system-dev
#     - libboost-filesystem-dev
#     - libboost-program-options-dev     - 

THIS_DIR=$(cd $(dirname $0); pwd)
DEPEND_LIBS_DIR=${THIS_DIR}/depend_libs
XERCES_DIR=${DEPEND_LIBS_DIR}/xerces-c-3.2.2
XERCES_URL=http://archive.apache.org/dist/xerces/c/3/sources
XERCES_TB_NAME=xerces-c-3.2.2.tar.bz2
LIBS_INSTALL_PATH=${DEPEND_LIBS_DIR}/opt/local

# 依存ライブラリ展開用ディレクトリ作成
mkdir -p ${DEPEND_LIBS_DIR}

# xerces の準備
if [ ! -e ${DEPEND_LIBS_DIR}/${XERCES_TB_NAME} ]; then
    wget ${XERCES_URL}/${XERCES_TB_NAME} -O ${DEPEND_LIBS_DIR}/${XERCES_TB_NAME}
fi

cd ${DEPEND_LIBS_DIR}
if [ ! -e ${XERCES_DIR} ]; then
    # xerces を展開
    tar xfv ${XERCES_TB_NAME}
fi

cd ${XERCES_DIR}
./configure --prefix=${LIBS_INSTALL_PATH} --disable-network --disable-shared
make
make install

cd ${THIS_DIR}
CONFIGURE_OPTIONS="--with-xml --with-xerces-headers=${LIBS_INSTALL_PATH}/include --with-xerces-libraries=${LIBS_INSTALL_PATH}/lib"

# OS が Linux であれば、スタティックリンクを行う
if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    CONFIGURE_OPTIONS+=" --options=-static"
fi

./configure ${CONFIGURE_OPTIONS}
make

