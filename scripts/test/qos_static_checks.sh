#!/usr/bin/env bash
set -euo pipefail

PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build/qos_static_checks"
SRC="${BUILD_DIR}/qos_static_checks.cpp"
BIN="${BUILD_DIR}/qos_static_checks"
STUB_DIR="${BUILD_DIR}/stubs"

mkdir -p "${BUILD_DIR}"
mkdir -p "${STUB_DIR}/lwip"

cat > "${STUB_DIR}/lwip/ip4_addr.h" <<'H'
#ifndef QOS_STATIC_CHECKS_LWIP_IP4_ADDR_H
#define QOS_STATIC_CHECKS_LWIP_IP4_ADDR_H
#include <stdint.h>
typedef struct ip4_addr {
  uint32_t addr;
} ip4_addr_t;
#endif
H

cat > "${SRC}" <<'CPP'
#include "mros2/qos.h"
#include <iostream>

static int failures = 0;

static void expect(bool condition, const char *label) {
  if (!condition) {
    std::cerr << "[FAIL] " << label << "\n";
    failures++;
  } else {
    std::cout << "[PASS] " << label << "\n";
  }
}

int main() {
  using namespace mros2;

  QoSProfile valid = QoSProfile::reliable();
  expect(QoSPolicy::validate(valid), "reliable profile is valid");

  QoSProfile zero_depth = valid;
  zero_depth.depth = 0;
  expect(!QoSPolicy::validate(zero_depth), "KEEP_LAST depth=0 is rejected");

  QoSProfile huge_depth = valid;
  huge_depth.depth = 101;
  expect(!QoSPolicy::validate(huge_depth), "oversized depth is rejected");

  QoSProfile manual_liveliness = valid;
  manual_liveliness.liveliness = LivelinessKind::MANUAL_BY_TOPIC;
  expect(!QoSPolicy::validate(manual_liveliness),
         "unsupported manual liveliness is rejected");

  QoSProfile invalid_duration = valid;
  invalid_duration.deadline = Duration{0, 1000000000U};
  expect(!QoSPolicy::validate(invalid_duration),
         "invalid nanosecond duration is rejected");

  QoSProfile sample_limit = valid;
  sample_limit.depth = 5;
  sample_limit.max_samples = 4;
  expect(!QoSPolicy::validate(sample_limit),
         "max_samples smaller than KEEP_LAST depth is rejected");

  QoSProfile keep_all = valid;
  keep_all.history = HistoryKind::KEEP_ALL;
  keep_all.depth = 0;
  keep_all.max_samples = 1;
  expect(QoSPolicy::validate(keep_all), "KEEP_ALL does not require depth");

  return failures == 0 ? 0 : 1;
}
CPP

g++ -std=c++17 \
  -I"${STUB_DIR}" \
  -I"${PROJECT_ROOT}/mros2/include" \
  -I"${PROJECT_ROOT}/mros2/embeddedRTPS/include" \
  "${SRC}" -o "${BIN}"

"${BIN}"
