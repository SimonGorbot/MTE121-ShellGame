//MTE 121 - Shell Game Robot
//=========================================
const int RED_COL = (int)colorRed;
const int BLUE_COL = (int)colorBlue;
const char RED_WIN_POS = 'r';
const char BLUE_WIN_POS = 'l';
const int MAX_MIX_MOVES = 12;
const int ULTRASONIC_LENGTH = 50; //cm
const int HALF_ROT = 180;
const int SWITCH_ROT = 230;
const int CC = -1; //Counter Clockwise
const int CW = 1; //Clockwise
const int MOT_SPEED = 10;
const int STOP_SPEED = 0;
const int MIX_TURN = 230;
const int RESET_TURN = 50;

const tSensors ULTSON_SEN_PORT = S1;
const tSensors COLOUR_SEN_PORT = S2;
const tSensors TOUCH_SEN_PORT = S3;
const tMotor RIGHT_MIX_MOTOR = motorA;
const tMotor LEFT_MIX_MOTOR = motorD;
const tMotor CENTER_MOTOR = motorB;
//=========================================
char RedMotorMixMoves[MAX_MIX_MOVES] = {'L', 'L', 'R', 'R', 'L', 'L', 'L', 'L', 'L', 'R', 'R', 'L'};
int RedDirMixMoves[MAX_MIX_MOVES] = {1, -1, -1, 1, 1, 1, 1, -1, 1, -1, 1, 1};
int RedNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

char BlueMotorMixMoves[MAX_MIX_MOVES] = {'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R'};
int BlueDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int BlueNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
//=========================================
void calibrateSensors(tSensors ultson_port, tSensors colour_port, tSensors touch_port);
int senseGamePiece(tSensors colour_port);
void motorMix(char* arrayMotor, int* arrayDirection, int* arraySpins);
bool tooClose(tSensors ultsonPort);
char getPlayerGuess(tSensors touchsensorPort);
void centerGuess(char playerGuess);
char pieceEnding(int colourScanned);
bool guessCorrectness(int colourScanned, char playerGuess);
void liftChoice(tMotor motorPortsMedium);
//=========================================
task main()
{
	calibrateSensors(ULTSON_SEN_PORT, COLOUR_SEN_PORT, TOUCH_SEN_PORT);

	int gamePiece = senseGamePiece(COLOUR_SEN_PORT);
	char usingMotorMixMoves[MAX_MIX_MOVES];
	int usingDirMixMoves[MAX_MIX_MOVES];
	int usingNumSpins[MAX_MIX_MOVES];

	eraseDisplay();

    if(gamePiece = RED_COL)
        motorMix(RedMotorMixMoves, RedDirMixMoves, RedNumSpins);

    else   
        motorMix(BlueMotorMixMoves, BlueDirMixMoves, BlueNumSpins);

	displayString(5, "%d", gamePiece);
	wait1Msec(5000);
}
//=========================================
void calibrateSensors(tSensors ultson_port, tSensors colour_port, tSensors touch_port)
{
	SensorType[COLOUR_SEN_PORT] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[COLOUR_SEN_PORT] = modeEV3Color_Color;
	wait1Msec(50);
	SensorType[ULTSON_SEN_PORT] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[TOUCH_SEN_PORT] = sensorEV3_Touch;
	wait1Msec(50);
}

int senseGamePiece(tSensors colour_port)
{
	int scannedColour = 0;
	string scannedColourName = "";
	bool scanFlag = false;
	displayString(5, "PLEASE SCAN A RED OR BLUE BRICK");
	while(scanFlag == false)
	{
		if (SensorValue[COLOUR_SEN_PORT] == RED_COL)
		{
			scannedColourName = "RED";
			scannedColour = RED_COL;
			scanFlag = true;
		}
		else if (SensorValue[COLOUR_SEN_PORT] == BLUE_COL)
		{
			scannedColourName = "BLUE";
			scannedColour = BLUE_COL;
			scanFlag = true;
		}
	}
	eraseDisplay();
	displayString(5, "PLEASE PLACE YOUR %s BRICK UNDER THE CENTER CUP", scannedColourName);
	displayString(6, "PRESS THE TOUCH SENSOR WHEN DONE");
	while (SensorValue[TOUCH_SEN_PORT] == 0)
	{}
	eraseDisplay();
	return scannedColour;
}

