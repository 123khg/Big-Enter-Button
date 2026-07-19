#include <BleKeyboard.h>
#include <USB.h>
#include <USBHIDKeyboard.h>


BleKeyboard bleKeyboard("Nut bam vi dai");
USBHIDKeyboard usbKeyboard;


const int BUTTON_PIN = 4; 


int buttonState = HIGH;          
int lastReading = HIGH;     
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 150;
unsigned long lastRepeatTime = 0; 
const int repeatSpeed = 200;
bool computer= false; 
int reading;

void setup() {
  Serial.begin(115200);
  // Không đấm = HIGH, Bị đấm = LOW
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.println("Khoi dong he thong...");
  bleKeyboard.begin();
  USB.begin();
  usbKeyboard.begin();
  Serial.println("dam duoc roi");
}

void loop() {
  reading = digitalRead(BUTTON_PIN);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading; 
    }
    /*KEY_DELETE: Phím Delete
      KEY_BACKSPACE: Phím xóa lùi (Backspace)
      'a'
      '5'
      ' '
    */
    if (buttonState == LOW) { 
      if (!computer) {
        if (bleKeyboard.isConnected()) {
          bleKeyboard.write(KEY_RETURN);
        } 
        else {
          usbKeyboard.write(KEY_RETURN);
        }
      computer = true; 
      lastRepeatTime = millis();      
      Serial.println("Xuong dong!");
      }
      else {
        if ((millis() - lastRepeatTime) > repeatSpeed) {
          if (bleKeyboard.isConnected()) {
          bleKeyboard.write(KEY_RETURN); 
        } 
        else {
          usbKeyboard.write(KEY_RETURN);
        }
        lastRepeatTime = millis();
        Serial.println("Xuong dong...");
        }
      }
    } 
    else {
      if (computer) {
        computer = false;
        Serial.println("Ngung");
      }
    }
  }
  lastReading = reading;
}