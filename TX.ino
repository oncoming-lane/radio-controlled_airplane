
#define pinX1 A1       // ось X джойстика1
#define pinY1 A0       // ось Y джойстика1
//#define pinX1 A3       // ось X джойстика2
#define pinY2 A3       // ось Y джойстика2
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

void setup() {
  pinMode(pinX1, INPUT); // указываем пин для джойстика
  pinMode(pinY1, INPUT);  
  //pinMode(pinX2, INPUT); 
  pinMode(pinY2, INPUT);
  Serial.begin(9600);         // открываем порт для связи с ПК

  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(1, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openWritingPipe(address[0]);  // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x70);             // выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MIN);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике
  //при самой низкой скорости имеем самую высокую чувствительность и дальность

  radio.powerUp();        // начать работу
  radio.stopListening();  // не слушаем радиоэфир, мы передатчик
}

void loop() {
  int X1 = analogRead(pinX1);      // считываем значение оси X
  int Y1 = analogRead(pinY1);      // считываем значение оси Y
  int Y2 = analogRead(pinY2);

  X1 = map(X1, 0, 1023, 0, 100); // переводим значение X в новый диапазон
  Y1 = map(Y1, 0, 1023, 0, 100);  // переводим значение Y в новый диапазон
  Y2 = map(Y2, 0, 1023, 0, 255);
  int transmit_data[3] = {X1, Y1, Y2};
  Serial.print("Sent: ");
  Serial.print(transmit_data[0]);
  Serial.print(" ");
  Serial.println(transmit_data[1]);
  Serial.print(" ");
  Serial.println(transmit_data[2]);
  radio.write(&transmit_data, sizeof(transmit_data));
  delay(10);
}