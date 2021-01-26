#include <Servo.h>

const int echoPin= 6; 
const int trigPin= 7; 

int out=1;

float low_pass_filter_data,ex_low_pass_filter_data;
float alpha=0.9;

float moving_average_filter_data;
float data_stack[5];
int delta = 5;
Servo servo;    

double Kp = 0.9; 
double Ki = 0; 
double Kd = 0.9;  

double current_error;
double prev_error;
unsigned long prev_time;
unsigned long current_time;
float dt;
int converge_counter = 0;

double current_distance;
double prev_distance=0;
double P_control,I_control,D_control;
double PID_control;
double desired_distance;


float measure(void);
float filter(void);
void move_servo(float);

void setup() {

   Serial.begin(9600);
   
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);
   servo.attach(9);   
  
   delay(1000); 
   move_servo(90);
   delay(2000);
   current_distance = measure();  // cube
   
   delay(1000);
   desired_distance = 15;
   prev_time = micros();
   prev_error = 0;
   Serial.println("setup");
   
}

void loop() {
  current_time = micros();
  dt = (float)(current_time - prev_time)/1000000;
  prev_time = current_time;
  current_distance = filter();
  if(out==1){
     current_distance = prev_distance;
  }
  else{}
   prev_distance = current_distance;
  current_error = desired_distance - current_distance;
  
  if(current_error != 0){
    converge_counter++;
  }
  P_control = Kp * current_error;
  I_control += Ki * current_error * dt;
  D_control = Kd * (current_error - prev_error)/dt;
  PID_control = P_control+I_control+D_control;
  
//  if(current_error > 0.5){
//    PID_control = abs(PID_control);
//    PID_control = map(PID_control,0,15,95,110);
//    move_servo(PID_control);
//    delay(50);
//  }
//  else if(current_error < -0.5){
//    PID_control = abs(PID_control);
//    PID_control = 160-map(PID_control,0,15,70,85);
//    move_servo(PID_control);
//    delay(50);
//  }
//  else {
//  move_servo(90);
//  }

  if(PID_control>-20 && PID_control<20){
    move_servo(90+PID_control);
  }
  else if(PID_control>20){
    move_servo(110);
    }
  else{
    move_servo(70);
    }
  Serial.println(current_distance);

//Serial.print("current_angle : ");
//Serial.print(current_angle);
//Serial.print(" ");
//Serial.print("current_error : ");
//Serial.print(current_error);
//Serial.print(" ");
//Serial.print("PID_control : ");
//Serial.println(PID_control);
//Serial.print(" ");
//Serial.print("dt : ");
//Serial.println(dt);
//Serial.print(" ");
//Serial.print("converge_counter : ");
//Serial.println(converge_counter);
prev_error = current_error;


}


float measure(void) {
  digitalWrite(trigPin, LOW); 
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);   
  unsigned long duration = pulseIn(echoPin, HIGH); 
  float distance = ((float)(340 * duration) / 10000) / 2;
  return distance;
}
float filter(void){ 
  int distance;
  distance=measure();
  low_pass_filter_data=alpha*ex_low_pass_filter_data+(1-alpha)*distance;
  ex_low_pass_filter_data=low_pass_filter_data;
  
  for(int i=0;i<(delta-1);i++){
    data_stack[i]=data_stack[i+1];
  }
  data_stack[(delta-1)]=ex_low_pass_filter_data;
  int sum_data_stack = 0;
  for(int i=0;i<delta;i++){
    sum_data_stack +=data_stack[i];
  }
  moving_average_filter_data = sum_data_stack/ (float) delta;
  if(moving_average_filter_data>30){
    out=1;
  }
  else{
    out=0;
  }
  return moving_average_filter_data; 
//
//  if(ex_low_pass_filter_data>27){
//    out=1;
//  }
//  else{
//    out=0;
//  }
//  return ex_low_pass_filter_data;
}
void move_servo(float u) {
  
servo.write(u);
   
}
