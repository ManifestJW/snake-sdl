#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${BUILD_DIR:-build}"
BUILD_TYPE="${BUILD_TYPE:-Debug}"
GENERATOR="${GENERATOR:-Ninja}"

# Prefer VCPKG_ROOT (recommended by vcpkg docs), but also accept VCPKG_TOOLCHAIN_FILE directly.
if [[ -n "${VCPKG_TOOLCHAIN_FILE:-}" ]]; then
  TOOLCHAIN="$VCPKG_TOOLCHAIN_FILE"
elif [[ -n "${VCPKG_ROOT:-}" ]]; then
  TOOLCHAIN="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
else
  cat >&2 <<'EOF'
ERROR: vcpkg not configured.

Set one of:
  - VCPKG_ROOT=/path/to/vcpkg
  - VCPKG_TOOLCHAIN_FILE=/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake

Example:
  export VCPKG_ROOT="$HOME/vcpkg"
EOF
  exit 1
fi

if [[ ! -f "$TOOLCHAIN" ]]; then
  echo "ERROR: vcpkg toolchain file not found: $TOOLCHAIN" >&2
  exit 1
fi

# Optional triplet support
EXTRA=()
if [[ -n "${VCPKG_TARGET_TRIPLET:-}" ]]; then
  EXTRA+=("-DVCPKG_TARGET_TRIPLET=$VCPKG_TARGET_TRIPLET")
fi

cmake -S . -B "$BUILD_DIR" -G "$GENERATOR" \
  -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN" \
  -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
  "${EXTRA[@]}"

cmake --build "$BUILD_DIR" --parallel
echo "Built: ./$BUILD_DIR/snake"
