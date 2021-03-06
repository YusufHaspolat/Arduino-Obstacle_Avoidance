#include <Wire.h>
#include "DualVNH5019MotorShield.h"
DualVNH5019MotorShield md(11, 12, 13, 6, A0, 8, 9, 3, 12, A1);

// Define Slave I2C Address
#define SLAVE_ADDR 9

// Define array for return data
int main_buffer[5];

const int button_pin = 7;
static int button_read = 0;
static int cnt = 0;

static int front_flag = 0;
static int right_flag = 0;
static int left_flag = 0;
static int right_corn_flag = 0;
static int left_corn_flag = 0;
static int trap_flag_r = 0;
static int trap_flag_l = 0;
static int left_speed;
static int right_speed;

static int data_numb = 0;
static char sensor;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  md.init();
  pinMode(button_pin , INPUT);
}

void sensor_reading()
{
  Wire.requestFrom(SLAVE_ADDR, 1);

  if(Wire.available())
  {
    sensor = Wire.read();
    // Set array for coming data by checking received letter from slave
    if(sensor == 'a')
    {
      data_numb = 0;
    }
    else if(sensor == 'b')
    {
      data_numb = 1;
    }
    else if(sensor == 'c')
    {
      data_numb = 2;
    }
    else if(sensor == 'd')
    {
      data_numb = 3;
    }
    else if(sensor == 'e')
    {
      data_numb = 4;
    }
    
    else
    {
      main_buffer[data_numb] = sensor;
      // Checking data if it exceeds 250
      if(main_buffer[data_numb] < 0)
      {
        main_buffer[data_numb] = main_buffer[data_numb] * -1;
        main_buffer[data_numb] = main_buffer[data_numb] + 250;
      }
    }
  }

  // Read the measurements of sensor from left to right
  Serial.print(main_buffer[2]);
  Serial.print("\t");
  Serial.print(main_buffer[4]);
  Serial.print("\t");
  Serial.print(main_buffer[0]);
  Serial.print("\t");
  Serial.print(main_buffer[3]);
  Serial.print("\t");
  Serial.print(main_buffer[1]);
  Serial.println();
}

void loop()
{
  sensor_reading();
  // Both motors run forward
  left_speed = 300;
  right_speed = -300;

  if(front_flag)
  {
  	// Choose a side to turn against the obstacle ahead
    if (main_buffer[2] < main_buffer[1])
    {
      left_flag = 1;
    }
    else
    {
      right_flag = 1;
    }
  }
  // An obstacle ahead
  if (main_buffer[0] < 50 || main_buffer[3] < 50 || main_buffer[4] < 50)
  {
    front_flag = 1;
  }
  // Obstacles on both corners -you're stuck-
  else if (main_buffer[1] < 30 && main_buffer[2] < 30)
  {
  	// Determine which side to turn
    if(main_buffer[2] < main_buffer[1])
    {
      trap_flag_r = 1;
    }
    else
    {
      trap_flag_l = 1;
    }
  }
  // Obstacle on right corner
  else if (main_buffer[1] < 40)
  {
    right_corn_flag = 1;
  }
  // Obstacle on left corner
  else if (main_buffer[2] < 40)
  {
    left_corn_flag = 1;
  }
  else 
  {
    // The obstacle ahead is gone
    if (front_flag == 1 && main_buffer[0] > 50 && main_buffer[3] > 50 && main_buffer[4] > 50)
    {
      front_flag = 0;
    }
  }
  // Turn untill the obstacle is gone "between the lines 146-205"
  if (right_flag)
  {
    left_speed = 300;
    right_speed = 300;
    left_flag = 0;
    if (main_buffer[0] > 50 && main_buffer[3] > 50 && main_buffer[4] > 50)
    {
      right_flag = 0;
    }
  }
  if (left_flag)
  {
    left_speed = -300;
    right_speed = -300;
    right_flag = 0;
    if (main_buffer[0] > 50 && main_buffer[3] > 50 && main_buffer[4] > 50)
    {
      left_flag = 0;
    }
  }
  if (right_corn_flag)
  {
    left_speed = 300;
    right_speed = 300;
    left_corn_flag = 0;
    if (main_buffer[1] > 55)
    {
      right_corn_flag = 0;
    }
  }
  if (left_corn_flag)
  {
    left_speed = -300;
    right_speed = -300;
    right_corn_flag = 0;
    if (main_buffer[2] > 55)
    {
      left_corn_flag = 0;
    }
  }
  if (trap_flag_r)
  {
    left_speed = 300;
    right_speed = 300;
    trap_flag_l = 0;
    if (main_buffer[0] > 50 && main_buffer[1] > 50 && main_buffer[3] > 50 && main_buffer[4] > 50)
    {
      trap_flag_r = 0;
    }
  }
  if (trap_flag_l)
  {
    left_speed = -300;
    right_speed = -300;
    trap_flag_r = 0;
    if (main_buffer[0] > 50 && main_buffer[2] > 50 && main_buffer[3] > 50 && main_buffer[4] > 50)
    {
      trap_flag_l = 0;
    }
  }
  // A button control to run and stop motors
  button_read = digitalRead(button_pin);
  if(button_read == HIGH)
  {
    cnt++;
    delay(500);
  }
    if(cnt % 2 == 0)
    {
      md.setM1Speed(0);
      md.setM2Speed(0);
    }
    else
    {
      md.setM1Speed(right_speed);
      md.setM2Speed(left_speed);
    }
}
