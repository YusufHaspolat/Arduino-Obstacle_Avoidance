#include <Wire.h>

// Define Slave I2C Address
#define SLAVE_ADDR 9

// Define sensor pins

// Sensor 0
const int TRIGGER_PIN_0 = 24;
const int ECHO_PIN_0 = 25;

// Sensor 1
const int TRIGGER_PIN_1 = 26;
const int ECHO_PIN_1 = 27;

// Sensor 2
const int TRIGGER_PIN_2 = 28;
const int ECHO_PIN_2 = 29;

// Sensor 3
const int TRIGGER_PIN_3 = 30;
const int ECHO_PIN_3 = 31;

// Sensor 4
const int TRIGGER_PIN_4 = 32;
const int ECHO_PIN_4 = 33;

// Define return data array, one element per sensor
int distance[5];

int sure_0;
int sure_1;
int sure_2;
int sure_3;
int sure_4;

static int data_numb = 0;
static int flag = 0;

int data_val;
int extra_val;

void setup() {

  pinMode(TRIGGER_PIN_0, OUTPUT);
  pinMode(ECHO_PIN_0, INPUT);
  pinMode(TRIGGER_PIN_1, OUTPUT);
  pinMode(ECHO_PIN_1, INPUT);
  pinMode(TRIGGER_PIN_2, OUTPUT);
  pinMode(ECHO_PIN_2, INPUT);
  pinMode(TRIGGER_PIN_3, OUTPUT);
  pinMode(ECHO_PIN_3, INPUT);
  pinMode(TRIGGER_PIN_4, OUTPUT);
  pinMode(ECHO_PIN_4, INPUT);
  
  // Initialize I2C communications as Slave
  Wire.begin(SLAVE_ADDR);
   // Function to run when data requested from master
  Wire.onRequest(requestEvent);
  
}

void requestEvent()
{
  data_val = distance[data_numb];
  
  /* Before sending data to master,
  send a specific letter to master to indicate which sensor is going to send data */
  
  if(flag == 0)
  {
    if(data_numb == 0)
    {
      Wire.write('a');
    }

    else if(data_numb == 1)
    {
      Wire.write('b');
    }

    else if(data_numb == 2)
    {
      Wire.write('c');
    }

    else if(data_numb == 3)
    {
      Wire.write('d');
    }

    else if(data_numb == 4)
    {
      Wire.write('e');
    }
    
    flag = 1;
  }

  else if(flag == 1)
  {
  	/* Values over 255 cannot be sent through I2C. 
	So if a distance exceeds 250, send the exceeding amount by multiplying with -1. 
	This way, master will detect the situation. */
	
    if(data_val >= 250)
    {
      extra_val = data_val - 250;
      extra_val = extra_val * -1;
    
      Wire.write(extra_val);
    }

    else
    {
      Wire.write(data_val);
    }
  
    data_numb++;

    if(data_numb == 5)
    {
      data_numb = 0;
    }

    flag = 0;
  }

}

void readDistance()
{
  // Sensors measure distances one by one
  
  digitalWrite(TRIGGER_PIN_0, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN_0, LOW);
  sure_0 = pulseIn(ECHO_PIN_0, HIGH);
  distance[0] = (sure_0/2) / 29.1;
  
  digitalWrite(TRIGGER_PIN_1, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN_1, LOW);
  sure_1 = pulseIn(ECHO_PIN_1, HIGH);
  distance[1] = (sure_1/2) / 29.1;
  
  digitalWrite(TRIGGER_PIN_2, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN_2, LOW);
  sure_2 = pulseIn(ECHO_PIN_2, HIGH);
  distance[2] = (sure_2/2) / 29.1;

  digitalWrite(TRIGGER_PIN_3, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN_3, LOW);
  sure_3 = pulseIn(ECHO_PIN_3, HIGH);
  distance[3] = (sure_3/2) / 29.1;
  
  digitalWrite(TRIGGER_PIN_4, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGGER_PIN_4, LOW);
  sure_4 = pulseIn(ECHO_PIN_4, HIGH);
  distance[4] = (sure_4/2) / 29.1;

}

void loop()
{
  // Refresh readings continuesly
  readDistance();
  
}
