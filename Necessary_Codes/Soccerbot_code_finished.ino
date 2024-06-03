#include <IBusBM.h>

IBusBM IBus;     //create IBus object:

//variables to store channel value:
int CH1 = 0; 
int CH2 = 0; 
int THROTTLE = 0; 
//int CH4 = 0; 
//int CH5 = 0; 
//int CH6 = 0;

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
int ch2_right_start = 1490;
int ch2_right_end = 1000;
//LEFT RANGES
int ch1_left_end = 1000;
int ch1_left_start = 1490;
int ch2_left_start = 1510;
int ch2_left_end = 2000;


//Throttle ranges
int throttle_start = 1040;
int throttle_end = 1980;

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
  //CH4 = IBus.readChannel(3);
  //CH5 = IBus.readChannel(4);
  //CH6 = IBus.readChannel(5);
  
  Serial.print(CH1);
  Serial.print(",1 ");
  Serial.print(CH2);
  Serial.print(",2 ");
  Serial.println(THROTTLE);
  //Serial.print(",3 ");
  //Serial.print(CH4);
  //Serial.print(",4 ");
  //Serial.print(CH5);
  //Serial.print(",5 ");
  //Serial.println(CH6);


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

int spd_fwd = map(CH2, fwd_start, fwd_end, 0, MaxSpeed); //FWD 
int spd_bac = map(CH2, bac_start, bac_end, 0, MaxSpeed); //BAC
int ch1_spd_right = map(CH1, ch1_right_start, ch1_right_end, 0, MaxSpeed); //RIGHT ch1
int ch2_spd_right = map(CH2, ch2_right_start, ch2_right_end, 0, MaxSpeed); //RIGHT ch2
int ch1_spd_left = map(CH1, ch1_left_start, ch1_left_end, 0, MaxSpeed); //LEFT ch1
int ch2_spd_left = map(CH2, ch2_left_start, ch2_left_end, 0, MaxSpeed); //LEFT ch1



Serial.print(MaxSpeed);
Serial.print(",MaxSpeed ");
Serial.print(spd_fwd);
Serial.print(",spd_fwd ");
Serial.print(spd_bac);
Serial.print(",spd_bac ");
Serial.print(ch1_spd_right);
Serial.print(",ch1_spd_right ");
Serial.print(ch2_spd_right);
Serial.print(",ch2_spd_right ");
Serial.print(ch1_spd_left);
Serial.print(",ch1_spd_left ");
Serial.print(ch2_spd_left);
Serial.println(",ch2_spd_left ");



if(MaxSpeed<0){     
analogWrite(L_fwd, 0);
analogWrite(L_bac, 0);
analogWrite(R_fwd, 0);
analogWrite(R_bac, 0);
}


//FORWARD
else if(CH1>fwd_start){     
analogWrite(L_fwd, spd_fwd);
analogWrite(L_bac, 0);
analogWrite(R_fwd, spd_fwd);
analogWrite(R_bac, 0); 
}


//RIGHT
else if((CH1>ch1_right_start)&&(CH2<ch2_right_start)){ 
ch1_spd_right = abs(ch1_spd_right);
ch2_spd_right = abs(ch2_spd_right);

analogWrite(L_fwd, ch1_spd_right);
analogWrite(L_bac, 0);
analogWrite(R_fwd, 0);
analogWrite(R_bac, ch2_spd_right);
}


//LEFT
else if((CH1<ch1_left_start)&&(CH2>ch2_left_start)){  
ch1_spd_left = abs(ch1_spd_left);
ch2_spd_left = abs(ch2_spd_left); 
 
analogWrite(L_fwd, 0);
analogWrite(L_bac, ch1_spd_left);
analogWrite(R_fwd, ch2_spd_left);
analogWrite(R_bac, 0);
}


//BACKWARD
else if(CH1<bac_start) // here channel 1 and 2 has same reading
{  
analogWrite(L_fwd, 0);
analogWrite(L_bac, spd_bac);
analogWrite(R_fwd, 0);
analogWrite(R_bac, spd_bac); 
}


else{     
analogWrite(L_fwd, 0);
analogWrite(L_bac, 0);
analogWrite(R_fwd, 0);
analogWrite(R_bac, 0);
}
}
