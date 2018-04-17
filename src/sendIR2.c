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

/* OnTime, OffTimeを保存する構造体 */
struct onofftime{

	unsigned int onTime;
	unsigned int offTime;
	struct onofftime *next;

};

// 赤外線センサのOutputピンに接続するGPIOピンの指定
const unsigned int SEND_PIN = GPIO_18;

// 赤外線信号を繰り返し送信する回数.
const unsigned int REPEAT_SEND_CODE = 1;

// 変調単位の設定(マイクロ秒)
const unsigned int MODULATION_TIME = 562;

const unsigned int DUTY = 3;

void freeOnOffTime(struct onofftime *p) {
	
	if(p -> next != NULL) freeOnOffTime(p -> next);

	free(p);

}

int main(int argc, char *argv[]){

    FILE *fp;
	struct onofftime onOffTime = {0, 0, NULL};
	struct onofftime *tmpAddr = NULL;
    char fileName[128];
    int onTime, offTime, signalBit;
    int n;

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

	/* SEND_PINの初期設定 */
    pinMode(SEND_PIN, PWM_OUTPUT);
    pwmSetMode(PWM_MODE_MS);
    pwmSetRange(DUTY);
    pwmSetClock((int)(19200 / 38 / DUTY));

	/* リーダコード部分は読み込まない */
	fseek(fp, (int)sizeof(int) * 7, SEEK_SET);
	
	tmpAddr = &onOffTime;

    while(fscanf(fp, "%d %d", &onTime, &offTime) != EOF) {
	
		struct onofftime *newAddr = (struct onofftime *)malloc(sizeof(struct onofftime));
		newAddr -> onTime = onTime;
		newAddr -> offTime = offTime;
		newAddr -> next = NULL;

		tmpAddr -> next = newAddr;
		tmpAddr = newAddr;

	}

    for(n = 0; n < REPEAT_SEND_CODE; n++) {		

		tmpAddr = onOffTime.next;

        /* Send Leader Code */
        pwmWrite(SEND_PIN, 1);
        delayMicroseconds(MODULATION_TIME * 16);
        pwmWrite(SEND_PIN, 0);
        delayMicroseconds(MODULATION_TIME * 8);
		
		while( tmpAddr != NULL ){
		
			// データの整形
        	signalBit = tmpAddr -> offTime / tmpAddr -> onTime >= 2 ? 1 : 0;

        	// データの送信
        	pwmWrite(SEND_PIN, 1);
        	delayMicroseconds(MODULATION_TIME);
        	pwmWrite(SEND_PIN, 0);
        	delayMicroseconds(MODULATION_TIME * ( 1 + signalBit * 2 ) );
	
#ifdef DEBUG
			printf("onTime: %d, offTime: %d, send code is %d\n", tmpAddr -> onTime, tmpAddr -> offTime, signalBit);
#endif
			tmpAddr = tmpAddr -> next;
		
		}

    }

    fclose(fp);
	
	tmpAddr = onOffTime.next;

	freeOnOffTime(tmpAddr);

    return 0;
}
