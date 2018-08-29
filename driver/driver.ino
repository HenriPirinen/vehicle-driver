#define btnExec 10 //Read
#define charge 11 //Read
#define reverse 2 //Write
#define forward 3 //Write
#define cruise 4 //Write
#define pump 5 //Write
#define serialCharge 6 //Write

void commands(bool charging) {
  String command = "0";
  if (Serial.available() > 0) {
    command = Serial.readString();
    if (!charging) {
      if (command.charAt(0) == command.charAt(1)) {
        digitalWrite(reverse, HIGH);
        digitalWrite(forward, HIGH);
      } else {
        digitalWrite(reverse, command.charAt(0) == '1' ? HIGH : LOW);
        digitalWrite(forward, command.charAt(1) == '1' ? HIGH : LOW);
      }
      digitalWrite(cruise, command.charAt(2) == '1' ? HIGH : LOW);

      String _message = "{\"origin\":\"Driver\",\"type\":\"param\",\"name\":\"driverState\",\"value\":\"" + command + "\",\"importance\":\"Medium\"}";
      Serial.println(_message);
      _message = "{\"origin\":\"Driver\",\"type\":\"log\",\"msg\":\"Set driver parameters: " + command + " \",\"importance\":\"Medium\"}";
      Serial.println(_message);
    } else {
      digitalWrite(serialCharge, command.charAt(2) == '0' ? HIGH : LOW); //SC0 = Stop serial charging, SC1 = Start serial charging (not used)
      String _message = "{\"origin\":\"Driver\",\"type\":\"log\",\"msg\":\"Stopping serial charge\",\"importance\":\"Medium\"}";
      Serial.println(_message);
      _message = "$B1";
      Serial.println(_message); //Send ACK to controller -> Controller can start balance state
    }
  }
}

int main(void) {
  init();
  bool buttonState = true;
  bool lastButtonState = true; //Debounce
  bool isCharging = false;

  Serial.begin(9600);
  pinMode(reverse, OUTPUT);
  pinMode(forward, OUTPUT);
  pinMode(pump, OUTPUT);
  pinMode(serialCharge, OUTPUT);
  pinMode(btnExec, INPUT);
  pinMode(charge, INPUT);
  digitalWrite(reverse, HIGH);
  digitalWrite(forward, HIGH);
  digitalWrite(serialCharge, HIGH);

  while (true) { //Main Loop
    buttonState = digitalRead(btnExec);
    if (buttonState != lastButtonState && !isCharging) {
      if (buttonState == false) {
        //Default state i.e button is not pressed.
      } else {
        Serial.println("$getParams");
        Serial.println("{\"origin\":\"Driver\",\"type\":\"log\",\"msg\":\"Get parameters from the server.\",\"importance\":\"Low\"}");

        do {} while (Serial.available() == 0); //Wait for response
        commands(false);
      }
      delay(50);
    }
    if (isCharging) {
      commands(true);
    }
    if (digitalRead(charge) == LOW) {
      digitalWrite(pump, LOW);
      delay(1000);
      if (digitalRead(charge) == LOW) { //If charge is still LOW, socket is most likely in place.
        if (!isCharging) {
          Serial.println("$charging ");
          digitalWrite(serialCharge, LOW);
        }
        isCharging = true;
      }
    } else {
      if (isCharging) {
        Serial.println("$!charging");
        digitalWrite(serialCharge, HIGH);
      }
      isCharging = false;
    }
    lastButtonState = buttonState;
  }
}
