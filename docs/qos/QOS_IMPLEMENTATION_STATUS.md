# QoS Implementation Status

本文档用于区分三件事：

```text
1. 当前 demo 已经能证明什么。
2. 当前源码已经实现到什么程度。
3. 距离完整 DDS/ROS2 QoS 产品级实现还差什么。
```

## Current Position

当前项目已经从旧版 mROS2-ESP32 的固定/简化 QoS 方式，推进到以下状态：

```text
QoSProfile API: 已有
QoSProfile 校验: 已有，非法 depth/duration、未实现 MANUAL liveliness 会被拒绝
部分 QoS SEDP 序列化: 已有，包含 Reliability/Durability/Deadline/Lifespan/Liveliness/History/Resource Limits PID
稳定现场 demo: ESP32->ROS2 RELIABLE；ROS2->ESP32 RELIABLE；双向 reliable 已实机通过
Full RELIABLE reply path: 已进入正式严格测试路径；2026-06-10 preflight 和 3-run reset stress 通过
Deadline/Lifespan/Liveliness/Resource Limits demo 行为验证: 已有
Deadline missed count: reader/writer 状态已改为按 missed period 递增，避免 heartbeat/poll 循环重复乱加
History KEEP_LAST(depth): writer cache 已做深度约束，并由实机日志验证 `5/5 samples`
连续 reset 压力测试: 2026-06-10 严格双向 RELIABLE 实机 3/3 PASS；正式演示路径稳定
Reader receive observability: 已新增 received/accepted-before-match/out-of-order-drop/unmatched-writer-drop 统计，便于区分 discovery、旧 endpoint、序号和 callback 问题
WSL2 mirrored 入站风险: 当前机器最新实测已通过；如果换电脑、换网络或 WSL IP 改变，仍需先跑 network doctor/firewall 脚本再解释 QoS 结果
```

但当前项目还不是完整 DDS QoS 产品级实现。

## Demo-Covered QoS Categories

| Category | Current source support | Current test support | Product-grade gap |
| --- | --- | --- | --- |
| Reliability | Stateful writer path for ESP32 `RELIABLE` uplink; ROS2 reply publisher and ESP32 reply subscriber are also `RELIABLE` in the strict demo | Real hardware TX/RX; ROS2 discovery; strict preflight PASS; 3-run reset stress PASS | Needs broader loss/reorder/fragmentation/interoperability tests before claiming product-grade reliable transport. |
| Durability | QoS field and SEDP representation for `VOLATILE`; partial TRANSIENT_LOCAL writer cache behavior exists | VOLATILE visible in ROS2 topic info | TRANSIENT_LOCAL late-joiner behavior needs formal test coverage and cleanup rules. |
| History | QoS profile carries kind/depth; writer cache enforces `KEEP_LAST(depth)`; SEDP emits PID_HISTORY | Demo prints `History cache: 5/5 samples` and `History KEEP_LAST enforcement PASSED`; ROS2 CLI still reports History depth as UNKNOWN in current tests | Need more unit/stress tests for KEEP_ALL, late ACKNACK, and discovery exposure across DDS vendors. |
| Deadline | Deadline duration carried into reader/writer state; missed count advances by missed period instead of every poll/heartbeat | Focused ESP32 deadline miss test | Need DDS-compatible requested/offered deadline status callbacks and ROS2 event interop. |
| Lifespan | Lifespan duration is represented and writer cache aging logic exists | Focused expired/fresh behavior test; reply endpoint discovery | Need exhaustive cache expiry tests for retransmission, late delivery, and RELIABLE edge cases. |
| Liveliness | AUTOMATIC setting and lease duration represented; reader tracks heartbeat/activity; MANUAL modes are rejected by QoS validation | Focused activity/lease demo | Need liveliness lost/recovered state machine and MANUAL_BY_TOPIC/MANUAL_BY_NODE semantics before enabling manual modes. |
| Resource Limits | max samples/max bytes represented and writer-side local checks exist; SEDP emits standard PID_RESOURCE_LIMITS sample count | Burst rejection behavior and stats | maxBytes is ESP32-local and not represented by DDS PID_RESOURCE_LIMITS; need allocation strategy and failure reporting. |

## Latest Real-Hardware Validation

2026-06-10 当前机器验证结果：

