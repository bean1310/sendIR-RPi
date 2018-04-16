/*
 * File:   main.c
 * Author: BEAN
 *
 * Created on August 2, 2017, 3:57 AM
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
#define SENDPIN GPIO_18

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

    pinMode(SENDPIN, OUTPUT);

    for(n = 0; n < 1; n++) {

        rewind(fp);

    while(fscanf(fp, "%d %d", &onTime, &offTime) != EOF) {

        for(i = 0; i < onTime / 26; i++) {

            digitalWrite(SENDPIN, 1);
            delayMicroseconds(9);
            digitalWrite(SENDPIN, 0);
            delayMicroseconds(17);

        }

            digitalWrite(SENDPIN, 0);
            delayMicroseconds(offTime);

        printf("%d %d\n", onTime, offTime);

    }

    }

    fclose(fp);

    return 0;
}
