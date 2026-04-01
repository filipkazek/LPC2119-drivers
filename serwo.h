void ServoInit(unsigned int uiServoFrequency);
void ServoCallib(void);
void ServoGoTo(unsigned int uiPosition);
enum ServoState {CALLIB, IDLE, IN_PROGRESS, OFFSET};
struct Servo{
  enum ServoState eState;
  unsigned int uiCurrentPosition;
  unsigned int uiDesiredPosition;
};

extern struct Servo sServo;