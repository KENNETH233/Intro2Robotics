#define PWMA 27
#define DIRA 14
#define PWMB 12
#define DIRB 13
#define IR1 32
#define IR2 33
#define IR3 34
#define IR4 36
#define IR5 39

int ctr = 0;
int num = 0;
int state = 0;
int dir;

void setup() {
  // put your setup code here, to run once:
  pinMode( PWMB, OUTPUT);
  pinMode( DIRB, OUTPUT);
  pinMode( DIRA, OUTPUT);
  pinMode( PWMA, OUTPUT);
  pinMode(23, OUTPUT);
  ledcAttachPin(PWMA, 1);
  ledcAttachPin(PWMB, 2);
  ledcSetup(1, 12000, 8); // 12 kHz PWM, 8-bit resolution
  ledcSetup(2, 12000, 8);
  analogSetWidth(12);
  analogSetPinAttenuation(IR1, ADC_11db);
  analogSetPinAttenuation(IR2, ADC_11db);
  analogSetPinAttenuation(IR3, ADC_11db);
  analogSetPinAttenuation(IR4, ADC_11db);
  analogSetPinAttenuation(IR5, ADC_11db);
  Serial.begin(9600);
}

void motorA ( int speed, int direction) {
  if (direction) {
    digitalWrite(DIRA, HIGH);
    ledcWrite(1, 255 - speed);

    // sigmaDeltaWrite(0, 255 - speed);
  }
  else {
    digitalWrite(DIRA, LOW);
    //sigmaDeltaWrite(0, speed);
    ledcWrite(1, speed);
  }
}

void motorB ( int speed, int direction) {
  if (direction) {
    digitalWrite(DIRB, HIGH);
    //sigmaDeltaWrite(3, 255 - speed);
    ledcWrite(2, 255 - speed);
  }
  else {
    digitalWrite(DIRB, LOW);
    //sigmaDeltaWrite(3, speed);
    ledcWrite(2, speed);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(23, LOW);
  delay(20);
  digitalWrite(23, HIGH);
  delay(10);
  int reading1 = analogRead(IR1);
  int reading2 = analogRead(IR2);
  int reading3 = analogRead(IR3);
  int reading4 = analogRead(IR4);
  int reading5 = analogRead(IR5);
  Serial.println(reading3);
  if (state == 0){
    if (reading1 + reading2 > reading4 + reading5){
      dir = 0;
    } else {
      dir = 1;
    }
    state = 1;
  }
  if (state == 1){
    int reading1 = analogRead(IR1);
    int reading2 = analogRead(IR2);
    int reading3 = analogRead(IR3);
    int reading4 = analogRead(IR4);
    int reading5 = analogRead(IR5);
    if (reading3 > reading1 && reading3 > reading2 && reading3 > reading4 && reading3 > reading5){
      state = 2;
    } else {
      motorA (100, 1-dir);
      motorB (100, dir);
    }
  }
  if (state == 2){
    int reading3 = analogRead(IR3);
    if (reading3 >= 4000){
      state = 3;
    } else {
      int Speed = (5000-reading3)/16;
      if (Speed > 255){
        Speed = 255;
      }
      motorA (Speed, 1);
      motorB (Speed, 1);
    }
  }
}
