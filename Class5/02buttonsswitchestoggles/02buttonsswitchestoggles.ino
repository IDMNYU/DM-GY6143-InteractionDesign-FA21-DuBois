void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // opens a serial communication with the host
  pinMode(2, INPUT_PULLUP); // says whether a pin is an INPUT, an OUTPUT, or an INPUT using a PULLUP
  pinMode(3, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  boolean b = !digitalRead(2); // read in the value of pin 2
  boolean t = !digitalRead(3); // read in the value of pin 2
  Serial.println(t); // send the value of k down the serial line
}
