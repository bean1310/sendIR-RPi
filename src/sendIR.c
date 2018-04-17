/*
 * File:   main.c
 * Author: BEAN
 *
 * Created on April 16, 2018, 21:00
 */

 /*
  * このプログラムについて
  * ^^^^^^^^^^^^^^^^^^^
  * 赤外線センサより赤外線信号を読み、解析してそのデータを表示するRaspberry Pi 3用のプログラム
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <wiringPi.h>
// GPIO指定をわかりやすくする自作ヘッダファイル
#include "rpi-pin.h"

// 赤外線センサのOutputピンに接続するGPIOピンの指定
const unsigned int SEND_PIN = GPIO_18;

// 赤外線信号を繰り返し送信する回数.
const unsigned int REPEAT_SEND_CODE = 3;

// 変調単位の設定(マイクロ秒)
const unsigned int MODULATION_TIME = 560;

int main(int argc, char *argv[]){

    FILE *fp;
    char fileName[32];
    int onTime, offTime;
    int i, n;

    if (argc > 1 && argc < 3) {
        strcpy(fileName, argv[1]);
    }else{
        puts("Usage: ./readIR [Input file name]");
    }

    if ( (fp = fopen(fileName, "r")) == NULL ) {
        perror("Failed to open IR-Data file !!");
        return 1;
    }

    if(wiringPiSetup() == -1) return 1;

    pinMode(SEND_PIN, OUTPUT);

    for(n = 0; n < REPEAT_SEND_CODE; n++) {

        rewind(fp);

        while(fscanf(fp, "%d %d", &onTime, &offTime) != EOF) {

        for(i = 0; i < onTime / 26; i++) {

            digitalWrite(SEND_PIN, 1);
            delayMicroseconds(9);
            digitalWrite(SEND_PIN, 0);
            delayMicroseconds(17);

        }

        digitalWrite(SEND_PIN, 0);
        delayMicroseconds(offTime);

#ifdef DEBUG
        printf("%d %d\n", onTime, offTime);
#endif


        }

    }

    fclose(fp);

    return 0;
}
