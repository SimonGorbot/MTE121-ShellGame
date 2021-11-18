//MTE 121 - Shell Game Robot
//=========================================
const int RED_COL = 1;
const int GREEN_COL = 1;
const int BLUE_COL = 1;
const int RED_WIN_POS = 0;
const int GREEN_WIN_POS = 0;
const int BLUE_WIN_POS = 0;
const int MAX_MIX_MOVES = 12;
const int ULTRASONIC_LENGTH = 50; //cm

const tSensors ULTSON_SEN_PORT = S1;
const tSensors COLOUR_SEN_PORT = S2;
const tSensors TOUCH_SEN_PORT = S3;
const tMotor RIGHT_MIX_MOTOR = motorA;
const tMotor LEFT_MIX_MOTOR = motorD;
const char L = LEFT_MIX_MOTOR;
const char R = RIGHT_MIX_MOTOR;
const tMotor CENTER_MOTOR = motorB;
//=========================================
char RedMotorMixMoves[MAX_MIX_MOVES] = {L, R, L, R, L, R, L, R, L, R, L, R};
int RedDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int RedEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

char GreenMotorMixMoves[MAX_MIX_MOVES] = {L, R, L, R, L, R, L, R, L, R, L, R};
int GreenDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};;
int GreenEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};

char BlueMotorMixMoves[MAX_MIX_MOVES] = {L, R, L, R, L, R, L, R, L, R, L, R};
int BlueDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};;
int BlueEncMixMoves[MAX_MIX_MOVES] = {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180};
//=========================================
void calibrateSensorsAndMotors(tSensors ultson_port, tSensors colour_port, tSensors touch_port, tMotor motor_left, tMotor motor_right, tMotor motor_center); //done
void intakeSetUpArrays(fin, arrays);
string senseGamePiece(tSensors colour_port);
void motorMix(array, motorPorts);
bool tooClose(ultrasensorPort);
string getPlayerGuess(touchsensorPort);
void centerGuess(playerGuess, motorPorts);
bool guessCorrectness(array, playerGuess);
void liftChoice(motorPortsMedium);

task main()
{
}

void calibrateSensorsAndMotors(tSensors ultson_port, tSensors colour_port, tSensors touch_port)
{
	SensorType[COLOUR_SEN_PORT] = sensorEV3_Color;
	SensorMode[COLOUR_SEN_PORT] = modeEV3Color_Color;
	SensorType[ULTSON_SEN_PORT] = sensorEV3_Ultrasonic;
	SensorType[TOUCH_SEN_PORT] = sensorEV3_Touch;
}

int senseGamePiece(tSensors colour_port)
{
	int scannedColour = 0;
	string scannedColourName = "";
	displayBigTextLine(5, "PLEASE SCAN AN R,G,B BRICK");
	while(scannedColour != (RED_COL || GREEN_COL || BLUE_COL))
	{
		if (SensorValue[COLOUR_SEN_PORT] == RED_COL)
			scannedColourName = "RED";
		if (SensorValue[COLOUR_SEN_PORT] == BLUE_COL)
			scannedColourName = "BLUE";
		if (SensorValue[COLOUR_SEN_PORT] == GREEN_COL)
			scannedColourName = "GREEN";
	}
	eraseDisplay();
	displayBigTextLine(5, "PLEASE PLACE YOUR %s BRICK UNDER THE CENTER CUP", scannedColour);
	displayBigTextLine(8, "PRESS THE TOUCH SENSOR WHEN DONE");
	while (SensorValue[TOUCH_SEN_PORT] == 0)
	{}
	return scannedColour;
}