void motorMix(char* arrayMotor, int* arrayDirection, int* arraySpins)
{

    for(int moves = 0; moves < MAX_MIX_MOVES; moves++)
    {
        nMotorEncoder[LEFT_MIX_MOTOR] = 0;
        nMotorEncoder[RIGHT_MIX_MOTOR] = 0;

        if(arrayMotor[moves] == 'L')
        {
            if(arrayDirection[moves] == CW)
            {
                motor[LEFT_MIX_MOTOR] = MOT_SPEED;
                while(nMotorEncoder[LEFT_MIX_MOTOR] <= (230 + 180 * (arraySpins[moves]-1)))
                {}
                motor[LEFT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);

                nMotorEncoder[LEFT_MIX_MOTOR] = 0;
                motor[LEFT_MIX_MOTOR] = -MOT_SPEED;
                while(abs(nMotorEncoder[LEFT_MIX_MOTOR]) < RESET_TURN)
                {}
                motor[LEFT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);

            }

            else
            {
                motor[LEFT_MIX_MOTOR] = -MOT_SPEED;
                while(nMotorEncoder[LEFT_MIX_MOTOR] >= -(230 + 180 * (arraySpins[moves]-1)))
                {}
                motor[LEFT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);

                nMotorEncoder[LEFT_MIX_MOTOR] = 0;
                motor[LEFT_MIX_MOTOR] = MOT_SPEED;
                while(abs(nMotorEncoder[LEFT_MIX_MOTOR]) < RESET_TURN)
                {}
                motor[LEFT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);
            }
        }

        else
        {
            if(arrayDirection[moves] == CW)
            {
                motor[RIGHT_MIX_MOTOR] = -MOT_SPEED;
                while(nMotorEncoder[RIGHT_MIX_MOTOR] >= -(230 + (180 * (arraySpins[moves]-1))))
                {}
                motor[RIGHT_MIX_MOTOR] = STOP_SPEED;
                nMotorEncoder[RIGHT_MIX_MOTOR] = 0;

                wait1Msec(100);

                nMotorEncoder[RIGHT_MIX_MOTOR] = 0;
                motor[RIGHT_MIX_MOTOR] = MOT_SPEED;
                while(abs(nMotorEncoder[RIGHT_MIX_MOTOR]) < RESET_TURN)
                {}
                motor[RIGHT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);
            }

            else
            {
                motor[RIGHT_MIX_MOTOR] = MOT_SPEED;
                while(nMotorEncoder[RIGHT_MIX_MOTOR] <= (230 + (180 * (arraySpins[moves]-1))))
                {}
                motor[RIGHT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);

                nMotorEncoder[RIGHT_MIX_MOTOR] = 0;
                motor[RIGHT_MIX_MOTOR] = -MOT_SPEED;
                while(abs(nMotorEncoder[RIGHT_MIX_MOTOR]) < RESET_TURN)
                {}
                motor[RIGHT_MIX_MOTOR] = STOP_SPEED;

                wait1Msec(100);
            }
        }
        wait1Msec(50);
    }
}




bool tooClose(tSensors ultsonPort)
{
	if(SensorValue[ultsonPort]<ULTRASONIC_LENGTH)
	{
		//if the player is too close
		displayString(4,"PLAYER IS TOO CLOSE");
		playSound(soundBeepBeep);
		return true;
	}
	else
	{
		//if the player is far enough away
		displayString(2,"Player distance: %d",SensorValue[ultsonPort]);
		return false;
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

void centerGuess(char playerGuess)
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

