/****************************Read Sensor Functions*******************************/

int DHT_readTemp(){
  
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  //if (isnan(h) || isnan(t) || isnan(f)) {
    //Serial.println("Failed to read from DHT sensor!");
    //return;
 // }

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  return t;
}

int DHT_readHumid(){
  
  // Wait a few seconds between measurements.
  delay(2000);
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h = dht.readHumidity();
  
  return h;
}

int Moisture_read(){

  delay(2000);

  int sensorValue = analogRead(A0);
  int val = sensorValue;
  
//  if(sensorValue > thresholdValue){
//    Serial.println(" - Doesn't need watering");
//  }
//  else {
//    Serial.println(" - Time to water your plant");
//  }

  return val;
}
