# mROS2-ESP32 Live Teacher Demo Runbook

鏈枃妗ｇ敤浜庡湪鍙︿竴鍙?Windows + WSL2 鐢佃剳涓婂鍒荤幇鍦哄疄鏈烘紨绀恒€?
婕旂ず鐩爣锛?
- ROS2 Humble 杩愯鍦?WSL2銆?- ESP32-S3 杩愯 mROS2 鍥轰欢銆?- ESP32 涓?WSL2 ROS2 閫氳繃 WiFi 鍋?DDS/RTPS 閫氫俊銆?- 涓插彛鍙敤浜庤瀵?ESP32 鏃ュ織锛屼笉鏄€氫俊閾捐矾銆?- 瀹炴満楠岃瘉鍙屽悜浜掗€氾細ESP32 鍙戞秷鎭粰 ROS2锛孯OS2 鍐嶅洖鍖呯粰 ESP32銆?- 灞曠ず鏈湴椤圭洰瀵?mROS2-ESP32 QoS 鑳藉姏鐨勬墿灞曞師鍨嬶紱姝ｅ紡鐜板満璺緞鏄?ESP32->ROS2 RELIABLE锛孯OS2->ESP32 RELIABLE銆?- 鏄庣‘璇存槑锛氬綋鍓嶄笉鏄畬鏁?DDS QoS 浜у搧绾у疄鐜般€?
鏈€缁堢幇鍦?demo 鍙繚鐣欎竴濂楋細`workspace/step7_full_qos`銆?
## 0. 鐜板満绐楀彛鍒嗗伐

鐜板満寮€涓変釜绐楀彛锛屽崈涓囦笉瑕佹贩鐢細

```text
绐楀彛 1: Windows PowerShell
鐢ㄩ€? 鍙仛 usbipd锛屾妸 ESP32 USB 涓插彛鎸傜粰 WSL2銆?
绐楀彛 2: WSL Terminal A
鐢ㄩ€? 鍚姩 ROS2 host echo reply銆?
绐楀彛 3: WSL Terminal B
鐢ㄩ€? 鍚姩 ESP32 serial monitor锛岀湅瀹炴満鏃ュ織銆?
鍙€夌獥鍙?4: WSL Terminal C
鐢ㄩ€? ros2 topic info --verbose锛屽睍绀?QoS discovery 璇佹嵁銆?```

## 0.1 瀹為獙瀹ゅ彟涓€鍙扮數鑴戜竴灞忓懡浠?
涓嬮潰杩欏潡鏄粠闆跺埌鍙睍绀虹殑鏈€鐭畬鏁村懡浠ゃ€傚懡浠ゆ墍鍦ㄧ獥鍙ｄ笉瑕佹贩鐢ㄣ€?
### 绠＄悊鍛?Windows PowerShell

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mros2\mros2-esp32\scripts\demo\qos_wsl_firewall_admin.ps1"
```

### 鏅€?Windows PowerShell

```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
```

鎵惧埌 `CP2102N USB to UART Bridge Controller` 鐨?BUSID銆備笅闈㈢敤 `X-Y` 浠ｈ〃鐪熷疄 BUSID銆?
濡傛灉 CP2102N 鏄?`Not shared`锛屽洖鍒扮鐞嗗憳 Windows PowerShell 鎵ц锛?
```powershell
usbipd bind --busid X-Y
```

濡傛灉 CP2102N 涓嶆槸 `Attached`锛屾櫘閫?Windows PowerShell 鎵ц锛?
```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
usbipd list
```

### WSL

```bash
cd /home/your-user/mros2/mros2-esp32
ls -l /dev/ttyUSB0
test -f /opt/ros/humble/setup.bash && echo "ROS2 OK" || echo "ROS2 missing"
test -f ~/esp-idf/export.sh && echo "ESP-IDF OK" || echo "ESP-IDF missing"
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

鍙帴鍙楋細

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

鍙€夊帇鍔涙祴璇曪細

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_reset_stress.sh 3 /dev/ttyUSB0 140
```

鍙帴鍙楋細

```text
[stress] RESULT: PASS
```

### 姝ｅ紡婕旂ず绐楀彛

绐楀彛 1锛學indows PowerShell锛?
```powershell
usbipd list
```

绐楀彛 2锛學SL Terminal A锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_policy_host.sh all
```

