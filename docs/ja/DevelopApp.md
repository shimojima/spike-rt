# アプリケーションの開発方法
アプリケーション開発における一連の流れを説明する。

## 必要なパッケージのインストール（Ubuntu/WSL2）

[ENV.md](Env.md)を参照して開発環境を構築しておく。

## ソースコードの入手
以下により、ソースコードをクローンする。
```bash
git clone https://github.com/shimojima/spike-rt.git
```
以下、特に断りの無い限りトップディレクトリが`spike-rt` のディレクトリであるとする。
また、アプリケーション・ソースコードのディレクトリが `spike-rt/sdk/workspace/$appdir`に配置されているとする。

## 開発手順
RasPikeやRasPike-ARTに合わせて、sdk/workspace の中に、プロジェクトファイルを置く。
例として、RasPike-ART の sample_c5_spike を移植して、sdk/workspace/sample_c5 に置いておく。

### ビルド
```bash
cd ~/spike-rt/sdk/workspace
make img=sample_c5
```

asp.binファイルを sdk/workspace フォルダにコピーし、プロジェクトのフォルダ名を appdir に書き込む。

中間ファイル等は、プロジェクトの下の build フォルダに生成される。
カーネルライブラリ等も、必要に応じて自動生成される。これらはプロジェクト共通のため、
~/spike-rt/build 以下に保存する。

### クリーンアップ

プロジェクトの中間ファイル等を消すコマンド：
```bash
cd ~/spike-rt/sdk/workspace
make clean 
```
ただし、影響があるのは、appdir で指定されているプロジェクトだけ。appdirファイルがなかったり別の場所を指している場合は、ほかのプロジェクトのbuildフォルダには触らない。

カーネルライブラリ等も消したければ：
```bash
cd ~/spike-rt/sdk/workspace
make realclean 
```

また、（git pullなどして、asp3, drivers, external配下のファイルが更新された場合などに）sdk/workspace以下のすべてのプロジェクトに対して realclean するには：
```bash
cd ~/spike-rt/sdk/workspace
make distclean 
```

## 書き込み
SPIKE Prime Hub に転送するファイルは、asp.bin である。

### HubのDFUモードへの遷移
- USBケーブルを抜き，Hubの電源を切る。
- Bluetoothボタンを押しながらUSBを接続する。
- ボタンのLEDが点滅するまで，Bluetoothボタンを押し続ける。

### 書き込みの実行
SPIKE Prime HubをDFUモードにしてPCに接続し、Windows上のpythonをWSL2から起動してasp.binをHubに書き込む。
```bash
cd ~/spike-rt/sdk/workspace
make upload
```

sdk/workspace/asp.bin が転送される。

## USBシリアルの接続
WindowsのTeratermなどでUSBシリアルに接続し，ログ出力を確認することができる。
一旦，Hub の電源をONにしてからでないと，接続できないことに注意する。

