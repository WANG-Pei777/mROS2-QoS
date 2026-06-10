# Live Demo Cheatsheet

杩欎釜鏂囦欢鐢ㄤ簬姝ｅ紡婕旂ず褰撳ぉ蹇€熺収鐫€璧般€傚畬鏁村鍒绘楠ょ湅 `docs/demo/DEMO_RUNBOOK.md`銆?
## 0. 鍙︿竴鍙扮數鑴戝厛璺戣繖涓€缁?
绠＄悊鍛?Windows PowerShell:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mros2\mros2-esp32\scripts\demo\qos_wsl_firewall_admin.ps1"
```

鏅€?Windows PowerShell:

```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
```

`X-Y` 鎹㈡垚 CP2102N 鐨勭湡瀹?BUSID锛涘鏋?CP2102N 鏄?`Not shared`锛屽厛鐢ㄧ鐞嗗憳 PowerShell 鎵ц `usbipd bind --busid X-Y`銆?
濡傛灉 CP2102N 涓嶆槸 `Attached`锛屾櫘閫?Windows PowerShell 鎵ц锛?
```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
usbipd list
```

WSL:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

鍙帴鍙?

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

## 1. 鏄庡ぉ婕旂ず鍓嶅繀椤诲厛鍋?
绠＄悊鍛?Windows PowerShell:

```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mros2\mros2-esp32\scripts\demo\qos_wsl_firewall_admin.ps1"
```

杩欎竴姝ユ斁琛?WSL2 mirrored mode 涓?DDS/RTPS UDP `7400-7420`銆傛病鏈夎繖涓€姝ユ椂锛孍SP32 鍙兘鏄剧ず宸茬粡鍙?discovery锛屼絾 WSL 鎶撲笉鍒板寘銆?
鏅€?Windows PowerShell:

```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
```

纭 CP2102N 鏄?`Attached`銆傚鏋滀笉鏄紝鎶?`X-Y` 鎹㈡垚瀹為檯 BUSID:

```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
```

`usbipd` 鏄?Windows PowerShell 鍛戒护锛屼笉瑕佽繘鍏?WSL 鍚庡啀鏁层€傚鏋滅姸鎬佹槸 `Not shared`锛屽厛鍦ㄧ鐞嗗憳 Windows PowerShell 鎵ц `usbipd bind --busid X-Y`銆?
WSL:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

蹇呴』鐪嬪埌:

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

鏃堕棿鍏佽鏃跺啀璺?2-3 娆?reset 鍘嬪姏棰勬:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_reset_stress.sh 3 /dev/ttyUSB0
```

蹇呴』鐪嬪埌:

```text
[stress] RESULT: PASS
```

## 2. 涓夌獥鍙ｆ寮忔紨绀?
绐楀彛 1, Windows PowerShell:

```powershell
usbipd list
```

璇存槑 CP2102N 宸茬粡 `Attached` 鍒?WSL2銆?
绐楀彛 2, WSL Terminal A:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_policy_host.sh all
```

淇濇寔绐楀彛涓嶅姩锛岀湅鍒?

```text
Echo reply node started, listening on /step7_full_qos
```

绐楀彛 3, WSL Terminal B:

```bash
cd /home/your-user/mros2/mros2-esp32/workspace/step7_full_qos
source ~/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 monitor
```

鐭寜 ESP32 鐨?`RST`銆乣RESET` 鎴?`EN`銆?
## 3. 蹇呴』鎸囧嚭鐨勮瘉鎹?
ESP32 monitor:

```text
publisher matched with remote subscriber
subscriber matched with remote publisher
Warm-up reply confirmed
[ROS2 -> ESP32] Echo reply received
Lifespan check PASSED
Liveliness lease check PASSED
Liveliness finite lease behavior PASSED
Rejected during burst
TX: 40 msgs
RX: nonzero; final verification example was RX: 27 msgs
Packets Dropped:  0
All phases complete.
```

