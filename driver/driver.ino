#define btnExec 10
#define charge 11
#define reverse 2
#define forward 3
#define cruise 4
#define pump 5

void commands() {
  String command = "0";
  if (Serial.available() > 0) {
    command = Serial.readString();
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
  pinMode(btnExec, INPUT);
  digitalWrite(reverse, HIGH);
  digitalWrite(forward, HIGH);

  while (true) {
    buttonState = digitalRead(btnExec);
    if (buttonState != lastButtonState) {

      if (buttonState == false) {
        //Default state i.e button is not pressed.
      } else {
        Serial.println("$getParams");
        Serial.println("{\"origin\":\"Driver\",\"type\":\"log\",\"msg\":\"Get parameters from the server.\",\"importance\":\"Low\"}");

        do {} while (Serial.available() == 0); //Wait for response
        commands();
      }
      delay(50);
    }
    if(digitalRead(charge) == LOW){
      digitalWrite(pump, LOW);
      if(!isCharging) Serial.println("$charging");
      isCharging = true;
    } else {
      if(isCharging) Serial.println("$!charging");
      isCharging = false;
    }
    lastButtonState = buttonState;
  }
}
