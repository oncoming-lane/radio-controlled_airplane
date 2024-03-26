#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h> // подключаем библиотеку для серво
Servo servo1;           // объявляем объект servo1
Servo servo2;           // объявляем объект servo2

RF24 radio(9, 10);  // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб

void setup() {
  servo1.attach(5);            // указываем пин для первого серво
  servo2.attach(3);         // указываем пин для второго серво
  pinMode(6, OUTPUT);
  Serial.begin(9600);         // открываем порт для связи с ПК
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(1, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах

  radio.openReadingPipe(1, address[0]);   // хотим слушать трубу 0
  radio.setChannel(0x70);     // выбираем канал (в котором нет шумов)

  radio.setPALevel (RF24_PA_MIN);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике
  //при самой низкой скорости имеем самую высокую чувствительность и дальность

  radio.powerUp();        // начать работу
  radio.startListening(); // начинаем слушать эфир, мы приёмный модуль
}

void loop() {
  
  byte pipeNo;
  int reciv_data[3];
  while (radio.available(&pipeNo)) {        // слушаем эфир со всех труб
    radio.read(&reciv_data, sizeof(reciv_data));  // чиатем входящий сигнал
    Serial.print("Recieved: ");
    Serial.print(reciv_data[0]);
    Serial.print(" ");
    Serial.println(reciv_data[1]);
    Serial.print(" ");
    Serial.println(reciv_data[2]);
    servo1.write(reciv_data[0]*1.8);                      // поворачиваем первый серво
    servo2.write(reciv_data[1]*1.8);
    analogWrite(6, reciv_data[2]);
    delay(5);
  }
}