// 버튼이 연결된 핀 번호
const int buttonPin = 2; 
// LED 또는 다른 동작을 제어할 핀 번호
const int actionPin = 13; 

// 버튼의 상태와 마지막 상태를 저장할 변수
int buttonState;             
int lastButtonState = LOW;   

// 눌린 횟수, 마지막으로 체크한 시간, 버튼이 마지막으로 눌린 시간을 저장할 변수
unsigned long lastDebounceTime = 0;  
unsigned long debounceDelay = 50;    
unsigned long lastPressTime = 0;     
const unsigned long pressResetTime = 500; // pressCount 초기화 시간
int pressCount = 0;                   

// 꾹 누르기를 감지하기 위한 변수
unsigned long pressStartTime = 0; // 버튼이 눌리기 시작한 시간
const unsigned long longPressTime = 5000; // 꾹 누르기로 간주할 시간 (예: 1000ms = 1초)
bool isPressing = false; // 현재 버튼이 눌리고 있는지 상태

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(actionPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int reading = digitalRead(buttonPin);

  // 버튼 상태가 마지막 상태와 다를 경우, 반동(debounce)을 처리
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // 버튼을 누른 상태가 안정적인 경우
    if (reading != buttonState) {
      buttonState = reading;

      // 버튼이 눌렸다면 (HIGH에서 LOW로 변경)
      if (buttonState == LOW) {
        pressCount++;
        lastPressTime = millis(); // 마지막으로 눌린 시간 갱신
        isPressing = true;
        pressStartTime = millis(); // 꾹 누르기 시작 시간 기록
        // Serial.print("Button pressed ");
        // Serial.println(pressCount);
      } else {
        isPressing = false;
      }
    }
  }

  // 꾹 누르기 감지
  if (isPressing && (millis() - pressStartTime > longPressTime)) {
    Serial.println("꾹 눌러졌음");
    // 꾹 누르기에 대한 동작을 여기에 추가
    digitalWrite(actionPin, HIGH); // 예시로 LED 켜기
    delay(1000); // 잠시 대기
    digitalWrite(actionPin, LOW); // LED 끄기

    isPressing = false; // 재감지 방지를 위해 상태 초기화
    pressCount = 0; // pressCount도 초기화
  }

  // 마지막으로 버튼이 눌린 시간으로부터 500ms가 지났는지 확인
  if (millis() - lastPressTime > pressResetTime && pressCount > 0) {
    // pressCount 초기화
    Serial.println("Press count reset due to timeout");
    // 버튼이 한 번 눌렸다면 원하는 동작 수행
    if (pressCount == 1 && !isPressing) { // 버튼 누르기가 안정적으로 감지되고, 꾹 누르기가 아닐 때
      Serial.println("한 번 눌렀습니다.");
      // 버튼 누르기를 처리한 후 pressCount 초기화
      
    } else if (pressCount == 2 && !isPressing) {
      Serial.println("두 번 눌렀습니다.");
    }
    pressCount = 0;
  }

  // 버튼이 3번 눌렸다면 원하는 동작 수행
  if (pressCount == 3) {
    Serial.println("따다닥");
    digitalWrite(actionPin, HIGH); // 예시로 LED 켜기

    // 잠시 기다린 후 LED를 끕니다.
    delay(1000); // 1초 대기
    digitalWrite(actionPin, LOW); // LED 끄기

    // pressCount 초기화
    pressCount = 0;
  }

  // 마지막 버튼 상태 저장
  lastButtonState = reading;
}
