
float x=0; //Data to be transmitted! Replace with temperature data
float y=0; //Moisture data to be transmitted
float z=0; //Humidity data to be transmitted

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  x = DHT_readTemp(); //Reads Temperature and Humidity
  y = Moisture_read(); //Reads the moisture reading
  z = DHT_readHumid(); //Reads Humidity

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &temp_out) {
      Serial.print(F("Got: "));
      Serial.println((char *)temp_out.lastread);
    }  
  }

  // Now we can publish stuff!
  // Publish Temperature Data
  Serial.print(F("\nSending temp val "));
  Serial.print(x);
  Serial.print("...");
  if (temp.publish(x) == 0) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish Moisture Data
  Serial.print(F("\nSending moisture val "));
  Serial.print(y);
  Serial.print("...");
  if (moisture.publish(y) == 0) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }

  // Publish Humidity Data
  Serial.print(F("\nSending humidity val "));
  Serial.print(z);
  Serial.print("...");
  if (humidity.publish(z) == 0) {
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


/****************************FUNCTION - DHT_read **********************************/

float DHT_readTemp(){
  
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    //return;
 // }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");

  return t;
}

float DHT_readHumid(){
  
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  
  return h;
}

float Moisture_read(){

  delay(2000);

  int sensorValue = analogRead(rainPin);
  float val = sensorValue;
  
  Serial.print(val);
  
  if(sensorValue > thresholdValue){
    Serial.println(" - Doesn't need watering");
  }
  else {
    Serial.println(" - Time to water your plant");
  }

  return val;
}