绐楀彛 3锛學SL Terminal B锛?
```bash
cd /home/your-user/mros2/mros2-esp32/workspace/step7_full_qos
source ~/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 monitor
```

鐭寜 ESP32 鐨?`RST`銆乣RESET` 鎴?`EN`銆?
绐楀彛 4锛屽彲閫夛紝WSL Terminal C锛?
```bash
source /opt/ros/humble/setup.bash
ros2 topic info /step7_full_qos --verbose
ros2 topic info /step7_full_qos_reply --verbose
```

## 1. 鍙︿竴鍙扮數鑴戝噯澶?
### 1.1 Windows PowerShell: 妫€鏌?WSL2

鍦?Windows PowerShell 杩愯锛?
```powershell
wsl -l -v
```

瑕佹眰鐪嬪埌 Ubuntu 鏄?WSL2锛?
```text
Ubuntu-22.04    Running    2
```

濡傛灉 Ubuntu 娌″惎鍔紝涓嶈鍏堣繘鍏ヤ氦浜掑紡 WSL 鍐嶆墽琛?`usbipd`銆傚湪 Windows PowerShell 閲岃繍琛屼竴涓悗鍙颁繚娲昏繘绋嬶細

```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
```

### 1.2 Windows PowerShell: 妫€鏌?usbipd

鍦?Windows PowerShell 杩愯锛?
```powershell
usbipd list
```

瑕佹眰鐪嬪埌绫讳技璁惧锛?
```text
CP2102N USB to UART Bridge Controller
```

濡傛灉娌℃湁 `usbipd` 鍛戒护锛屽厛瀹夎 usbipd-win锛屽畨瑁呭悗閲嶆柊鎵撳紑 PowerShell銆?
### 1.3 Windows PowerShell: 鎶?ESP32 USB 鎸傜粰 WSL2

涓嶈鐓ф妱鏈満鐨?`2-4`銆傚彟涓€鍙扮數鑴戠殑 BUSID 鍙兘涓嶅悓銆?
鍏堣繍琛岋細

```powershell
usbipd list
```

鎵惧埌 `CP2102N USB to UART Bridge Controller` 鎵€鍦ㄨ锛岃涓?BUSID銆備笅闈㈢敤 `X-Y` 浠ｈ〃鐪熷疄 BUSID銆?
绗竴娆′娇鐢ㄨ繖涓澶囨椂杩愯锛?
```powershell
usbipd bind --busid X-Y
```

鐒跺悗鎸傝浇缁?WSL2锛?
```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
usbipd list
```

瑕佹眰璁惧鐘舵€佸彉鎴愶細

```text
Attached
```

濡傛灉宸茬粡鏄?`Shared`锛屽彲浠ョ洿鎺ユ墽琛?`attach`銆?
### 1.4 WSL: 妫€鏌ヤ覆鍙ｃ€丷OS2銆丒SP-IDF

鍦?WSL 杩愯锛?
```bash
ls -l /dev/ttyUSB0
test -f /opt/ros/humble/setup.bash && echo "ROS2 OK" || echo "ROS2 missing"
test -f ~/esp-idf/export.sh && echo "ESP-IDF OK" || echo "ESP-IDF missing"
```

蹇呴』鐪嬪埌锛?
```text
/dev/ttyUSB0
ROS2 OK
ESP-IDF OK
```

濡傛灉娌℃湁 `/dev/ttyUSB0`锛屽洖鍒?PowerShell锛?
```powershell
usbipd list
usbipd detach --busid X-Y
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
```

鐒跺悗鍥?WSL 鍐嶆鏌ワ細

```bash
ls -l /dev/ttyUSB0
```

## 2. 鍙︿竴鍙扮數鑴戦」鐩斁缃?
鎺ㄨ崘椤圭洰璺緞鍥哄畾涓猴細

```bash
/home/your-user/mros2/mros2-esp32
```

鍦?WSL 妫€鏌ワ細

```bash
cd /home/your-user/mros2/mros2-esp32
ls workspace/step7_full_qos/main/app.cpp
ls workspace/step7_full_qos/echo_reply.py
ls scripts/demo/qos_policy_host.sh
ls scripts/demo/qos_policy_flash.sh
ls docs/demo/DEMO_RUNBOOK.md
```

濡傛灉杩欎簺鏂囦欢閮藉瓨鍦紝椤圭洰浣嶇疆姝ｇ‘銆?
## 3. 鍙︿竴鍙扮數鑴?WiFi 妫€鏌?
ESP32 鍜?WSL2 ROS2 鎵€鍦ㄧ數鑴戝繀椤诲湪鍚屼竴涓眬鍩熺綉銆傛鏌?ESP32 WiFi 閰嶇疆锛?
```bash
cd /home/your-user/mros2/mros2-esp32
grep -n "ssid\\|password\\|SSID\\|PASSWORD" platform/wifi/wifi.h
```

濡傛灉鐜板満 WiFi 鍜屾湰鏈轰笉鍚岋紝鍏堜慨鏀?`platform/wifi/wifi.h`锛屽啀鐑у綍銆?
### 3.1 WSL2 mirrored mode 鍏ョ珯妫€鏌?
濡傛灉 Windows 鐨?`.wslconfig` 浣跨敤锛?
```text
[wsl2]
networkingMode=mirrored
```

ESP32 鍙戝埌 Windows/WSL 鍏变韩 IP 鐨?DDS/RTPS UDP 鍖呭彲鑳借 Hyper-V firewall 鎷﹀湪 WSL 澶栭潰銆傚吀鍨嬬幇璞★細

```text
ESP32 涓插彛鏄剧ず宸茬粡鍙戦€?SPDP/SEDP銆?WSL 閲?ros2 topic info 鐪嬩笉鍒?_CREATED_BY_BARE_DDS_APP_銆?sudo tcpdump -ni <iface> 'udp and (port 7400 or port 7401 or portrange 7410-7420)' 鎶撲笉鍒?ESP32 鍖呫€?```

鍏堝湪 WSL 璺戣瘖鏂細

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_network_doctor.sh /dev/ttyUSB0
```

