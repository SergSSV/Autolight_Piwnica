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
  { active = 1;
    movements_counter = 0;
    wake_up = 0;
    digitalWrite(IR_led, HIGH);      //shows sensor work
    R_move2 = 0;
    analogWrite(radar_led, 0);    // radar reset
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
  active = 0;
  PIR_move = 0;
  R_move2 = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////
void Movement_Sensors_Check() 
{ int analog3 = analogRead(PIR);                                    // PIR
  if (analog3 > 300 && !PIR_move)
    { movements_counter ++;
      PIR_move = 1;
      analogWrite(pir_led, 100);
    }
  if (analog3 < 300)
    { PIR_move = 0;
      analogWrite(pir_led, 0);
    }
  if(debug) { Serial.print("PIR: "); Serial.println(analog3); }
///////////////
  if (movements_counter > 0)
  { int analog2 = analogRead(radar);                                 // radar 2
    if (analog2 > 300 && !R_move2)
      { movements_counter++;
        R_move2 = 1;
        analogWrite(radar_led, 100);
      }
    if (analog2 < 300)
      { R_move2 = 0;
        analogWrite(radar_led, 0);
      }
    if(debug) { Serial.print("radar: "); Serial.println(analog2); }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////
void Switch_check()
{ int button = analogRead(manual_switch);
  while(button < 200)
  { digitalWrite(LED, HIGH);            //main LED light ON
    if(debug) 
      Serial.println("Driver OFFLINE");
    delay(1000);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////
void PowerUP()
{ analogWrite(pir_led, 100);
  delay(200);
  analogWrite(radar_led, 100);
  delay(200);
  digitalWrite(IR_led, HIGH);
  delay(200);
  analogWrite(pir_led, 0);
  delay(200);
  analogWrite(radar_led, 0);
  delay(200);
  digitalWrite(IR_led, LOW);
}


