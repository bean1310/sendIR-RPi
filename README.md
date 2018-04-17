# sendIR-RPi

## What is this ?
Raspberry Pi用の赤外線信号を送信するCプログラム.  
GPIOピン配置はRaspberry Pi 3 Model Bと同様のもの(以降これらをRPiと書く)なら動く(と思う).

## build
`make`すると実行ファイルができる.

## How to use
赤外線LEDをRPiに繋ぐ.(赤外線LED: OSI5FU5111C-40で動作確認)
赤外線LEDのアノードを
```objectivec
#define SEND_PIN
```
で定義されたGPIOピンに接続する(デフォルトはGPIOの18番ピン).  
...のでもいいけれど電流が足りないので, LEDはRPiの5vピンから電源を取り,  
SEND_PINに指定されたGPIOピンにトランジスタのベースを接続して使いほうが良い.

これで `./sendIR <readIRにより出力されたファイル名>`を実行すると, 接続された赤外線LEDからしたいされたファイルの赤外線信号が送信される.

## What is the difference between sendIR(.c) and sendIR2(.c)

sendIR(.c)とsendIR2(.c)の違いは  
readIRで受信した赤外線信号データを生データで送信するか, NECフォーマットで整形して送信するかの違い.

赤外線リモコンの通信フォーマットに関しては, [こちらのサイト](http://elm-chan.org/docs/ir_format.html)が参考になります.

## Attentions
このソフトウェアを使用したことによる機器の故障などの損害については一切の責任を負いません.