#!/bin/sh
set -eu

SOURCE_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
CC=${CC:-cc}

"$CC" -std=gnu11 -O0 -fwrapv -fno-strict-aliasing \
  -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast \
  -Wno-incompatible-pointer-types \
  -o "$SOURCE_DIR/transform" "$SOURCE_DIR/transform.c"

ACTUAL=$("$SOURCE_DIR/transform" "$SOURCE_DIR/transform_const_data.bin" \
  8d0bb7b5268114fe1309be4089f1e972 \
  0327a08dd7456809a2ba342b11a9a236 \
  b4f27cb84eaaff3c1309be4089f02972)
EXPECTED=c75f0df06831e738231dfae48cac7968
if [ "$ACTUAL" != "$EXPECTED" ]; then
  echo "transform self-test failed: $ACTUAL" >&2
  exit 1
fi
echo "transform self-test passed"
