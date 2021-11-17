//MTE 121 - Shell Game Robot
#include "PC_FileIO.c"
//=========================================
const int RED_COL = 1;
const int GREEN_COL = 1;
const int BLUE_COL = 1;
const int RED_WIN_POS = 0;
const int GREEN_WIN_POS = 0;
const int BLUE_WIN_POS = 0;
const int MAX_MIX_MOVES = 12;
const int ULTRASONIC_LENGTH = 50; //cm

const tSensors ULTSON_SEN_PORT = 1;
const tSensors COLOUR_SEN_PORT = 2;
const tSensors TOUCH_SEN_PORT = 3;
const tMotor RIGHT_MIX_MOTOR = A;
const tMotor LEFT_MIX_MOTOR = D;
const tMotor CENTER_MOTOR = B;
//=========================================
char RedMotorMixMoves[MAX_MIX_MOVES] = {A, D, A, D, A, D, A, D, A, D, A, D};
int RedDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int RedEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

char GreenMotorMixMoves[MAX_MIX_MOVES] = {A, D, A, D, A, D, A, D, A, D, A, D};
int GreenDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};;
int GreenEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

char BlueMotorMixMoves[MAX_MIX_MOVES] = {A, D, A, D, A, D, A, D, A, D, A, D};
int BlueDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};;
int BlueEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
//=========================================
void calibrateSensorsAndMotors(tSensors ultson_port, tSensors colour_port, tSensors touch_port, tMotor motor_left, tMotor motor_right, tMotor motor_center);
void intakeSetUpArrays(fin, arrays);
int senseGamePiece(colourSensorPort);
void motorMix(array, motorPorts);
bool tooClose(ultrasensorPort);
string getPlayerGuess(touchsensorPort);
void centerGuess(playerGuess, motorPorts);
bool guessCorrectness(array, playerGuess);
void liftChoice(motorPortsMedium);

task main()
{
}

void calibrateSensorsAndMotors(
