# QoS Evidence Matrix

本文档只描述当前项目已经能被源码、实机日志或 ROS2 discovery 支撑的事实，不把 demo 结果包装成完整 DDS 产品级能力。

## Demo Scope

```text
ESP32 -> ROS2 topic: /step7_full_qos
ESP32 role: publisher
Configured QoS: RELIABLE, VOLATILE, KEEP_LAST(5), resource limits

ROS2 -> ESP32 topic: /step7_full_qos_reply
ESP32 role: subscriber
ROS2 role: publisher
Configured QoS: RELIABLE, VOLATILE, finite deadline, finite lifespan, AUTOMATIC liveliness
```

## Maturity Levels

```text
L3: Implemented and verified by real hardware behavior plus ROS2 discovery where applicable.
L2: Configured and demonstrated by focused behavior/log evidence, but not complete DDS production behavior.
L1: Represented in the QoS profile or local configuration, with limited demo evidence.
L0: Not implemented or not covered by this demo.
```

## Evidence Matrix

| QoS policy | Current value in demo | Maturity | Evidence | Honest statement |
| --- | --- | --- | --- | --- |
| Reliability | ESP32 uplink `RELIABLE`; ROS2 reply `RELIABLE` | L3 for current live demo path | Bidirectional real-hardware TX/RX; `ros2 topic info --verbose`; strict preflight PASS; 3-run reset stress PASS | Both directions are configured as RELIABLE and exercised on real hardware. |
| Durability | `VOLATILE` | L2 | QoS profile; ROS2 discovery shows `VOLATILE` | VOLATILE is configured and visible. Full TRANSIENT_LOCAL durability behavior is not the focus of the current demo. |
| History | `KEEP_LAST(5)` | L2 | ROS2 CLI exposes the History field but reports depth as UNKNOWN; ESP32 startup log; QoS profile; `History cache: 5/5 samples`; `History KEEP_LAST enforcement PASSED`; SEDP emits PID_HISTORY | Writer-side depth enforcement is implemented and demonstrated. Current ROS2 CLI visibility alone is not enough to prove KEEP_LAST(5). |
| Deadline | ESP32 app-level `100ms`; reply endpoint finite deadline | L2 | ESP32 `Deadline missed: YES`; ROS2 discovery on reply path; deadline counters advance by missed period | The demo verifies deadline detection behavior, but complete DDS deadline status/event interop is not product-grade yet. |
| Lifespan | `2000ms` | L2 | ESP32 expired/fresh checks; ROS2 discovery on reply path | Current evidence is focused behavior testing and endpoint discovery, not exhaustive RTPS cache expiry coverage. |
| Liveliness | `AUTOMATIC`, lease `3000ms` | L2 | ESP32 writer activity/lease log; ROS2 discovery shows `AUTOMATIC` | The demo shows automatic liveliness configuration and observed writer activity. Full liveliness lost/recovered event semantics need more work. |
| Resource Limits | `30 samples`, `12288 bytes` | L2 | ESP32 burst test: rejected count and resource stats; SEDP emits standard sample-count resource limit | Local resource limiting behavior is demonstrated. `maxBytes` is ESP32-local and not a standard ROS2-visible resource-limit field. |

## Bidirectional Communication Evidence

Latest real-hardware evidence on 2026-06-10:

```text
Strict full-RELIABLE preflight PASS:
  /home/your-user/mros2/mros2-esp32/results/qos_preflight_20260610_022457

Strict full-RELIABLE reset stress 3/3 PASS:
  /home/your-user/mros2/mros2-esp32/results/qos_reset_stress_20260610_022750

ROS2 topic info evidence:
  Reliability: ESP32->ROS2 RELIABLE and ROS2->ESP32 RELIABLE visible
  Durability: VOLATILE visible
  History: History field visible; depth is UNKNOWN in current ROS2 Humble output
  Deadline: finite deadline visible
  Lifespan: finite lifespan visible
  Liveliness: AUTOMATIC visible

ESP32 behavior evidence:
  History: KEEP_LAST(5) configured and enforced
  Resource Limits: configured sample/byte limit plus burst rejection behavior
  Deadline/Lifespan/Liveliness: additional focused behavior checks
```

ESP32 serial monitor should show:

```text
publisher matched with remote subscriber
subscriber matched with remote publisher
Warm-up reply confirmed
[ROS2 -> ESP32] Echo reply received
TX: 40 msgs
RX: nonzero; latest preflight example was RX: 40 msgs
Packets Dropped:  0
All phases complete.
```

Interpretation:

```text
TX proves ESP32 -> ROS2.
RX proves ROS2 -> ESP32. RX is not expected to equal TX in the combined demo because KEEP_LAST and resource-limit phases intentionally drop/reject some samples.
Warm-up reply confirmed proves formal statistics start only after the real echo path is alive.
```

The final ESP32 report also prints reader-side receive diagnostics:

```text
Reader received count
Reader accepted-before-match count
Reader out-of-order drop count
Reader unmatched-writer drop count
```

These counters are debugging evidence for discovery/order/reset behavior. They help explain failures, but they are not themselves QoS policy proof.

## Network Preconditions

Before interpreting any QoS result, confirm DDS/RTPS packets can enter WSL:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_network_doctor.sh /dev/ttyUSB0
./scripts/demo/qos_network_doctor.sh /dev/ttyUSB0 20
```

If ESP32 logs show SPDP/SEDP sends but `tcpdump` in WSL sees no UDP traffic on `7400/7401/7410-7420`, the failure is a Windows/WSL network ingress problem, not evidence that a QoS policy failed.

## What Not To Claim

Do not claim:

```text
All DDS QoS policies are fully implemented.
All seven demo QoS categories are production-grade.
Every QoS field is completely visible through ros2 topic info.
History depth and Resource Limits are fully represented through stable SEDP interop.
Manual liveliness and liveliness-lost events are complete.
```

Accurate wording:

```text
This project extends mROS2-ESP32 beyond the old fixed-BEST_EFFORT-style demo by adding a structured QoS profile, SEDP discovery for several endpoint QoS fields, and real-hardware tests for selected policies.
The current strict demo gives strong evidence for bidirectional RELIABLE real-hardware communication and focused evidence for Deadline, Lifespan, Liveliness, History, and Resource Limits.
It is still an engineering prototype, not a complete DDS QoS product implementation.
```
