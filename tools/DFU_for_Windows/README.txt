【準備（一度だけ）】
- pythonをインストールしてください
　インストーラ：python-x.xx.x-amd64.exe @ https://pythonlinks.python.jp/
- Windows Powershellで pyusbをインストールしてください
  pip install pyusb
- Zadigで「LEGO Technic Large Hub in DFU Mode」のDriverをlibusb-win32に変更してください
　zadig-x.xx.exe @ https://zadig.akeo.ie
　※「No backend available」というエラーが出る場合は、USBドライバが変更されていないので確認してください
- dfu.pyとpydfu.pyは、このファイルと同じフォルダに置いたままにしてください

【ファイルのアップロード】
書き込むファイル（asp.bin）はどこにあっても構いません。引数で渡すか、このファイルにドラッグしてください

