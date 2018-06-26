byte driveDirection = 0;
byte setdriveDirection = 0;
bool buttonState = true;
bool lastButtonState = true; //Debounce
byte reverse = 3; //Dig pin 3
byte drive = 2; //Dig pin 2
byte btnExec = 10; //Dig pin 10

/**
   $getParams via Serial.print = request driver parameters from the server.
   0 = Neutral
   1 = Reverse
   2 = Drive
   99 = getDriverParams
*/

void commands() {
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
}

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  pinMode(drive, OUTPUT);
  pinMode(reverse, OUTPUT);
  pinMode(btnExec, INPUT);
}

void loop() {
  commands(); //Read serial on regular loops (requests / commands)
  buttonState = digitalRead(btnExec);
  if (buttonState != lastButtonState) {

    if (buttonState == false) {
      //Default state eg. button is not pressed.
    } else {
      Serial.println("$getParams");
      Serial.println("{\"type\":\"log\",\"msg\":\"Get parameters from the server.\",\"importance\":\"Low\"}");

      do {
        digitalWrite(13, HIGH); //Wait for response
      } while (Serial.available() == 0);
      digitalWrite(13, LOW);
      commands(); //Get response from the server

      if (setdriveDirection != driveDirection) {
        String _message = "{\"type\":\"log\",\"msg\":\"Set driver parameters: driveDirection: " + String(setdriveDirection) + " => " + String(driveDirection) + " \",\"importance\":\"Medium\"}";
        setdriveDirection = driveDirection;
        Serial.println(_message);
      }
    }
    delay(50);
  }
  lastButtonState = buttonState;


  switch (setdriveDirection) {
    case 0: //Neutral
      digitalWrite(drive, HIGH);
      digitalWrite(reverse, HIGH);
      break;
    case 1: //Reverse
      digitalWrite(drive, HIGH);
      digitalWrite(reverse, LOW);
      break;
    case 2: //Drive
      digitalWrite(drive, LOW);
      digitalWrite(reverse, HIGH);
      break;
    default:
      Serial.println("{\"type\":\"log\",\"msg\":\"Invalid drive direction!\",\"importance\":\"High\"}");
  }
}
