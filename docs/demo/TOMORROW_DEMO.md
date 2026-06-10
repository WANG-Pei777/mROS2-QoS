# Tomorrow Demo Checklist

鏄庡ぉ姝ｅ紡灞曠ず鍙寜杩欎釜璧般€傚畬鏁磋В閲婅 `docs/demo/DEMO_RUNBOOK.md`銆?
## 0. 瀹為獙瀹ゅ彟涓€鍙扮數鑴戝畬鏁村懡浠?
鍓嶆彁锛?
```text
鍙︿竴鍙扮數鑴戝凡缁忔湁 WSL2 Ubuntu-22.04銆丷OS2 Humble銆丒SP-IDF銆乽sbipd-win銆?椤圭洰璺緞鍥哄畾涓?/home/your-user/mros2/mros2-esp32銆?ESP32 鍜岀數鑴戝湪鍚屼竴涓?WiFi/灞€鍩熺綉銆?```

### 0.1 绠＄悊鍛?Windows PowerShell

鍙仛涓€娆★紝鐢ㄦ潵鏀捐 WSL2 DDS/RTPS 鍏ョ珯 UDP锛?
```powershell
Set-ExecutionPolicy -Scope Process Bypass
& "\\wsl.localhost\Ubuntu-22.04\home\your-user\mros2\mros2-esp32\scripts\demo\qos_wsl_firewall_admin.ps1"
```

### 0.2 鏅€?Windows PowerShell

鍚姩 WSL 淇濇椿锛屾煡鐪?ESP32 涓插彛璁惧锛?
```powershell
Start-Process -FilePath wsl.exe -ArgumentList '-d Ubuntu-22.04 -- sleep 900' -WindowStyle Hidden
usbipd list
```

鎵惧埌 `CP2102N USB to UART Bridge Controller` 鎵€鍦ㄨ銆備笅闈㈢敤 `X-Y` 琛ㄧず鐪熷疄 BUSID锛屼緥濡?`2-4`銆?
濡傛灉鐘舵€佹槸 `Not shared`锛屽厛鍦ㄧ鐞嗗憳 Windows PowerShell 杩愯锛?
```powershell
usbipd bind --busid X-Y
```

濡傛灉鐘舵€佷笉鏄?`Attached`锛屽湪鏅€?Windows PowerShell 杩愯锛?
```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
usbipd list
```

鏈€缁堝繀椤荤湅鍒?CP2102N 鏄細

```text
Attached
```

### 0.3 WSL 棰勬銆佹瀯寤恒€佺儳褰曘€佸疄鏈洪獙璇?
鏂板紑鏅€?PowerShell锛岃繘鍏?WSL锛?
```powershell
wsl -d Ubuntu-22.04
```

鍦?WSL 閲岃繍琛岋細

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

濡傛灉杩樻湁鏃堕棿锛岃拷鍔?3 娆?reset 鍘嬪姏娴嬭瘯锛?
```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_reset_stress.sh 3 /dev/ttyUSB0 140
```

鍙帴鍙楋細

```text
[stress] RESULT: PASS
```

## A. 婕旂ず鍓?10 鍒嗛挓

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

`X-Y` 鏄?`CP2102N USB to UART Bridge Controller` 鐨勭湡瀹?BUSID銆俙usbipd` 鏄?Windows PowerShell 鍛戒护锛屼笉瑕佽繘鍏?WSL 鍚庡啀鏁层€?
濡傛灉 `usbipd list` 鏄剧ず CP2102N 鏄?`Not shared`锛屽厛鍦ㄧ鐞嗗憳 Windows PowerShell 杩愯锛?
```powershell
usbipd bind --busid X-Y
```

濡傛灉 CP2102N 涓嶆槸 `Attached`锛屾櫘閫?Windows PowerShell 杩愯锛?
```powershell
usbipd attach --wsl Ubuntu-22.04 --busid X-Y
usbipd list
```

WSL:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_tomorrow_ready.sh /dev/ttyUSB0 all
```

鍙帴鍙楋細

```text
[verify] RESULT: PASS
[ready] RESULT: ALL PASS
```

## B. 姝ｅ紡灞曠ず涓夌獥鍙?
绐楀彛 1锛學indows PowerShell:

```powershell
usbipd list
```

璇存槑 CP2102N 宸茬粡 `Attached`銆?
绐楀彛 2锛學SL:

```bash
cd /home/your-user/mros2/mros2-esp32
./scripts/demo/qos_policy_host.sh all
```

绐楀彛 3锛學SL:

```bash
cd /home/your-user/mros2/mros2-esp32/workspace/step7_full_qos
source ~/esp-idf/export.sh
idf.py -p /dev/ttyUSB0 monitor
```

鐭寜 ESP32 `RST/RESET/EN`銆?
绐楀彛 4锛屽彲閫夛紝WSL:

```bash
source /opt/ros/humble/setup.bash
ros2 topic info /step7_full_qos --verbose
ros2 topic info /step7_full_qos_reply --verbose
```

## C. 鐜板満鍙ｅ緞

```text
杩欎笉鏄豢鐪熴€俁OS2 Humble 鍦?WSL2锛宮ROS2 鍦?ESP32-S3锛岄€氫俊璧?WiFi DDS/RTPS锛孶SB 涓插彛鍙湅鏃ュ織銆?
ESP32 鍙戝竷 /step7_full_qos 缁?ROS2锛屼娇鐢?RELIABLE銆?ROS2 鏀跺埌鍚庡彂甯?/step7_full_qos_reply 鍥炵粰 ESP32锛屼篃浣跨敤 RELIABLE銆?鍥犳 ESP32 鍜?ROS2 閮藉悓鏃舵壙鎷?publisher 鍜?subscriber锛岃瘉鏄庡弻鍚戝疄鏈轰簰閫氥€?
褰撳墠椤圭洰鏄?QoS 鎵╁睍鍘熷瀷锛屼笉鏄畬鏁?DDS QoS 浜у搧绾у疄鐜般€?鍙互灞曠ず Reliability銆丏urability銆丠istory銆丏eadline銆丩ifespan銆丩iveliness銆丷esource Limits 涓冪被閰嶇疆/琛屼负璇佹嵁銆?```

## D. 鏈€鏂板疄鏈洪獙璇佽褰?
鎴嚦 2026-06-10锛屽綋鍓嶆満鍣ㄥ凡缁忓畬鎴愪弗鏍肩増鍙屽悜 RELIABLE 瀹炴満楠岃瘉锛?
```text
瀹屾暣棰勬: PASS
缁撴灉鐩綍: /home/your-user/mros2/mros2-esp32/results/qos_preflight_20260610_022457

reset 鍘嬪姏澶嶆祴: 3/3 PASS
缁撴灉鐩綍: /home/your-user/mros2/mros2-esp32/results/qos_reset_stress_20260610_022750

鍏抽敭瑕佹眰:
ESP32 -> ROS2: RELIABLE
ROS2 -> ESP32: RELIABLE
ROS2 topic info evidence: Reliability/Durability/History field/Deadline/Lifespan/Liveliness
ESP32 behavior evidence: History KEEP_LAST(5), Resource Limits burst rejection
```

杩欎簺璁板綍璇存槑褰撳墠婕旂ず璺緞鍦ㄥ疄鏈轰笂宸茬粡璺戦€氾紱瀹冧滑涓嶅簲琚〃杩颁负瀹屾暣 DDS QoS 浜у搧绾ц璇併€?