濡傛灉闇€瑕佹姄 DDS/RTPS 鍖咃細

```bash
./scripts/demo/qos_network_doctor.sh /dev/ttyUSB0 20
```

濡傛灉纭鍖呰繘涓嶄簡 WSL锛岄渶瑕佺敤鈥滅鐞嗗憳 Windows PowerShell鈥濇坊鍔?WSL Hyper-V 鍏ョ珯瑙勫垯銆傛帹鑽愮洿鎺ヨ繍琛岄」鐩噷鐨勮剼鏈細

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mros2\mros2-esp32\scripts\demo\qos_wsl_firewall_admin.ps1"
```

绛変环鐨勬墜鍔ㄥ懡浠ゆ槸锛?
```powershell
$id = '{40E0AC32-46A5-438A-A0B2-2B479E8F2E90}'
New-NetFirewallHyperVRule `
  -Name 'mros2-dds-rtps-wsl-udp-7400-7420' `
  -DisplayName 'mROS2 DDS RTPS WSL UDP 7400-7420' `
  -Direction Inbound `
  -VMCreatorId $id `
  -Protocol UDP `
  -LocalPorts 7400-7420 `
  -Action Allow `
  -Enabled True
```

褰撳墠鏅€?PowerShell/闈炵鐞嗗憳鐜鏃犳硶鍒涘缓杩欐潯瑙勫垯锛屼細鎶?`Access is denied`銆傝繖涓嶆槸 mROS2 QoS 婧愮爜闂銆?
## 4. 鍙︿竴鍙扮數鑴戠儳褰曞浐浠?
鍙湪鍑嗗闃舵鐑у綍锛屼笉寤鸿姝ｅ紡灞曠ず鏃跺綋鍦虹儳褰曘€?
鍦?WSL 杩愯锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 flash
```

鐑у綍鎴愬姛鐨勫叧閿粨灏撅細

```text
Leaving...
Hard resetting via RTS pin...
Done
```

濡傛灉 `idf.py` 鎻愮ず鎵句笉鍒帮紝涓€鑸槸 ESP-IDF 鐜娌″姞杞姐€傝剼鏈細鑷姩 `source ~/esp-idf/export.sh`锛屼絾濡傛灉浠嶇劧澶辫触锛屽厛鎵嬪姩杩愯锛?
```bash
source ~/esp-idf/export.sh
```

鍐嶉噸鏂版墽琛岀儳褰曞懡浠ゃ€?
## 5. 鍙︿竴鍙扮數鑴戦婕?Smoke Test

