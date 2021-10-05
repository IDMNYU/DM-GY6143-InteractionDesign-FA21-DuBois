void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // opens a serial communication with the host
}

void loop() {
  // put your main code here, to run repeatedly:
  int k = analogRead(A0); // read in the value of pin A0
  Serial.println(k); // send the value of k down the serial line
}
