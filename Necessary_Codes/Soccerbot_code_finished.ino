#include <IBusBM.h>

IBusBM IBus;     //create IBus object:

//variables to store channel value:
int CH1 = 0; 
int CH2 = 0; 
int THROTTLE = 0; 

//variables
int L_fwd = 5;
int L_bac = 6;
int R_fwd = 7;
int R_bac = 8;

//motor driver enable pins
int R1_EN = 23;
int L1_EN = 25;
int R2_EN = 27;
int L2_EN = 29;
int R1_IS = 31;
int L1_IS = 33;
int R2_IS = 35;
int L2_IS = 37;

//FWD range
int fwd_end = 2000;
int fwd_start = 1510;
//BAC range
int bac_start = 1490;
int bac_end = 1000;
//RIGHT RANGES
int ch1_right_end = 2000;
int ch1_right_start = 1510;
int ch2_right_start = 1510;
int ch2_right_end = 2000;
//LEFT RANGES
int ch1_left_end = 1000;
int ch1_left_start = 1490;
int ch2_left_start = 1490;
int ch2_left_end = 1000;


//Throttle ranges
int throttle_start = 1040;
int throttle_end = 1985;

void setup(){
  Serial.begin(115200);
  IBus.begin(Serial); // iBUS connected to Serial0:

pinMode(R1_EN, OUTPUT);
pinMode(R2_EN, OUTPUT); 
pinMode(L1_EN, OUTPUT);
pinMode(L2_EN, OUTPUT);   
pinMode(R1_IS, OUTPUT);
pinMode(L1_IS, OUTPUT); 
pinMode(R2_IS, OUTPUT);
pinMode(L2_IS, OUTPUT); 
}

void loop(){
  CH1 = IBus.readChannel(0);
  CH2 = IBus.readChannel(1);
  THROTTLE = IBus.readChannel(2);

  
  //Serial.print(CH1);
  //Serial.print(",1 ");
  //Serial.print(CH2);
  //Serial.print(",2 ");
  //Serial.println(THROTTLE);
 

//5V for enable pins
digitalWrite(R1_EN, HIGH);
digitalWrite(R2_EN, HIGH); 
digitalWrite(L1_EN, HIGH);
digitalWrite(L2_EN, HIGH);     
digitalWrite(L1_IS, HIGH);
digitalWrite(L1_IS, HIGH); 
digitalWrite(R2_IS, HIGH);
digitalWrite(L2_IS, HIGH);

//speed mapping
int MaxSpeed = map(THROTTLE, throttle_start, throttle_end, 0, 255);
int ch1_spd_high = map(CH1, ch1_right_start, ch1_right_end, 0, MaxSpeed); //RIGHT ch1
int ch2_spd_high = map(CH2, ch2_right_start, ch2_right_end, 0, MaxSpeed); //RIGHT ch2
int ch1_spd_low = map(CH1, ch1_left_start, ch1_left_end, 0, MaxSpeed); //LEFT ch1
int ch2_spd_low = map(CH2, ch2_left_start, ch2_left_end, 0, MaxSpeed); //LEFT ch1



Serial.print(MaxSpeed);
Serial.print(",max ");
Serial.print(ch1_spd_high);
Serial.print(",ch1_high ");
Serial.print(ch2_spd_high);
Serial.print(",ch2_high ");
Serial.print(ch1_spd_low);
Serial.print(",ch1_spd_low ");
Serial.print(ch2_spd_low);
Serial.println(",ch2_low ");



if(MaxSpeed<0){     
analogWrite(L_fwd, 0);
analogWrite(L_bac, 0);
analogWrite(R_fwd, 0);
analogWrite(R_bac, 0);
}

//ForWard
else if((CH2 > fwd_start)&&( 1510 > CH1 > 1490 )){

analogWrite(L_fwd, 0);
analogWrite(L_bac, ch2_spd_high);
analogWrite(R_fwd, 0);
analogWrite(R_bac, ch2_spd_high); 
}


//TOP-RIGHT
else if(( CH1 > 1510 )&&( CH2 > 1510 )){

analogWrite(L_fwd, ch1_spd_high);
analogWrite(L_bac, 0);
analogWrite(R_fwd, ch2_spd_high);
analogWrite(R_bac, 0);
}


//TOP_LEFT
else if((CH1 < 1490 )&&(CH2 > 1510 )){

analogWrite(L_fwd, ch2_spd_high);
analogWrite(L_bac, 0);
analogWrite(R_fwd, ch1_spd_low);
analogWrite(R_bac, 0);
}


//BOTTOM-RIGHT
else if(( CH1 > 1510 )&&( CH2 < 1490 )){ 
 
analogWrite(L_fwd, 0);
analogWrite(L_bac, ch2_spd_high);
analogWrite(R_fwd, 0);
analogWrite(R_bac, ch1_spd_low);
}


//BOTTOM-LEFT
else if((CH1 < 1490 )&&(CH2 < 1490 )){ 
 
analogWrite(L_fwd, 0);
analogWrite(L_bac, ch2_spd_low);
analogWrite(R_fwd, 0);
analogWrite(R_bac, ch1_spd_low);
}


//BACKWARD
else if((CH2 < bac_start)&&(1510 > CH1 > 1490)){
analogWrite(L_fwd, 0);
analogWrite(L_bac, ch2_spd_low);
analogWrite(R_fwd, 0);
analogWrite(R_bac, ch2_spd_low); 
}


else{     
analogWrite(L_fwd, 0);
analogWrite(L_bac, 0);
analogWrite(R_fwd, 0);
analogWrite(R_bac, 0);
}

}