鐑у綍鍚庡厛鍋氫竴娆″畬鏁撮婕旓紝纭瀹炴満鑳借窇銆?
鎺ㄨ崘鍏堣繍琛岃嚜鍔ㄥ娴嬭剼鏈€傚畠浼氳嚜鍔ㄦ洿鏂?`REMOTE_PARTICIPANT_IP` 涓哄綋鍓?WSL IPv4銆佹瀯寤恒€佺儳褰曞苟璺?3 娆′互鍐呯殑瀹炴満棰勬锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

濡傛灉鏈€鍚庣湅鍒帮細

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

璇存槑浜掗€氬拰鍙睍绀?QoS 璇佹嵁宸茬粡閫氳繃銆傝繖涓剼鏈細鍦ㄩ獙璇佸墠鍒锋柊 ROS2 daemon锛屽噺灏?discovery 缂撳瓨鎴栨椂搴忓鑷寸殑鍋跺彂涓嶅尮閰嶃€備笅闈㈢殑 Terminal A/B/C 姝ラ鐢ㄤ簬鎵嬪姩澶嶇幇姝ｅ紡鐜板満娴佺▼銆?
濡傛灉杩樻湁鏃堕棿锛屽彲浠ヨ拷鍔?reset 鍘嬪姏棰勬锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_reset_stress.sh 3 /dev/ttyUSB0
```

鐪嬪埌 `[stress] RESULT: PASS` 鍚庡啀姝ｅ紡婕旂ず銆?
### 5.1 WSL Terminal A: 鍚姩 ROS2 host

```bash
cd /home/your-user/mros2/mros2-esp32
pkill -f 'workspace/step7_full_qos/echo_reply.py' || true
./scripts/demo/qos_policy_host.sh all
```

淇濇寔杩欎釜绐楀彛杩愯銆?
棰勬湡鐪嬪埌锛?
```text
Echo reply node started, listening on /step7_full_qos
```

### 5.2 WSL Terminal B: 鍚姩 ESP32 monitor

```bash
cd /home/your-user/mros2/mros2-esp32/workspace/step7_full_qos
source ~/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 monitor
```

鐒跺悗鐭寜 ESP32 鏉垮瓙鐨?`RST`銆乣RESET` 鎴?`EN` 鎸夐敭銆備笉瑕佹寜 `BOOT`锛屼篃涓嶉渶瑕侀暱鎸夈€?
棰勬湡鐪嬪埌锛?
```text
Reliability: RELIABLE uplink, RELIABLE reply path
Durability : VOLATILE
History    : KEEP_LAST(5)
Deadline   : 100ms
Lifespan   : 2000ms
Liveliness : AUTOMATIC (lease=3000ms)
Resources  : 30 samples, 12288 bytes
publisher matched with remote subscriber
subscriber matched with remote publisher
Warm-up reply confirmed
[ROS2 -> ESP32] Echo reply received
Lifespan check PASSED: expired message correctly identified
Lifespan check PASSED: fresh message accepted
Liveliness lease check PASSED
Liveliness finite lease behavior PASSED
Rejected during burst
TX: 40 msgs
RX: nonzero; final verification example was RX: 27 msgs
Latency (round-trip)
Packets Dropped:  0
All phases complete.
```

璇存槑锛?
```text
TX 鏄?ESP32 鍙戠粰 ROS2 鐨勬秷鎭暟銆?RX 鏄?ESP32 鏀跺埌 ROS2 echo reply 鐨勬秷鎭暟銆?姝ｅ紡棰勬鑴氭湰瑕佹眰 RX 鑷冲皯杈惧埌闃堝€笺€傜患鍚?demo 鍚屾椂鍖呭惈 KEEP_LAST 鍜?Resource Limits 娴嬭瘯锛屾墍浠?RX 涓嶈姹傜瓑浜?TX锛涘彧瑕?warm-up confirmed銆丷X 杈炬爣銆丳ackets Dropped 涓?0锛屽苟涓旀渶缁?PASS 鍗冲彲銆?濡傛灉鐪嬪埌 `DEMO NOT READY`锛岃鏄?warm-up 鍥炲寘娌℃湁纭锛岀洿鎺ュ仠姝㈡紨绀哄苟閲嶆柊璺?preflight銆?```

濡傛灉鐪嬪埌 RX 鏄庢樉缈诲€嶏紝璇存槑鍙兘寮€浜嗕袱涓?ROS2 host锛屼笉鏄€氫俊澶辫触銆傛墽琛岋細

```bash
pkill -f 'workspace/step7_full_qos/echo_reply.py' || true
```

