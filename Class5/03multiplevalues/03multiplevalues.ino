void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // opens a serial communication with the host
  pinMode(2, INPUT_PULLUP); // says whether a pin is an INPUT, an OUTPUT, or an INPUT using a PULLUP
  pinMode(3, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  boolean b = !digitalRead(2); // read in the value of pin 2
  boolean t = !digitalRead(3); // read in the value of pin 3
  int k = analogRead(A0); // read in the value of pin A0
  int p = analogRead(A1); // read in the value of pin A1
  Serial.print(b); // send the value of b down the serial line
  Serial.print(" "); // send a space
  Serial.print(t); // send the value of t down the serial line
  Serial.print(" "); // send a space
  Serial.print(k); // send the value of t down the serial line
  Serial.print(" "); // send a space
  Serial.print(p); // send the value of t down the serial line
  Serial.println(); // send a carriage return / line feed
}
