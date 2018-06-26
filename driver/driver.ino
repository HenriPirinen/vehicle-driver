int driveDirection = 0;
int setdriveDirection = 0;
int buttonState = 1;
int lastButtonState = 1; //Debounce

/**
   0 = Neutral
   1 = Reverse
   2 = Drive
   99 = getDriverParameters
*/

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
}

void loop() {
  int command = 0;
  if (Serial.available() > 0) {
    command = Serial.parseInt();

    switch (command) {
      case 0: {
          driveDirection = 0;
          break;
        }
      case 1: {
          driveDirection = 1;
          break;
        }
      case 2: {
          driveDirection = 2;
          break;
        }
      case 99: {
          String _settings = "{\"type\":\"param\",\"direction\": " + String(driveDirection) + ",\"importance\":\"Low\"}";
          Serial.println(_settings);
          break;
        }
      default: {
          Serial.println("{\"type\":\"log\",\"msg\":\"Invalid command!\",\"importance\":\"High\"}");
        }
    }
  }

  buttonState = digitalRead(7);

  if (buttonState != lastButtonState) {

    if (buttonState == 0) {
    } else {
      if (setdriveDirection != driveDirection) {
        String _message = "{\"type\":\"log\",\"msg\":\"Set driver parameters: driveDirection: " + String(setdriveDirection) + " => " + String(driveDirection) + " \",\"importance\":\"Medium\"}";
        setdriveDirection = driveDirection;
        Serial.println(_message);
      } else {
        Serial.println("{\"type\":\"log\",\"msg\":\"Set driver parameters, no changes.\",\"importance\":\"Low\"}");
      }
    }
    delay(50);
  }
  lastButtonState = buttonState;


  switch (setdriveDirection) {
    case 0: //Neutral
      digitalWrite(5, LOW);
      digitalWrite(6, LOW);
      break;
    case 1: //Reverse
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
      break;
    case 2: //Drive
      digitalWrite(5, HIGH);
      digitalWrite(6, LOW);
      break;
    default:
      Serial.println("{\"type\":\"log\",\"msg\":\"Invalid drive direction!\",\"importance\":\"High\"}");
  }
}