鐒跺悗閲嶆柊鍚姩 Terminal A 鍜?Terminal B銆?
## 6. 姝ｅ紡鐜板満婕旂ず姝ラ

### Step 1: 璇存槑杩欐槸鐪熷疄纭欢閾捐矾

瀵硅€佸笀璇达細

```text
杩欎笉鏄豢鐪熴€俁OS2 鍦?WSL2 閲岃繍琛岋紝ESP32-S3 璺?mROS2 鍥轰欢銆?涓よ竟閫氳繃 WiFi 鍋?DDS/RTPS 閫氫俊锛孶SB 涓插彛鍙敤鏉ョ湅 ESP32 鏃ュ織銆?```

### Step 2: PowerShell 纭 ESP32 宸叉寕缁?WSL2

Windows PowerShell锛?
```powershell
usbipd list
```

鐪嬪埌 CP2102N 鏄?`Attached` 鍗冲彲銆?
濡傛灉涓嶆槸 `Attached`锛?
```powershell
usbipd list
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
```

`X-Y` 蹇呴』鎹㈡垚 `usbipd list` 閲屽綋鍓嶇殑 BUSID銆?
`usbipd` 鏄?Windows PowerShell 鍛戒护銆傚鏋滀綘鐪嬪埌鎻愮ず绗﹀儚 `your-user@...$`锛岃鏄庡凡缁忓湪 WSL 閲屼簡锛岃繖鏃惰鍥炲埌 Windows PowerShell 鍐嶆墽琛?`usbipd`銆?
### Step 3: WSL Terminal A 鍚姩 ROS2 host

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_policy_host.sh all
```

鐪嬪埌涓嬮潰鍐呭鍚庝繚鎸佺獥鍙ｄ笉鍔細

```text
Echo reply node started, listening on /step7_full_qos
```

### Step 4: WSL Terminal B 鍚姩 ESP32 monitor

```bash
cd /home/your-user/mros2/mros2-esp32/workspace/step7_full_qos
source ~/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 monitor
```

鐭寜 ESP32 鐨?`RST`銆乣RESET` 鎴?`EN`銆?
### Step 5: 鎸囧嚭鍙屽悜浜掗€氳瘉鎹?
ESP32 monitor 涓繖浜涜璇佹槑鍙屽悜浜掗€氾細

```text
publisher matched with remote subscriber
subscriber matched with remote publisher
[ROS2 -> ESP32] Echo reply received
Warm-up reply confirmed
Liveliness lease check PASSED
TX: 40 msgs
RX: nonzero; typical value is around 20-30 msgs
Packets Dropped:  0
```

瑙ｉ噴锛?
```text
TX 鏄?ESP32 鍙戝竷 /step7_full_qos锛孯OS2 鏀跺埌銆?RX 鏄?ROS2 鍙戝竷 /step7_full_qos_reply锛孍SP32 鏀跺埌銆?鎵€浠?ESP32 鏃㈡槸 publisher锛屼篃鏄?subscriber锛汻OS2 涔熸棦鏄?subscriber锛屼篃鏄?publisher銆?```

### Step 6: WSL Terminal C 灞曠ず QoS discovery 璇佹嵁

鏂板紑涓€涓?WSL 绐楀彛杩愯锛?
```bash
source /opt/ros/humble/setup.bash
ros2 topic info /step7_full_qos --verbose
ros2 topic info /step7_full_qos_reply --verbose
```

閲嶇偣灞曠ず锛?
```text
/step7_full_qos
Node name: _CREATED_BY_BARE_DDS_APP_
Reliability: RELIABLE
History (Depth): UNKNOWN
Durability: VOLATILE

/step7_full_qos_reply
Reliability: RELIABLE
History (Depth): UNKNOWN
Deadline: 23283064 nanoseconds
Lifespan: 2000000000 nanoseconds
Liveliness: AUTOMATIC
Node name: _CREATED_BY_BARE_DDS_APP_
```

