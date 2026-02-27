#!/bin/sh
cd "$(dirname "$0")"
cd ..

NAME=$1
VERSION=$2
LIBSOURCE=$3

if [ ! -e ./utils/setup_env ]; then
    echo "File .env not found, create one from .env.example and fill it with your data."
    exit 1
fi

source ./utils/setup_env

mkdir -p $INSTALL_DIR/$NAME/$VERSION/lib
mkdir -p $INSTALL_DIR/$NAME/$VERSION/include
mkdir -p $INSTALL_DIR/lib
mkdir -p $INSTALL_DIR/include

rsync -aHpPq --exclude-from=utils/install_exclude.txt $LIBSOURCE/ $INSTALL_DIR/$NAME/$VERSION/lib/
rsync -aHpPq --exclude-from=utils/install_exclude.txt include/ $INSTALL_DIR/$NAME/$VERSION/include/

rsync -aHpPq --exclude-from=utils/install_exclude.txt $LIBSOURCE/ $INSTALL_DIR/lib/
rsync -aHpPq --exclude-from=utils/install_exclude.txt include/ $INSTALL_DIR/include/