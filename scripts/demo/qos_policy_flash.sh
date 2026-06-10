#!/usr/bin/env bash
set -euo pipefail

POLICY="${1:-all}"
PORT="${2:-/dev/ttyUSB0}"
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
WORKSPACE="step7_full_qos"

usage() {
  cat <<'EOF'
Usage:
  ./scripts/demo/qos_policy_flash.sh [all] [port]

This project keeps one live hardware demo:
  all    WSL2 ROS2 <-> ESP32 mROS2 bidirectional QoS demo

Examples:
  QOS_DEMO_MONITOR=0 ./scripts/demo/qos_policy_flash.sh all /dev/ttyUSB0
  ./scripts/demo/qos_policy_flash.sh all /dev/ttyUSB0
EOF
}

case "${POLICY}" in
  all) ;;
  -h|--help) usage; exit 0 ;;
  *)
    echo "Unknown policy: ${POLICY}"
    usage
    exit 1
    ;;
esac

set +u
source "${HOME}/esp-idf/export.sh"
set -u

cd "${PROJECT_ROOT}/workspace/${WORKSPACE}"
echo "[qos-demo] policy=all"
echo "[qos-demo] reply_path=RELIABLE"
echo "[qos-demo] workspace=${WORKSPACE}"
echo "[qos-demo] port=${PORT}"
if [ "${QOS_DEMO_AUTO_REMOTE_IP:-1}" = "1" ]; then
  if [ -n "${QOS_DEMO_REMOTE_IP:-}" ]; then
    "${PROJECT_ROOT}/scripts/demo/qos_set_remote_ip.sh" "${QOS_DEMO_REMOTE_IP}"
  else
    "${PROJECT_ROOT}/scripts/demo/qos_set_remote_ip.sh"
  fi
else
  echo "[qos-demo] auto remote IP update disabled"
fi
if [ -f build/CMakeCache.txt ] && ! grep -q "${PROJECT_ROOT}/workspace/${WORKSPACE}" build/CMakeCache.txt; then
  echo "[qos-demo] removing stale build directory with old source path"
  rm -rf build
fi

MROS2_QOS_FULL_RELIABLE_REPLY=1 idf.py reconfigure
MROS2_QOS_FULL_RELIABLE_REPLY=1 idf.py build
if [ "${QOS_DEMO_MONITOR:-1}" = "0" ]; then
  MROS2_QOS_FULL_RELIABLE_REPLY=1 idf.py -p "${PORT}" flash
else
  MROS2_QOS_FULL_RELIABLE_REPLY=1 idf.py -p "${PORT}" flash monitor
fi
