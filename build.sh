#! /bin/bash

if [ -d "build/playerone" ]; then
  rm -rf build/playerone;
fi

printf "\t=========== Building contracts ===========\n\n"

RED='\033[0;31m'
NC='\033[0m'

CORES=`getconf _NPROCESSORS_ONLN`
mkdir -p build
pushd build &> /dev/null
cmake ../
make -j${CORES}
popd &> /dev/null
