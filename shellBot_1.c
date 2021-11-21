// Motor Encoder = 230 degrees

//MTE 121 - Shell Game Robot
//=========================================
const int RED_COL = 1;
const int GREEN_COL = 1;
const int BLUE_COL = 1;
const char RED_WIN_POS = 'r';
const char GREEN_WIN_POS = 'c';
const char BLUE_WIN_POS = 'l';
const int MAX_MIX_MOVES = 12;
const int ULTRASONIC_LENGTH = 50; //cm
const int HALF_ROT = 180;
const int CC = -1; //Counter Clockwise
const int CW = 1; //Clockwise
const int MOT_SPEED = 30;
const int STOP_SPEED = 0;
const int MIX_TURN = 100000000000000;
const int RESET_TURN = 100000000000000;

const tSensors ULTSON_SEN_PORT = S1;
const tSensors COLOUR_SEN_PORT = S2;
const tSensors TOUCH_SEN_PORT = S3;
const tMotor RIGHT_MIX_MOTOR = motorA;
const tMotor LEFT_MIX_MOTOR = motorD;
const tMotor CENTER_MOTOR = motorB;
//=========================================
char RedMotorMixMoves[MAX_MIX_MOVES] = {'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R'};
int RedDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int RedNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};

char GreenMotorMixMoves[MAX_MIX_MOVES] = {'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R'};
int GreenDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int GreennNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};

char BlueMotorMixMoves[MAX_MIX_MOVES] = {'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R'};
int BlueDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int BlueNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};

//=========================================
void calibrateSensors(tSensors ultson_port, tSensors colour_port, tSensors touch_port);
int senseGamePiece(tSensors colour_port);
void motorMix(char arrayMotor, int arrayDirection, int arraySpins);
bool tooClose(tSensors ultrasensorPort);
char getPlayerGuess(tSensors touchsensorPort);
void centerGuess(char playerGuess, tMotor rightMotorPort, tMotor leftMotorPort);
char pieceEnding(int colourScanned);
bool guessCorrectness(int colourScanned, char playerGuess);
void liftChoice(tMotor motorPortsMedium);

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
		{
			scannedColourName = "RED";
			scannedColour = RED_COL;
		}
		if (SensorValue[COLOUR_SEN_PORT] == BLUE_COL)
		{
			scannedColourName = "BLUE";
			scannedColour = BLUE_COL;
		}
		if (SensorValue[COLOUR_SEN_PORT] == GREEN_COL)
		{
			scannedColourName = "GREEN";
			scannedColour = GREEN_COL;
		}
	}
	eraseDisplay();
	displayBigTextLine(5, "PLEASE PLACE YOUR %s BRICK UNDER THE CENTER CUP", scannedColour);
	displayBigTextLine(8, "PRESS THE TOUCH SENSOR WHEN DONE");
	while (SensorValue[TOUCH_SEN_PORT] == 0)
	{}
	return scannedColour;
}

void motorMix(char arrayMotor, int arrayDirection, int arraySpins)
{
	tMotor turnMotor = motorC;
    if(arrayMotor == 'A')
    {
        turnMotor = RIGHT_MIX_MOTOR;
    }
    else
    {
        turnMotor = LEFT_MIX_MOTOR;
    }

    int turnDist = (arraySpins * 360) + MIX_TURN;

    if(arrayDirection == 1)
    {
        nMotorEncoder[turnMotor] = 0;
        motor[turnMotor] = MOT_SPEED;
        while (nMotorEncoder[turnMotor] < turnDist)
        {}

        nMotorEncoder[turnMotor] = 0;

        turnMotor = -MOT_SPEED;
        while (nMotorEncoder[turnMotor] > RESET_TURN)
        {}
    }
    else
    {
        motor[turnMotor] = -MOT_SPEED;
        while (nMotorEncoder[turnMotor] > -turnDist)
        {}

        nMotorEncoder[turnMotor] = 0;

        turnMotor = MOT_SPEED;
        while (nMotorEncoder[turnMotor] < -RESET_TURN)
        {}
    }
}

char getPlayerGuess(tSensors touchsensorPort)
{
	char playerGuess = 'a';
	while(!getButtonPress(buttonAny))
	{}
	if(getButtonPress(buttonEnter))
		playerGuess = 'c';
	else if(getButtonPress(buttonRight))
		playerGuess = 'r';
	else if(getButtonPress(buttonLeft))
		playerGuess = 'l';
	while(!getButtonPress(buttonAny))
	{}
	return playerGuess;
}

void centerGuess(char playerGuess, tMotor rightMotorPort, tMotor leftMotorPort)
{
	if(playerGuess == 'R')
	{
		nMotorEncoder[RIGHT_MIX_MOTOR] = 0;

		motor[RIGHT_MIX_MOTOR] = 15;
		while(nMotorEncoder[RIGHT_MIX_MOTOR] < HALF_ROT)
		{}
		motor[RIGHT_MIX_MOTOR] = 0;
	}

	else if(playerGuess == 'L')
	{
		nMotorEncoder[LEFT_MIX_MOTOR] = 0;

		motor[LEFT_MIX_MOTOR] = 15;
		while(nMotorEncoder[LEFT_MIX_MOTOR] < HALF_ROT)
		{}
		motor[LEFT_MIX_MOTOR] = 0;
	}
}

char pieceEnding(int colourScanned)
{
	if(colourScanned == RED_COL)
		return RED_WIN_POS;
	else if(colourScanned == GREEN_COL)
		return GREEN_WIN_POS;
	else
		return BLUE_WIN_POS;
}

bool guessCorrectness(char pieceEnding, char playerGuess)
{
	if(pieceEnding == playerGuess)
		return true;
	else
		return false;
}
