# Example of using the MQTT client class to subscribe to a feed and print out
# any changes made to the feed.  Edit the variables below to configure the key,
# username, and feed to subscribe to for changes.

# Import standard python modules.
import sys
import RPi.GPIO as GPIO
import time

# Import Adafruit IO MQTT client.
from Adafruit_IO import MQTTClient

# Set to your Adafruit IO key.
ADAFRUIT_IO_KEY = 'e12cd025715b4fd6bca5aff2e7be447c'

# Set to your Adafruit IO username.
ADAFRUIT_IO_USERNAME = 'gabrielgreen'

# Set to the ID of the feed to subscribe to for updates.
FEED_ID1 = 'moisture'
FEED_ID2 = 'moisture2'

#Set up GPIO pins and set threshold value
GPIO.setmode(GPIO.BCM)
GPIO.setup(2, GPIO.OUT)
GPIO.setup(3, GPIO.OUT)
GPIO.setup(4, GPIO.OUT)
global threshold
threshold = 425

#Turn off everything upon startup
GPIO.output(2, GPIO.HIGH) #Turn off Valve 1
GPIO.output(3, GPIO.HIGH) #Turn off Valve 2
GPIO.output(4, GPIO.LOW) #Turn off Pump

# Functions for toggle of watering system
def update_signal_valve(wetness):
    if int(wetness) <= threshold:
        GPIO.output(2, GPIO.LOW) #Activate Valve 1
        GPIO.output(4, GPIO.HIGH) #Activate Pump
    else:
        GPIO.output(2, GPIO.HIGH) #Turn off Valve 1
        GPIO.output(4, GPIO.LOW) #Turn off Pump
def update_signal_valve2(wetness):
    if int(wetness) <= threshold:
        GPIO.output(3, GPIO.LOW) #Active Valve 2
        GPIO.output(4, GPIO.HIGH) #Activate Pump
    else:
        GPIO.output(3, GPIO.HIGH) #Turn off Valve 2
        GPIO.output(4, GPIO.LOW) #Turn off Pump
        
# Define callback functions which will be called when certain events happen.
def connected(client):
    # Connected function will be called when the client is connected to Adafruit IO.
    # This is a good place to subscribe to feed changes.  The client parameter
    # passed to this function is the Adafruit IO MQTT client so you can make
    # calls against it easily.
    print('Connected to Adafruit IO!  Listening for {0} changes...'.format(FEED_ID1))
    # Subscribe to changes on a feed named DemoFeed.
    client.subscribe(FEED_ID1)

    print('Listening for {0} changes... '.format(FEED_ID2))
    client.subscribe(FEED_ID2)

def disconnected(client):
    # Disconnected function will be called when the client disconnects.
    print('Disconnected from Adafruit IO!')
    sys.exit(1)

def message(client, feed_id, payload):  #retain parameter removed
    # Message function will be called when a subscribed feed has a new value.
    # The feed_id parameter identifies the feed, and the payload parameter has
    # the new value.
    print('Feed {0} received new value: {1}'.format(feed_id, payload))
    if feed_id == FEED_ID1:
        moisture = payload
        update_signal_valve(moisture)
    if feed_id == FEED_ID2:
       moisture2 = payload
       update_signal_valve2(moisture2)

# Create an MQTT client instance.
client = MQTTClient(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

# Setup the callback functions defined above.
client.on_connect    = connected
client.on_disconnect = disconnected
client.on_message    = message

# Connect to the Adafruit IO server.
client.connect()

# Start a message loop that blocks forever waiting for MQTT messages to be
# received.  Note there are other options for running the event loop like doing
# so in a background thread--see the mqtt_client.py example to learn more.
client.loop_blocking()
