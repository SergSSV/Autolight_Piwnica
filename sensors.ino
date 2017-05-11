void IR_sensor_check()
{ static byte counter;
  static int analog1, analog2, distance1, distance2; 
  if (counter < 2)
  { counter++;
    analog1 += analogRead(IR_1);
    if (counter == 2)
    { distance1 = analog1 / 2;
      analog1 = 0;
    }
  }
  if (counter >= 2)
  { counter++;
    analog2 += analogRead(IR_2);
    if (counter == 4)
    { distance2 = analog2 / 2;
      analog2 = 0;
      counter = 0;
    }
  }
  if (distance1 >= trigger1 || distance2 >= trigger2)  
  { ACTIVE = 1;
    timer = millis();
    movements_counter = 0;
    wake_up = 0;    
    Radar_move = 0;
    digitalWrite(IR_led, HIGH);      //shows sensor work
    analogWrite(radar_led, 0);       // radar reset
  }
  else
    digitalWrite(IR_led, LOW);     
    
  if(debug) { Serial.print("IR_1 dist.: "); Serial.println(distance1); 
              Serial.print("IR_2 dist.: "); Serial.println(distance2); } 
}

/////////////////////////////////////////////////////////////////////////////////
void Variable_Reset()
{ movements_counter = 0;
  wake_up = 0;
  ACTIVE = 0;
  PIR_move = 0;
  Radar_move = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Movement_Sensors_Check() 
{ if (analogRead(PIR) > 300 && !PIR_move)
    { movements_counter ++;
      PIR_move = 1;
      analogWrite(pir_led, 100);
    }
  if (analogRead(PIR) < 300)
    { PIR_move = 0;
      analogWrite(pir_led, 0);
    }
  if(debug) { Serial.print("PIR: "); Serial.println(analogRead(PIR)); }
///////////////////////////
  if (movements_counter > 0)
  { if (analogRead(radar) > 300 && !Radar_move)
      { movements_counter++;
        Radar_move = 1;
        analogWrite(radar_led, 100);
      }
    if (analogRead(radar) < 300)
      { Radar_move = 0;
        analogWrite(radar_led, 0);
      }
    if(debug) { Serial.print("radar: "); Serial.println(analogRead(radar)); }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
void Switch_check()
{ while(analogRead(manual_switch) < 200)
  { digitalWrite(LED, HIGH);            //main LED light ON
    if(debug) 
      Serial.println("Driver OFFLINE");
    delay(1000);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void PowerUP()
{ analogWrite(pir_led, 100);
  delay(100);
  analogWrite(radar_led, 100);
  delay(100);
  digitalWrite(IR_led, HIGH);
  delay(100);
  analogWrite(pir_led, 0);
  delay(100);
  analogWrite(radar_led, 0);
  delay(100);
  digitalWrite(IR_led, LOW);
}


