
int x=0; //Data to be transmitted! Replace with temperature data
int y=0; //Moisture data to be transmitted
int z=0; //Humidity data to be transmitted

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  x = DHT_readTemp(); //Reads Temperature and Humidity
  y = Moisture_read(); //Reads the moisture reading
  z = DHT_readHumid(); //Reads Humidity

  // Now we can publish stuff!
  // Publish Temperature Data
  Serial.print(F("\nSending temp val "));
  Serial.print(x);
  Serial.print("...");
  if (temp2.publish(x) == 0) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish Moisture Data
  Serial.print(F("\nSending moisture val "));
  Serial.print(y);
  Serial.print("...");
  if (moisture2.publish(y) == 0) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish Humidity Data
  Serial.print(F("\nSending humidity val "));
  Serial.print(z);
  Serial.print("...");
  if (humidity2.publish(z) == 0) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // ping the server to keep the mqtt connection alive
  // NOT required if you are publishing once every KEEPALIVE seconds
  /*
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }
  */
}


/****************************FUNCTION - MQTT_connect*******************************/
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
