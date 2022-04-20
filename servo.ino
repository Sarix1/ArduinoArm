#include <Servo.h>
#include <math.h>

typedef struct
{
  Servo servo;
  float angle;
  float angvel;
  float min_angle;
  float max_angle;
} Joint;

typedef struct
{
  Joint Upper;
  Joint Fore;
  float arm_length;
  float extension;
  float min_extension;
  float max_extension;
} Arm;

Joint Base;
Joint Claw;
Arm Arm1;

void initJoint(Joint* joint, int pin, float angle, float min_angle, float max_angle)
{
  joint->servo.attach(pin);
  joint->servo.write(angle);
  joint->angle     = angle;
  joint->angvel    = 0;
  joint->min_angle = min_angle;
  joint->max_angle = max_angle;
}

void updateJoint(Joint* joint)
{
  joint->servo.write((int)joint->angle);
}

void setJoint(Joint* joint, float angle)
{
  if (angle > joint->max_angle)
    angle = joint->max_angle;
  else if (angle < joint->min_angle)
    angle = joint->min_angle;
  joint->angle = angle;
  updateJoint(joint);
}

void initArm(Arm* arm, float arm_length, float extension, float min_extension, float max_extension)
{
  arm->arm_length = arm_length;
  arm->extension = extension;
  arm->min_extension = min_extension;
  arm->max_extension = max_extension;
}

void setArm(Arm* arm, float extension)
{
  float angle;
  
  if (extension <= arm->min_extension)
    extension = arm->min_extension;
  else if (extension >= arm->max_extension)
    extension = arm->max_extension;
  
  arm->extension = extension;
  angle = acos((extension/2)/arm->arm_length) / M_PI * 180.0;
  setJoint(&arm->Upper, arm->Upper.min_angle + angle);
  setJoint(&arm->Fore,  arm->Fore.min_angle + (angle*2));

  Serial.print("angle: ");
  Serial.println(angle);
}

void setup()
{
  Serial.begin(9600);
  
  initJoint(&Base,       9,   30,  30,  180);
  initJoint(&Arm1.Upper, 10,  90,  90,  180);
  initJoint(&Arm1.Fore,  11,  60,  60,  155);
  initJoint(&Claw,       12,  25,  25,  160);
  initArm  (&Arm1,       85,  30,  30,  160);
}

void loop()
{
  static float x = 30;
  while (x < 160)
  {
    x++;
    setArm(&Arm1, x);
  Serial.print("ext: ");
  Serial.println(x);
    delay(2);
  }
  setJoint(&Claw, Claw.min_angle);
  delay(500);
  setJoint(&Claw, Claw.max_angle);
  delay(500);
  setJoint(&Claw, Claw.min_angle);
  delay(500);
  while (x > 30)
  {
    x--;
    setArm(&Arm1, x);
      Serial.print("ext: ");
  Serial.println(x);
    delay(2);
  }
  setJoint(&Claw, Claw.min_angle);
  delay(500);
  setJoint(&Claw, Claw.max_angle);
  delay(500);
  setJoint(&Claw, Claw.min_angle);
  delay(500);
}
