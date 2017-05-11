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
bool Radar_move, PIR_move, ACTIVE, IR_obstackle, wake_up;
unsigned long movements_counter;
unsigned long timer, soft_hold = 10000, wake_hold = 6000, long_timer = 300000; //10s, 6s, 5min time
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
{ IR_sensor_check();            // IR distance sensors check door - entrance
  Movement_Sensors_Check();     // handles PIR and Radar sensor - check movement inside the room
  
  if (ACTIVE)
  { digitalWrite(LED, HIGH);            //main LED light ON
    
    if (movements_counter <= 1 && !wake_up)         // <= x numbers of movement sensings
    { if (Radar_move || PIR_move)                   //if flag coming from any sensor
        timer = millis();                           //loop timeout refresh
      if (timer + soft_hold < millis())             //after no movement for soft_hold time - go to wake-up()
        { wake_up = 1;
          timer = millis();                         //refresh timer for wake_up loop
        }
      if(debug) Serial.println(movements_counter);
    }
    
    if (movements_counter >= 2 && !wake_up)         // >= x numbers of movement sensings
    { if (Radar_move || PIR_move)                   //if flag coming from any sensor
        timer = millis();                           //loop timeout refresh
      if (timer + long_timer < millis())            //after 5 min no mevement - go to soft_hold
        movements_counter = 0;
      if(debug) Serial.println(movements_counter);
    }

    if (wake_up)                                    //called after every timeout, lasts for few sec.
    { digitalWrite(LED, LOW);                       //main LED OFF
      if (PIR_move)                                 //if PIR senses movement - go back to work
        { digitalWrite(LED, HIGH);
          wake_up = 0;                              //reset "wake_up" flag to break from this condition
          movements_counter++;                      //go stright to 2nd condition - "long_timer"
        }
      if (timer + wake_hold < millis())             //if the loop time expires - reset all variables and wait for IR
        Variable_Reset();
      if(debug) Serial.println(movements_counter);
    }
  }
}

