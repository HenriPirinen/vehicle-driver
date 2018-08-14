void commands() {
  String command = "0";
  if (Serial.available() > 0) {
    command = Serial.readString();
    if (command.charAt(0) == command.charAt(1)) {
      digitalWrite(2, HIGH);
      digitalWrite(3, HIGH);
    } else {
      digitalWrite(2, command.charAt(0) == '1' ? HIGH : LOW);
      digitalWrite(3, command.charAt(1) == '1' ? HIGH : LOW);
    }
    digitalWrite(4, command.charAt(2) == '1' ? HIGH : LOW);
    digitalWrite(5, command.charAt(3) == '1' ? HIGH : LOW);

    String _message = "{\"origin\":\"Driver\",\"type\":\"param\",\"value\":\"" + command + "\",\"importance\":\"Medium\"}";
    Serial.println(_message);
  }
}

int main(void) {
  init();
  bool buttonState = true;
  bool lastButtonState = true; //Debounce
  byte btnExec = 10; //Dig pin 10

  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(btnExec, INPUT);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);

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
    lastButtonState = buttonState;
  }
}
