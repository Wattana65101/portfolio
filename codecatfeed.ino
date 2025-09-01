#define BLYNK_TEMPLATE_ID "TMPL6Dh4VnpVo"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "h74OMHHbhDrgOmENKXlWcp6hQdUjle4N"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>

char ssid[] = "Nongminewifi";
char pass[] = "wattana15277";

BlynkTimer timer;
Servo foodServo;

int servoPin = D4;

// ตัวแปรสำหรับเวลา
int hour = 0;
int minute = 0;
int second = 0;
unsigned long startTime = 0;  // เวลาเริ่มต้น
unsigned long waitingTime = 0;  // เวลารอ
const int servoOpenDuration = 4000; // เปิดช่องอาหาร 4 วินาที (เวลาให้อาหาร)

// สถานะการให้อาหาร
enum FeedingState {
  IDLE,
  WAITING,
  FEEDING
};

FeedingState currentState = IDLE;

// ควบคุมเซอร์โวจากปุ่มใน Blynk (V4)
BLYNK_WRITE(V4) {
  int servoState = param.asInt();
  Serial.print("V4 Received: ");
  Serial.println(servoState);

  if (servoState == 0) {  // ถ้าค่าเป็น 0 เปิดช่องอาหาร
    openFoodCompartment();
  } else if (servoState == 1) {  // ถ้าค่าเป็น 1 ปิดช่องอาหาร
    closeFoodCompartment();
  }
}

// รับค่าจาก Slider สำหรับชั่วโมง (V1)
BLYNK_WRITE(V1) {
  hour = param.asInt();
  Serial.print("Hour: ");
  Serial.println(hour);
  updateWaitingTime();
}

// รับค่าจาก Slider สำหรับนาที (V2)
BLYNK_WRITE(V2) {
  minute = param.asInt();
  Serial.print("Minute: ");
  Serial.println(minute);
  updateWaitingTime();
}

// รับค่าจาก Slider สำหรับวินาที (V3)
BLYNK_WRITE(V3) {
  second = param.asInt();
  Serial.print("Second: ");
  Serial.println(second);
  updateWaitingTime();
}

void updateWaitingTime() {
  // คำนวณเวลาที่จะรอจากค่าที่ได้รับ (ชั่วโมง, นาที, วินาที)
  waitingTime = (hour * 3600UL + minute * 60 + second) * 1000;
  Serial.print("Waiting Time: ");
  Serial.print(hour);
  Serial.print("h ");
  Serial.print(minute);
  Serial.print("m ");
  Serial.print(second);
  Serial.println("s");

  // เริ่มจับเวลาใหม่
  startTime = millis();
  currentState = WAITING;
}

void openFoodCompartment() {
  Serial.println("เปิดช่องใส่อาหาร (90°)");
  foodServo.write(90);
}

void closeFoodCompartment() {
  Serial.println("ปิดช่องใส่อาหาร (0°)");
  foodServo.write(0);
}

void setup() {
  Serial.begin(115200);

  foodServo.attach(servoPin);
  closeFoodCompartment();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, myTimerEvent);
}

void myTimerEvent() {
  Blynk.virtualWrite(V1, hour);
  Blynk.virtualWrite(V2, minute);
  Blynk.virtualWrite(V3, second);
}

void loop() {
  Blynk.run();
  timer.run();

  // ตรวจสอบเวลาที่ถึงเวลาให้อาหาร (เมื่อเวลารอครบ)
  if (currentState == WAITING && millis() - startTime >= waitingTime) {
    Serial.println("Feeding Time! เปิดช่องอาหาร");
    openFoodCompartment();
    currentState = FEEDING;
    startTime = millis(); // จับเวลาเริ่มให้อาหาร
  }

  // ปิดช่องอาหารหลังจากเปิดครบ 4 วินาที
  if (currentState == FEEDING && millis() - startTime >= servoOpenDuration) {
    Serial.println("ปิดช่องอาหาร");
    closeFoodCompartment();
    currentState = IDLE;  // สถานะกลับเป็น IDLE หลังจากปิดช่องอาหาร
  }
}
