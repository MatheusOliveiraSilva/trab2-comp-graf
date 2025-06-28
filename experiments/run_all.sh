#!/usr/bin/env bash
set -e
cmake -S .. -B ../build -DCMAKE_BUILD_TYPE=Release

# Detect número de núcleos (compatível com macOS)
if command -v nproc >/dev/null 2>&1; then
  JOBS=$(nproc)
else
  JOBS=$(sysctl -n hw.ncpu)
fi

cmake --build ../build -j${JOBS}
mkdir -p ../experiments

# 1) Ruído vs spp
for spp in 50 200 800; do
  ../build/path_tracer --samples $spp
  mv output.ppm "noise_${spp}spp.ppm"
  if command -v convert >/dev/null 2>&1; then
    convert "noise_${spp}spp.ppm" "noise_${spp}spp.png"
  fi
done

# 2) MIS on/off
../build/path_tracer --samples 200 --mis_off && mv output.ppm mis_off_200.ppm
if command -v convert >/dev/null 2>&1; then convert mis_off_200.ppm mis_off_200.png; fi
../build/path_tracer --samples 200           && mv output.ppm mis_on_200.ppm
if command -v convert >/dev/null 2>&1; then convert mis_on_200.ppm mis_on_200.png; fi

# 3) Min depth
../build/path_tracer --samples 200 --min_depth 1 && mv output.ppm min1_200.ppm
if command -v convert >/dev/null 2>&1; then convert min1_200.ppm min1_200.png; fi
../build/path_tracer --samples 200 --min_depth 4 && mv output.ppm min4_200.ppm
if command -v convert >/dev/null 2>&1; then convert min4_200.ppm min4_200.png; fi

echo "Experimentos concluídos. Arquivos gerados em experiments/" 