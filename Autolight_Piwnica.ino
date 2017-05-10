#define radar A1              //czujnik radarowy
#define IR_1 A3               //górny goldpin bliżej micro USB
#define IR_2 A0               //
#define PIR A2                //dolny goldpin bliżej dławika
#define LED A4                //main output
#define manual_switch A5      //mini przełącznik manualny 
#define pir_led 9             //PIR indicator led
#define radar_led 10          //radar indicator led
#define IR_led 13             //IR indicator led

bool debug = 0;        //debug mode on/off (serial port)
bool R_move2, PIR_move, active, IR_obstackle, wake_up;
unsigned long movements_counter;
unsigned long timer, soft_hold = 10000, wake_hold = 5500, long_timer = 300000; //10s, 5,5s, 5min time
byte trigger1 = 155, trigger2 = 160;   // IR sensors triggering distance

void setup()
{ if(debug) Serial.begin(38400);
  pinMode(LED, OUTPUT);
  pinMode(IR_led, OUTPUT);
  pinMode(pir_led, OUTPUT);
  pinMode(radar_led, OUTPUT);
  
  Switch_check();         //if set - don't continue to "loop"
  PowerUP();              //if going to normal work mode - do PowerUP sequence
}

void loop()
{ IR_sensor_check();      //turns active to 1 if IR senses something
    
  while (active)
  { digitalWrite(LED, HIGH);            //main LED light ON (2nd time for sure)
    timer = millis();
    
    while (movements_counter <= 1 && !wake_up)      // <= x numbers of radar sensings
    { Movement_Sensors_Check();
      if (R_move2 || PIR_move)                      //if flag coming from any sensor
        timer = millis();                           //loop timeout refresh
      IR_sensor_check();                            //check IR sensor, if it senses something then break this loop
      if (timer + soft_hold < millis())             //after no movement for soft_hold time - break from loop
        { wake_up = 1;                              //go next to wake_up loop
          timer = millis();                         //refresh timer for wake_up loop
          break;
        }
      if(debug) Serial.println(movements_counter);
    }
    
    while (movements_counter >= 2 && !wake_up)      // >= x numbers of radar sensings
    { Movement_Sensors_Check();
      if (R_move2 || PIR_move)                      //if flag coming from any sensor
        timer = millis();                           //loop timeout refresh
      IR_sensor_check();                            //check IR sensor, if it senses something then break this loop
      if (timer + long_timer < millis())            //after 5 min break from loop and check movement
        { movements_counter = 0;
          break;
        }
      if(debug) Serial.println(movements_counter);
    }

    while (wake_up)                          //called after every timeout, lasts for few sec.
    { digitalWrite(LED, LOW);                //main LED OFF
      IR_sensor_check();                     //check IR sensor, if it senses something then break this loop
      Movement_Sensors_Check();
      if (PIR_move)                          //if any movement happens - go back to work
        { digitalWrite(LED, HIGH);
          wake_up = 0;                       //reset "wake_up" flag to break from this loop
          movements_counter++;
        }
      if (timer + wake_hold < millis())      //if the loop time expires, turn all OFF and wait for IR
        { digitalWrite(LED, LOW);
          Variable_Reset();
        }
      if(debug) Serial.println(movements_counter);
    }
  }
}