瑙ｉ噴锛?
```text
ros2 topic info --verbose 鏄?ROS2 discovery 灞傜湅鍒扮殑 endpoint QoS銆?杩欎笉鏄崟绾湪 ESP32 浠ｇ爜閲屾墦鍗板瓧绗︿覆銆?_CREATED_BY_BARE_DDS_APP_ 灏辨槸 ESP32/mROS2 杩欎竴绔殑 DDS endpoint銆?褰撳墠 demo 宸叉妸 ESP32 RELIABLE writer 鏀逛负鍖归厤鍚庡己鍒跺崟鎾彂閫侊紝鐜板満搴旇兘鍦?/step7_full_qos 鐪嬪埌 ESP32 publisher 鐨?RELIABLE銆?鍙屽悜浜掗€氫互 ESP32 monitor 鐨?RX 闈為浂鍜?ROS2 host 鐨?echo reply 璁℃暟涓烘渶缁堣瘉鎹€?```

## 7. QoS 灞曠ず鍙ｅ緞

鐜板満涓嶈璇粹€滀竷绉?QoS 鍏ㄩ儴閮借兘鍦?ros2 topic info 閲屽畬鏁存樉绀衡€濓紝涔熶笉瑕佽鈥滃畬鏁?DDS QoS 宸茬粡浜у搧绾у疄鐜扳€濄€傛洿鍑嗙‘鐨勮娉曟槸锛?
```text
杩欎釜 demo 瑕嗙洊浜嗕竷绫?QoS 鐩稿叧閰嶇疆鍜屾祴璇曠偣锛?Reliability銆丏urability銆丠istory銆丏eadline銆丩ifespan銆丩iveliness銆丷esource Limits銆?
鍏朵腑 Reliability 鏄弻鍚?RELIABLE锛欵SP32->ROS2 鍜?ROS2->ESP32 閮藉凡缁忓仛浜嗗疄鏈洪獙璇併€?ROS2 host 鐨?RELIABLE subscription銆佸洖鍖呮柟鍚戠殑 RELIABLE銆丏eadline銆丩ifespan 鑳借 ros2 topic info --verbose 鐪嬪埌銆?ROS2 Humble 浼氭樉绀?History 瀛楁锛屼絾褰撳墠杈撳嚭鏄?`History (Depth): UNKNOWN`锛屾墍浠ヤ笉瑕佽 topic info 鐩存帴璇佹槑浜?KEEP_LAST(5)銆?ESP32/mROS2 鐨?RELIABLE publisher 浼氫互 _CREATED_BY_BARE_DDS_APP_ 鏄剧ず鍦?/step7_full_qos銆?ESP32/mROS2 鐨勫洖鍖?subscriber 浼氫互 _CREATED_BY_BARE_DDS_APP_ 鏄剧ず鍦?/step7_full_qos_reply銆?History KEEP_LAST(5) 鍜?Resource Limits 灞炰簬鏈湴 endpoint/cache 璧勬簮琛屼负锛孍SP32 鍥轰欢鏃ュ織浼氭墦鍗?`History KEEP_LAST enforcement PASSED`銆乣Rejected during burst` 鍜?`Resource stats`銆?Liveliness 浣跨敤 AUTOMATIC + lease 閰嶇疆锛孍SP32 鍥轰欢鏃ュ織灞曠ず ROS2 writer activity within lease 鍜?finite lease 琛屼负锛孯OS2 discovery 閲屼篃鑳界湅鍒?liveliness 绫诲瀷銆?
杩欐槸涓€濂?QoS 鎵╁睍鍜岄獙璇佸師鍨嬶紝涓嶆槸瀹屾暣 DDS QoS 浜у搧瀹炵幇銆傚畬鏁翠骇鍝佺骇瀹炵幇杩橀渶瑕?late-joiner銆乸acket loss銆並EEP_ALL/KEEP_LAST銆乀RANSIENT_LOCAL銆乴iveliness lost/recovered銆乺esource exhaustion銆佽法 DDS vendor 绛夌郴缁熸祴璇曘€?```

褰撳墠缁煎悎 demo 浣跨敤锛?
```text
ESP32 -> ROS2: RELIABLE
ROS2 -> ESP32: RELIABLE
Durability   : VOLATILE
History      : KEEP_LAST(5)
Deadline     : 100ms / ROS2 reply discovery shows finite deadline
Lifespan     : 2000ms
Liveliness   : AUTOMATIC, lease=3000ms
Resources    : 30 samples, 12288 bytes
```

鍏充簬 RELIABLE 杈圭晫锛岀幇鍦哄彛寰勶細

```text
褰撳墠姝ｅ紡 demo 鏄弻鍚?RELIABLE锛屽苟涓斿凡缁忛€氳繃褰撳墠鏈哄櫒鐨勫疄鏈?preflight 鍜?3 娆?reset 鍘嬪姏娴嬭瘯銆?浣嗚繖浠嶇劧鏄紨绀虹骇/鍘熷瀷绾ч獙璇侊紝涓嶇瓑浠蜂簬瀹屾暣 DDS RELIABLE 浜у搧绾ц璇併€?濡傛灉鐜板満缃戠粶 discovery 鍋跺彂涓嶆敹鏁涳紝preflight 鑴氭湰浼氳嚜鍔ㄩ噸璇曪紝姝ｅ紡灞曠ず鍓嶅彧鎺ュ彈 PASS 鐘舵€併€?```

## 8. 甯歌鏁呴殰澶勭悊

### 8.1 `/dev/ttyUSB0` 涓嶅瓨鍦?
PowerShell锛?
```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
usbipd detach --busid X-Y
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
```

WSL锛?
```bash
ls -l /dev/ttyUSB0
```

### 8.2 `idf.py: 鏈壘鍒板懡浠

