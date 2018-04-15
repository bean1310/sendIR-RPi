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

#define MODULATION_TIME 560

int main(int argc, char *argv[]){

    FILE *fp;
    char fileName[32];
    int onTime, offTime, signalBit;
    int i, n;

    if (argc > 1 && argc < 3) {
        strcpy(fileName, argv[1]);
    }else{
        puts("Usage: ./sendIR [Input file name]");
    }

    if ( (fp = fopen(fileName, "r")) == NULL ) {
        perror("Failed to open IR-Data file !!");
        return 1;
    }

    if(wiringPiSetup() == -1) return 1;

    pinMode(SENDPIN, OUTPUT);

    /* Send Leader */
    for(i = 0; i < MODULATION_TIME * 16 / 26; i++) {

            digitalWrite(SENDPIN, 1);
            delayMicroseconds(9);
            digitalWrite(SENDPIN, 0);
            delayMicroseconds(17);

    }

    digitalWrite(SENDPIN, 0);
    delayMicroseconds(MODULATION_TIME * 8);    

    for(n = 0; n < 3; n++) {

        rewind(fp);

        fseek(fp, (int)sizeof(int) * 7, SEEK_SET);

        while(fscanf(fp, "%d %d", &onTime, &offTime) != EOF) {

            printf("%d %d\n", onTime, offTime);

            signalBit = offTime / onTime >= 3 ? 1 : 0;

            for(i = 0; i < MODULATION_TIME / 26; i++) {

                digitalWrite(SENDPIN, 1);
                delayMicroseconds(9);
                digitalWrite(SENDPIN, 0);
                delayMicroseconds(17);

            }
                
        digitalWrite(SENDPIN, 0);
        delayMicroseconds(MODULATION_TIME * ( 1 + signalBit * 2 ) );

        }

    }

    fclose(fp);

    return 0;
}
