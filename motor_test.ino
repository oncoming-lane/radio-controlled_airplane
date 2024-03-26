#define pinY2 A2
void setup() {
  pinMode(3, OUTPUT);          // Инициализция портов на вход и выход
  pinMode(pinY2, INPUT);
  Serial.begin(9600);
}

void loop() {

  int Y2 = analogRead(pinY2);
  Y2 = map(Y2, 0, 1023, 0, 255); // функция map переводит один диапазон значений в другой (указанный)
  analogWrite(3, Y2);
  Serial.println(Y2);
  delay(10);
}