# mROS2-ESP32 QoS Demo

This local project is organized for a real-hardware teacher demo:

```text
WSL2 ROS2 Humble <-> ESP32-S3 mROS2
```

USB serial is used only for flashing and observing logs. The actual ROS2/mROS2 communication runs over WiFi using DDS/RTPS.

## Demo Entry Point

Use only this workspace for the live demo:

```text
workspace/step7_full_qos
```

Tomorrow's shortest checklist:

```text
docs/demo/TOMORROW_DEMO.md
```

Full runbook:

```text
docs/demo/DEMO_RUNBOOK.md
docs/demo/LIVE_CHEATSHEET.md
```

QoS status and evidence:

```text
docs/qos/QOS_IMPLEMENTATION_STATUS.md
docs/qos/QOS_EVIDENCE_MATRIX.md
```

## Clone And Local Configuration

Clone this repository:

```bash
git clone https://github.com/WANG-Pei777/mROS2-QoS.git
cd mROS2-QoS
```

Create local WiFi credentials before building:

```bash
cp platform/wifi/wifi_secrets.example.h platform/wifi/wifi_secrets.h
```

Then edit `platform/wifi/wifi_secrets.h` for the local SSID and password. This file is ignored by git.

The demo scripts generate the local ROS2/WSL target IP in:

```text
platform/rtps/config_local.h
```

That file is also ignored by git. Run this whenever the WSL IP or network changes:

```bash
./scripts/demo/qos_set_remote_ip.sh
```

## One-Command Readiness Check

From WSL:

```bash
cd /home/your-user/mROS2-QoS
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

Only accept the demo as ready when the output contains:

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

## Windows WSL2 Firewall Setup

When using WSL2 mirrored networking, run this once from an elevated Windows PowerShell:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mROS2-QoS\scripts\demo\qos_wsl_firewall_admin.ps1"
```

This allows DDS/RTPS UDP ports `7400-7420` into WSL.

## Demo Scope

The stable live demo shows:

```text
ESP32 -> ROS2: /step7_full_qos, RELIABLE
ROS2 -> ESP32: /step7_full_qos_reply, RELIABLE
```

The seven QoS-related categories presented are:

```text
Reliability
Durability
History
Deadline
Lifespan
Liveliness
Resource Limits
```

Important boundary:

```text
This is a QoS extension prototype and real-hardware demo, not a complete product-grade DDS QoS implementation.
The strict full-RELIABLE path has passed the current real-hardware preflight and 3-run reset stress test.
```

## Current Project Layout

```text
mros2/                    Core mROS2 and embeddedRTPS source
platform/                 ESP32 WiFi and RTPS platform configuration
workspace/step7_full_qos/ Final real-hardware QoS demo firmware
scripts/demo/             Demo, flashing, preflight, and WSL firewall helpers
scripts/test/             Static QoS validation checks
docs/demo/                Live demo instructions
docs/qos/                 Objective QoS status and evidence matrix
```
