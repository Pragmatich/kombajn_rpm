#include <LiquidCrystal_I2C.h>

int lcdColumns = 16;
int lcdRows = 2;

LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows);

const int ledPin = 5;
const int buzzerPin = 25;
const int mikro1 = 32;
const int mikro2 = 33;

static uint32_t  lastMillis           = 0;
static bool      alarmRpm             = false; 
static uint32_t  tolerancePercentage  = 10;
 
struct Hall {
  const uint8_t PIN;
  uint32_t numberSenses;
  uint32_t numberTrigger;
  uint32_t maxRpm;
  uint32_t rpm;
};

Hall hallUtocseplo =      {18, 0, 3, 0, 0};
Hall hallRazoasztal =     {19, 0, 3, 0, 0};
Hall hallDob =            {26, 0, 4, 0, 0};
Hall hallFerdefelhordo =  {27, 0, 2, 0, 0};

byte leftArrow[] = {
  B00000,
  B00010,
  B00110,
  B01010,
  B11011,
  B01010,
  B00110,
  B00010
};

byte rightArrow[] = {
  B00000,
  B01000,
  B01100,
  B01010,
  B11011,
  B01010,
  B01100,
  B01000
};

void IRAM_ATTR isr1() {
  hallUtocseplo.numberSenses = hallUtocseplo.numberSenses + 1 ;
}

void IRAM_ATTR isr2() {
  hallRazoasztal.numberSenses = hallRazoasztal.numberSenses + 1 ;
}

void IRAM_ATTR isr3() {
  hallDob.numberSenses = hallDob.numberSenses + 1 ;
}

void IRAM_ATTR isr4() {
  hallFerdefelhordo.numberSenses = hallFerdefelhordo.numberSenses + 1 ;
}

void setup() {
  lcd.begin();
  lcd.backlight();
  
  pinMode(hallUtocseplo.PIN, INPUT_PULLUP);
  attachInterrupt(hallUtocseplo.PIN, isr1, RISING);

  pinMode(hallRazoasztal.PIN, INPUT_PULLUP);
  attachInterrupt(hallRazoasztal.PIN, isr2, RISING);

  pinMode(hallDob.PIN, INPUT_PULLUP);
  attachInterrupt(hallDob.PIN, isr3, RISING);

  pinMode(hallFerdefelhordo.PIN, INPUT_PULLUP);
  attachInterrupt(hallFerdefelhordo.PIN, isr4, RISING);

  pinMode(mikro1, INPUT_PULLUP);
  pinMode(mikro2, INPUT_PULLUP);
  
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.createChar(0, leftArrow);
  lcd.createChar(1, rightArrow);

/*
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fortschritt E516");
  lcd.setCursor(3, 1);
  lcd.print("RPM: 1/min");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Utocseplo");
  delay(1000);
  lcd.clear();
  lcd.setCursor(6, 0);
  lcd.print("Szalmarazo");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Cseplodob");
  delay(1000);
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Magelevator");
  delay(1000);
*/
}


void loop() {
  int miro1_Val = digitalRead(mikro1);
  int miro2_Val = digitalRead(mikro2);
    
  if (millis() - lastMillis > 1000) {
    lastMillis = millis();
    lcd.clear();
    alarmRpm = false;

    hallUtocseplo.rpm = hallUtocseplo.numberSenses * 60 / hallUtocseplo.numberTrigger;
    if (hallUtocseplo.rpm > hallUtocseplo.maxRpm)
    hallUtocseplo.maxRpm = hallUtocseplo.rpm;
    if (hallUtocseplo.rpm < hallUtocseplo.maxRpm * (1 - (tolerancePercentage / 100))) {
    alarmRpm = true;
    lcd.setCursor(7, 0);
    lcd.write(0);
    };
    lcd.setCursor(0, 0);
    lcd.print("UT");
    lcd.setCursor(3, 0);
    lcd.print(hallUtocseplo.rpm);
    hallUtocseplo.numberSenses = 0;

    hallRazoasztal.rpm = hallRazoasztal.numberSenses * 60 / hallRazoasztal.numberTrigger;
    if (hallRazoasztal.rpm > hallRazoasztal.maxRpm)
    hallRazoasztal.maxRpm = hallRazoasztal.rpm;
    if (hallRazoasztal.rpm <hallRazoasztal.maxRpm * (1 - (tolerancePercentage / 100))) {
    alarmRpm = true;
    lcd.setCursor(8, 0);
    lcd.write(1);
    };
    lcd.setCursor(9, 0);
    lcd.print("SZ");
    lcd.setCursor(12, 0);
    lcd.print(hallRazoasztal.numberSenses * 60 / hallRazoasztal.numberTrigger);
    hallRazoasztal.numberSenses = 0;

    hallDob.rpm = hallDob.numberSenses * 60 / hallDob.numberTrigger;
    if (hallDob.rpm > hallDob.maxRpm)
    hallDob.maxRpm = hallDob.rpm;
    if (hallDob.rpm < hallDob.maxRpm * (1 - (tolerancePercentage / 100))) {
    alarmRpm = true;
    lcd.setCursor(7, 1);
    lcd.write(0);
    };
    lcd.setCursor(0, 1);
    lcd.print("CS");
    lcd.setCursor(3, 1);
    lcd.print(hallDob.numberSenses * 60 / hallDob.numberTrigger);
    hallDob.numberSenses = 0;

    hallFerdefelhordo.rpm = hallFerdefelhordo.numberSenses * 60 / hallFerdefelhordo.numberTrigger;
    if (hallFerdefelhordo.rpm > hallFerdefelhordo.maxRpm)
    hallFerdefelhordo.maxRpm = hallFerdefelhordo.rpm;
    if (hallFerdefelhordo.rpm < hallFerdefelhordo.maxRpm * (1 - (tolerancePercentage / 100))) {
    alarmRpm = true;
    lcd.setCursor(8, 1);
    lcd.write(1);
    };
    lcd.setCursor(9, 1);
    lcd.print("MA");
    lcd.setCursor(12, 1);
    lcd.print(hallFerdefelhordo.numberSenses * 60 / hallFerdefelhordo.numberTrigger);
    hallFerdefelhordo.numberSenses = 0;

    digitalWrite (ledPin, LOW);

    if (alarmRpm && !miro1_Val && !miro2_Val)
      digitalWrite (buzzerPin, HIGH);
    else
      digitalWrite (buzzerPin, LOW);
  }

  if (millis() - lastMillis > 500 && millis() - lastMillis < 505 && alarmRpm && miro1_Val && miro2_Val)
    digitalWrite (ledPin, HIGH);
}
