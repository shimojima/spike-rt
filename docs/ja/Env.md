# 開発環境の構築
- ここでは、WSL2(Windows), Mac, Ubuntu(Linux)上で環境を構築することを想定している。
- アプリケーション・コンテナ（Docker など）を使わない方法を紹介する。
- ※　Linuxの場合は，Ubuntuを想定。
- ※　Macの場合は，Apple Siliconを想定。
- ※　Windowsの場合は，UbuntuイメージのWSL2上での作業を想定。

各作業：
| 作業内容 | 実行方法 |
| :--- | :--- |
| コーディング | 任意のエディタ（ホスト） |
| [ビルド](#ビルド環境の構築) | コンパイラ |
| [書き込み](#書き込み環境の構築) | python3（ホスト） |
| [シリアル通信](#シリアル通信)（任意） | minicomなど（ホスト） |
| [Bluetoothシリアル通信](#Bluetoothシリアル通信)（任意） | Google Chromeなど（ホスト） |


## ビルド環境の構築

### Windows (WSL) の場合
WSLのUbuntuで、以下を実行。
```bash
sudo apt update
sudo apt install git ruby make gcc-arm-none-eabi
```

### Linuxの場合
以下を実行。
```bash
sudo apt update
sudo apt install git ruby make gcc-arm-none-eabi
sudo python3 python3-usb
```

### Macの場合
- https://developer.arm.com/downloads/-/gnu-rm から gcc-arm-none-eabi-10.3-2021.10-mac.tar.bz2 をダウンロードし、/usr/local に展開しておく。
※gcc-arm-embedded(2026.01現在、v15.2.rel1)ではビルドできない。
- パスを切っておく
```bash
echo "export PATH=\"/usr/local/gcc-arm-none-eabi-10.3-2021.10/bin:\${PATH}\"" >> ~/.zprofile
```

## 書き込み環境の構築
Prime Hub への書き込みをホスト側で行うことを想定している。
PyUSB(Python3) を用いて書き込みを行う。

WSL2(Windows) と Mac については，前準備が必要。

### PyUSBの導入の前準備(Windowsの場合)
python3はWindowsにインストールする。

①python3
- python3.exe -- https://pythonlinks.python.jp/ja から python-3.14.0-amd64.exe >など
※必ず、「Add python.exe to PATH」にチェックを入れること

②　pyusb
― Windows PowerShellでpyusbをインストールしておく
```bash
pip install pyusb
```
※pyusbを入れ忘れると、後々「No module named usb」というエラーで落ちる

③libusb-win32ドライバー（一度変更しておけばOK）
- https://zadig.akeo.ie から zadig-2.9.exe をダウンロードし、インストールする
- 最初にSPIKE-RTをDFUモードで接続したら、zadigを起動し、「Options」→「List All Devices」→ 「LEGO Technic Large Hub in DFU Mode」を選ぶ
 - ドライバーを「WinUSB」から「libusb-win32」へ変更し、「Install Driver」を押す
※ドライバーが正しくないと、ファイル転送が「No backend available」で失敗する

### PyUSBの導入の前準備(Macの場合)
PyUSBの依存ライブラリのインストール
```bash
brew install libusb
```

## シリアル通信
USBまたはUSART(Port F)経由のシリアル通信について説明．
Bluetoothの場合は，[Bluetoothシリアル通信](#Bluetoothシリアル通信)を参照．

`minicom`によりHubとシリアル通信することを想定．  
以下などの方法により`minicom`をインストールする．

### Ubuntu(Linux)の場合
```bash
sudo apt install minicom
```

### Windowsの場合
Linux同様、minicomをインストールしても良いし、TeraTerm UTF-8 などでも良い

### Macの場合
```bash
brew install minicom
```

## Bluetoothシリアル通信
SPIKE-RTでは，Pybricksのデバイスドライバを再利用している．
現状では，Pybricksの[Web IDE](https://beta.pybricks.com/)経由でのBluetooth接続のみに対応している．

Pybricksの[Web IDE](https://beta.pybricks.com/)は，Firefox等には対応していない．
Google ChromeやChromiumブラウザを使用する．

接続時のデバイス名は、Pybricks Hub に設定されている。変更したい場合は、sdk/workspace/HUB_NAME
というファイルを作り新しいデバイス名を書いておくと、asp.binファイル生成時に置き換えられる。
デバイス名は、15文字以内の半角英数字・記号とする。
