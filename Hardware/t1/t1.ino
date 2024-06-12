// defining the pins
const int trigPin1 = 3;
const int echoPin1 = 5;
const int trigPin2 = 6;
const int echoPin2 = 9;
const int trigPin3 = 10;
const int echoPin3 = 11;
int irpin = 2;
int irpin2 = 13;

// defining variables
long duration1;
long duration2;
long duration3;
long distanceleft;
long distancefront;
long distanceright;
int a = 0;
bool manualMode = false;
char command = ' ';
unsigned long lastCommandTime = 0;
const unsigned long manualModeDuration = 3000; // 3 seconds

void setup() {
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin1, INPUT); // Sets the echoPin as an Input
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  pinMode(irpin, INPUT);
  pinMode(irpin2, INPUT);
  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(12, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();
    manualMode = true;
    lastCommandTime = millis(); // Record the time of the last command
  }

  if (manualMode) {
    handleManualMode();
    // Check if 3 seconds have passed since the last command
    if (millis() - lastCommandTime >= manualModeDuration) {
      manualMode = false; // Switch to auto mode
    }
  } else {
    handleAutoMode();
  }
  delay(50);
}

void handleManualMode() {
  switch (command) {
    case 'U': // UP
      digitalWrite(4, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(12, LOW);
      break;
    case 'D': // DOWN
      digitalWrite(4, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
      break;
    case 'L': // LEFT
      digitalWrite(4, HIGH);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, HIGH);
      break;
    case 'R': // RIGHT
      digitalWrite(4, LOW);
      digitalWrite(7, HIGH);
      digitalWrite(8, HIGH);
      digitalWrite(12, LOW);
      break;
    case 'P': // PAUSE
      digitalWrite(4, LOW);
      digitalWrite(7, LOW);
      digitalWrite(8, LOW);
      digitalWrite(12, LOW);
      manualMode = false; // Resume auto mode when paused
      break;
    default:
      // Do nothing for unknown commands
      break;
  }
}

void handleAutoMode() {
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distanceleft = duration1 * 0.034 / 2;
  Serial.print("Distance1: ");
  Serial.println(distanceleft);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
  distancefront = duration2 * 0.034 / 2;
  Serial.print("Distance2: ");
  Serial.println(distancefront);

  digitalWrite(trigPin3, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin3, LOW);
  duration3 = pulseIn(echoPin3, HIGH);
  distanceright = duration3 * 0.034 / 2;
  Serial.print("Distance3: ");
  Serial.println(distanceright);

  int s = digitalRead(irpin);
  Serial.print("Ir: ");
  Serial.println(s);

  if (s == HIGH) { 
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
    delay(1000);
    a = 1;
  }

  if ((a == 0) && (s == LOW) && ((distanceleft <= 20 && distancefront > 20 && distanceright <= 20) || (distanceleft > 20 && distancefront > 20 && distanceright > 20))) {
    digitalWrite(4, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);
  }

  if ((a == 1) && (s == LOW) || (s == LOW) && ((distanceleft <= 20 && distancefront <= 20 && distanceright > 20) || (distanceleft <= 20 && distancefront <= 20 && distanceright > 20) || (distanceleft <= 20 && distancefront > 20 && distanceright > 20) || (distanceleft <= 20 && distancefront > 20 && distanceright > 20))) {
    digitalWrite(4, HIGH);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(12, HIGH);
    delay(100);
    a = 0;
  }

  if ((s == LOW) && ((distanceleft > 20 && distancefront <= 20 && distanceright <= 20) || (distanceleft > 20 && distancefront > 20 && distanceright <= 20) || (distanceleft > 20 && distancefront <= 20 && distanceright > 20))) {
    digitalWrite(4, LOW);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(12, LOW);
  }
}