瑙ｉ噴鍙ｅ緞:

```text
TX 鏄?ESP32 鍙戝竷鍒?ROS2銆?RX 鏄?ESP32 鏀跺埌 ROS2 鍥炲寘銆?鎵€浠ヨ繖閲岃瘉鏄庝簡 WSL2 ROS2 涓?mROS2-esp32 鐨勫弻鍚戝疄鏈轰簰閫氥€?```

绐楀彛 4, WSL Terminal C:

```bash
source /opt/ros/humble/setup.bash
ros2 topic info /step7_full_qos --verbose
ros2 topic info /step7_full_qos_reply --verbose
```

閲嶇偣灞曠ず:

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
```

## 4. 30 绉掕瑙ｇ

```text
杩欏婕旂ず鏄疄鏈洪摼璺紝涓嶆槸浠跨湡銆俁OS2 Humble 杩愯鍦?WSL2锛孍SP32-S3 杩愯 mROS2 鍥轰欢銆?USB 涓插彛鍙礋璐ｈ瀵熸棩蹇楋紝瀹為檯閫氫俊璧?WiFi DDS/RTPS銆?
ESP32 鍏堜互 RELIABLE QoS 鍙戝竷 /step7_full_qos锛孯OS2 host 鏀跺埌鍚庝篃浠?RELIABLE QoS 鍙戝竷 /step7_full_qos_reply 鍥炵粰 ESP32銆傝繖鏍蜂富鐜板満 demo 鏄弻鍚?RELIABLE 瀹炴満閫氫俊銆?鎵€浠?ESP32 鍜?ROS2 涓よ竟閮藉悓鏃舵壙鎷?publisher 鍜?subscriber銆?
鏃х増 mROS2-esp32 鐨勫叕寮€ demo 涓昏浣撶幇 BEST_EFFORT 椋庢牸鐨勭畝鍗曢€氫俊銆傝繖涓湰鍦伴」鐩湪姝ゅ熀纭€涓婂仛浜?QoS 鎵╁睍鍘熷瀷锛?褰撳墠 demo 瑕嗙洊 Reliability銆丏urability銆丠istory銆丏eadline銆丩ifespan銆丩iveliness銆丷esource Limits 涓冪被 QoS 鐩稿叧閰嶇疆鍜屾祴璇曠偣銆?
杩欓噷涓嶈兘璇村畬鏁?DDS QoS 宸茬粡浜у搧绾у疄鐜般€傛洿鍑嗙‘鍦拌锛?ESP32 -> ROS2 鐨?RELIABLE 涓婅宸茬粡鏈夊疄鏈鸿瘉鎹紱
ROS2 -> ESP32 鐨?RELIABLE 鍥炲寘璺緞涔熷凡缁忔湁瀹炴満璇佹嵁锛?Reliability銆丏urability銆丏eadline銆丩ifespan銆丩iveliness 鍙互鐢?ROS2 topic info 鐩存帴鐪嬪埌鍏蜂綋鍊硷紱
History 瀛楁鑳藉湪 topic info 涓湅鍒帮紝浣?depth 鍦?ROS2 Humble 褰撳墠杈撳嚭涓槸 UNKNOWN锛屾墍浠?KEEP_LAST(5) 鐢?ESP32 琛屼负鏃ュ織璇佹槑锛?Resource Limits 涓嶇敱鏍囧噯 topic info 鎵撳嵃锛岀敤 ESP32 burst rejection 琛屼负娴嬭瘯璇佹槑銆?```

## 5. 鍑洪棶棰樻椂鍙仛杩欎笁浠朵簨

1. `/dev/ttyUSB0` 娌℃湁:

```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
```

2. discovery 娌″尮閰?

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_demo_preflight.sh /dev/ttyUSB0 3
```

3. monitor 閫€鍑?

```text
Ctrl + ]
```