WSL锛?
```bash
source ~/esp-idf/export.sh
```

鐒跺悗閲嶆柊杩愯 `idf.py` 鍛戒护銆?
### 8.3 `idf.py monitor` 鎶?`/dev/ttyUSB0` 涓嶅瓨鍦?
璇存槑 USB 娌℃湁鎴愬姛鎸傚埌 WSL2銆傚洖鍒?8.1銆?
### 8.4 ROS2 host 閲嶅鍚姩

WSL锛?
```bash
pkill -f 'workspace/step7_full_qos/echo_reply.py' || true
```

鐒跺悗閲嶆柊鍚姩 Terminal A銆?
### 8.5 鏉垮瓙鐧藉厜鍒虹溂

褰撳墠鍥轰欢鍚姩鏃朵細鍏抽棴鏉胯浇 RGB/鐧藉厜 LED銆傚鏋滀粛鐒朵寒锛屽厛鐭寜 `RST/RESET/EN` 璁╂柊鍥轰欢閲嶆柊鍚姩銆?
### 8.6 閫€鍑?monitor

鍦?ESP-IDF monitor 涓寜锛?
```text
Ctrl + ]
```

### 8.7 discovery 娌℃湁鍖归厤

濡傛灉 ESP32 monitor 涓病鏈夊嚭鐜帮細

```text
publisher matched with remote subscriber
subscriber matched with remote publisher
```

鍏堣繍琛岃嚜鍔ㄥ娴嬭剼鏈紝瀹冧細鍒锋柊 ROS2 daemon 骞堕噸鏂板惎鍔?host锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_demo_preflight.sh /dev/ttyUSB0 3
```

濡傛灉鑷姩澶嶆祴閫氳繃锛屽啀鎸夋寮忕幇鍦烘楠ら噸鏂板紑 Terminal A/B/C銆?
### 8.8 monitor 鍑虹幇 `DEMO NOT READY`

杩欒〃绀?ESP32 鍚姩鍚?warm-up 娌℃湁鏀跺埌 ROS2 鍥炲寘锛屾寮?QoS 闃舵宸茬粡涓诲姩鍋滄锛岄伩鍏嶄骇鐢熻瀵肩粨鏋溿€?
鎸夐『搴忓鐞嗭細

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_demo_preflight.sh /dev/ttyUSB0 3
```

濡傛灉 preflight 閫氳繃锛屽啀閲嶆柊鍚姩 Terminal A/B锛屽苟鐭寜 `RST/RESET/EN`銆?
## 9. 鐜板満缁撴潫鍓嶄繚鐣欑殑璇佹嵁

寤鸿鎴浘鎴栧綍灞忎繚鐣欎笁澶勶細

```text
1. PowerShell: usbipd list 鏄剧ず CP2102N Attached銆?2. ESP32 monitor: TX/RX銆丩atency銆丳ackets Dropped: 0銆丄ll phases complete銆?3. ROS2 topic info --verbose: ESP32->ROS2 RELIABLE锛汻OS2->ESP32 RELIABLE锛汥urability銆丠istory 瀛楁銆丏eadline銆丩ifespan銆丩iveliness銆?```

涔熷彲浠ヤ繚鐣欒嚜鍔ㄥ娴嬫棩蹇楋細

```text
/tmp/mros2_qos_serial.log
/tmp/mros2_qos_host.log
/tmp/mros2_qos_topic.log
```

