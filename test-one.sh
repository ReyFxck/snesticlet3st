#!/bin/bash

SRC="$1"
EXT="${SRC##*.}"
BASE="$(basename "${SRC%.*}")"
OBJ="build/${BASE}.o"
LOG="logs/${BASE}.log"

mkdir -p build logs

INCS="-I/root/SNESticle-Beta/Gep/Include/common \
-I/root/SNESticle-Beta/Gep/Include/ps2 \
-I/root/SNESticle-Beta/Gep/Source/common \
-I/root/SNESticle-Beta/Gep/Source/common/zlib \
-I/root/SNESticle-Beta/Gep/Source/common/unzip \
-I/root/SNESticle-Beta/SNESticle/Source/common \
-I/root/SNESticle-Beta/SNESticle/Source/ps2 \
-I/root/SNESticle-Beta/SNESticle/Modules/mcsave/ee \
-I/root/SNESticle-Beta/SNESticle/Modules/sjpcm/ee \
-I/root/SNESticle-Beta/SNESticle/Modules/netplay/Source/common \
-I/root/SNESticle-Beta/SNESticle/Modules/netplay/Source/ps2/common \
-I/root/SNESticle-Beta/SNESticle/Modules/netplay/Source/ps2/ee \
-I/root/SNESticle-Beta/SNESticle/Modules/libcdvd/common \
-I/root/SNESticle-Beta/SNESticle/Modules/libcdvd/ee \
-I/opt/ps2dev/ps2sdk/common/include \
-I/opt/ps2dev/ps2sdk/ee/include \
-I/root/snesticle-test/compat \
-I/opt/ps2dev/ps2sdk/ports/include"

echo "==> Compilando $SRC"

if [ "$EXT" = "c" ]; then
    COMPILER="ee-gcc"
    FLAGS="-G0 -O2 -Wall -D_EE -DPS2 -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3"
else
    COMPILER="ee-g++"
    FLAGS="-G0 -O2 -Wall -fno-exceptions -fno-rtti -D_EE -DPS2 -DLSB_FIRST -DALIGN_DWORD -DCODE_PLATFORM=3"
fi

if $COMPILER $FLAGS $INCS -c "$SRC" -o "$OBJ" > "$LOG" 2>&1
then
    echo "OK: $SRC"
else
    echo "ERRO: $SRC"
    tail -n 30 "$LOG"
fi