```text
严格双向 RELIABLE 完整预检: PASS
结果目录: /home/your-user/mros2/mros2-esp32/results/qos_preflight_20260610_022457

严格双向 RELIABLE reset 压力复测: 3/3 PASS
结果目录: /home/your-user/mros2/mros2-esp32/results/qos_reset_stress_20260610_022750
summary.txt: runs=3, passed=3, failed=0。
```

这组结果支持“明天的稳定现场演示路径已经实机跑通”。它不等价于完整 DDS QoS 产品级实现完成。

## Latest Engineering Change

本轮新增了 reader 接收路径统计和一个保守的 early-data recovery：

```text
subscriber_received_count()
subscriber_accepted_before_match_count()
subscriber_out_of_order_drop_count()
subscriber_unmatched_writer_drop_count()
```

`StatefulReader` 现在会在以下场景留下可见证据：

```text
1. DATA 在 SEDP endpoint matching 完成前到达。
2. reset 后旧 proxy 暂时存在，但新 writer 的 DATA 已到达。
3. 收到比 expectedSN 更旧的序号并被丢弃。
```

同时，reader callback 从 reader mutex 外调用，降低 callback 内部行为影响 RTPS 接收状态的风险。这个改动提高了排错能力和 reset 后恢复概率。当前严格 demo 已经通过实机 preflight 和 3 次 reset 压力测试，但仍不等价于完整 DDS RELIABLE 产品级认证。

## DDS QoS Beyond Current Demo

Complete DDS QoS is broader than the seven demo categories. Product-level work may also need policies such as:

```text
Presentation
Partition
Ownership
Ownership Strength
Destination Order
Time Based Filter
Transport Priority
User Data / Topic Data / Group Data
Durability Service
Reader Data Lifecycle
Writer Data Lifecycle
Latency Budget
```

These are currently not implemented as product-grade features in this project.

## Product-Grade Definition For This Project

在这个项目里，“产品级 QoS”至少应满足：

```text
1. API: 每个支持的 QoS 有明确 public API、默认值、合法性检查和兼容性检查。
2. Discovery: 需要互操作的 QoS 必须通过 SEDP/PID 正确序列化和反序列化。
3. Behavior: QoS 不只是字段保存，必须影响 writer/reader/cache/匹配行为。
4. Events: Deadline/Liveliness 等状态必须有可查询或回调式事件。
5. Interop: 与 ROS2 Humble/Fast DDS/Cyclone DDS 的典型组合做兼容验证。
6. Stress: reset、late joiner、packet loss、duplicate endpoint、network jitter 都有自动测试。
7. Documentation: 每个 QoS 明确说明 supported/partial/unsupported，不夸大。
```

## Recommended Roadmap

### Phase 1: Stabilize Existing Seven Categories

```text
Reliability: add loss/reorder tests and ACKNACK state tests.
Durability: add TRANSIENT_LOCAL late-joiner demo and tests.
History: enforce KEEP_LAST depth with deterministic tests.
Deadline: expose missed-count/status through mros2 API.
Lifespan: test cache expiry during RELIABLE retransmission.
Liveliness: implement lost/recovered status and lease expiry events.
Resource Limits: define exact behavior on allocation/sample rejection.
```

### Phase 2: Make Discovery Product-Grade

```text
Audit all serialized PID fields.
Add compatibility tests against ROS2 topic info and actual endpoint matching.
Avoid adding SEDP fields that break matching without parser/compatibility coverage.
Add duplicate/stale endpoint handling tests.
```

### Phase 3: Expand Beyond Demo QoS

```text
Decide which DDS QoS policies are in scope for ESP32.
Implement only policies that can be supported within memory/CPU constraints.
Clearly mark unsupported policies as unsupported instead of silently ignoring them.
```

## Recommended Public Wording

Use:

```text
The project currently implements and demonstrates a QoS extension prototype for mROS2-ESP32.
The strict live demo uses RELIABLE in both directions, with real-hardware bidirectional communication and focused evidence for several QoS-related behaviors.
The current evidence supports this demo path, but it is not yet product-grade DDS RELIABLE certification.
It is not yet a complete DDS QoS product implementation.
```

Avoid:

```text
All DDS QoS is fully implemented.
The seven QoS policies are all complete.
ROS2 topic info proves every QoS field.
```
