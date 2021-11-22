//MTE 121 - Shell Game Robot
//Constant Values
const int RED_COL = (int)colorRed;
const int BLUE_COL = (int)colorBlue;
const char RED_WIN_POS = 'r';
const char BLUE_WIN_POS = 'l';
const int MAX_MIX_MOVES = 12;
const int CW = 1;
const int CCW = -1;
const int ULTRASONIC_LENGTH = 50; //cm
const int SWITCH_ROT = 240;
const int MOT_SPEED = 10;
const int STOP_SPEED = 0;
const int HALF_TURN = 180;
const int MIX_TURN = 230;
const int RESET_TURN = 60;
const int OPEN_ARM_ENC = 15;
const int KICK_OUT_ENC = 150;
//Constant EV3 Ports
const tSensors ULTSON_SEN_PORT = S1;
const tSensors COLOUR_SEN_PORT = S2;
const tSensors TOUCH_SEN_PORT = S3;
const tMotor RIGHT_MIX_MOTOR = motorA;
const tMotor LEFT_MIX_MOTOR = motorD;
const tMotor CENTER_MOTOR = motorB;
//Mixing Arrays
char RedMotorMixMoves[MAX_MIX_MOVES] = {'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L', 'L'};
int RedDirMixMoves[MAX_MIX_MOVES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int RedNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

char BlueMotorMixMoves[MAX_MIX_MOVES] = {'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R', 'L', 'R'};
int BlueDirMixMoves[MAX_MIX_MOVES] = {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1};
int BlueNumSpins[MAX_MIX_MOVES] = {1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3};
//Funcitons
void calibrateSensors(tSensors ultson_port, tSensors colour_port, tSensors touch_port);
int senseGamePiece(tSensors colour_port);
void motorMix(char mixMoves, int dirMoves, int numSpins);
bool tooClose(tSensors ultsonPort);
char pieceEnding(int selectedGamePiece);
char getPlayerGuess(tSensors touchsensorPort);
void centerGuess(char playerGuess, tMotor rightMotorPort, tMotor leftMotorPort);
bool guessCorrectness(char winPosition, char playerGuess);
void pushChoice(tMotor motorPortMedium, tMotor rightMotorPort, tMotor leftMotorPort);

task main()
{
	bool playAgainFlag = false;
	calibrateSensors(ULTSON_SEN_PORT, COLOUR_SEN_PORT, TOUCH_SEN_PORT);
	do
	{
		int gamePiece = senseGamePiece(COLOUR_SEN_PORT);
		bool cheatFlag = false;
		//while (cheatFlag == false)
		//{}

		if(gamePiece = RED_COL)
            motorMix(RedMotorMixMoves, RedDirMixMoves, RedNumSpins);

        else   
            motorMix(BlueMotorMixMoves, BlueDirMixMoves, BlueNumSpins);

		eraseDisplay();
		displayString(1, "%d", cheatFlag);
		if (cheatFlag == true)
		{
			displayString(5, "Sorry, I don't play with cheaters");
			wait1Msec(2000);
			stopAllTasks();
		}
		else
		{
			char playerChoice = getPlayerGuess(TOUCH_SEN_PORT);
			centerGuess(playerChoice, RIGHT_MIX_MOTOR, LEFT_MIX_MOTOR);
			wait1Msec(1000);
			pushChoice(CENTER_MOTOR, RIGHT_MIX_MOTOR, LEFT_MIX_MOTOR);
			wait1Msec(1000);
			if (guessCorrectness(pieceEnding(gamePiece), playerChoice))
				displayString(5, "YOU WON");
			else
				displayString(5, "YOU LOST");
		}
		eraseDisplay();
		displayString(5, "Would you like to play again?");
		displayString(6, "Right Button = YES, Left Button = NO");
		while(!getButtonPress(buttonAny))
		{}
		if(getButtonPress(buttonRight))
			playAgainFlag = true;
		else if(getButtonPress(buttonLeft))
			playAgainFlag = false;
		while(getButtonPress(buttonAny))
		{}
	} while (playAgainFlag == true);

	displayString(5, "THANKS FOR PLAYING");
	wait1Msec(2000);
	stopAllTasks();
}

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

//MIX MOTORS
void motorMix(char* arrayMotor, int* arrayDirection, int* arraySpins)
{

    for(int moves = 0; moves < MAX_MIX_MOVES; moves++)
    {
        nMotorEncoder[LEFT_MIX_MOTOR] = 0;
        nMotorEncoder[RIGHT_MIX_MOTOR] = 0;

		tMotor motSide = LEFT_MIX_MOTOR;

        if(arrayMotor[moves] == 'R')
		{
			motSide = RIGHT_MIX_MOTOR;
		}

		if(arrayDirection[moves] == CW)
		{
			motor[motSide] = MOT_SPEED;
			while(nMotorEncoder[motSide] <= (230 + 180 * (arraySpins[moves]-1)))
			{}
			motor[motSide] = STOP_SPEED;

			wait1Msec(100);

			nMotorEncoder[motSide] = 0;
			motor[motSide] = -MOT_SPEED;
			while(abs(nMotorEncoder[motSide]) < RESET_TURN)
			{}
			motor[motSide] = STOP_SPEED;

			wait1Msec(100);

		}

		else
		{
			motor[motSide] = -MOT_SPEED;
			while(nMotorEncoder[motSide] >= -(230 + 180 * (arraySpins[moves]-1)))
			{}
			motor[motSide] = STOP_SPEED;

			wait1Msec(100);

			nMotorEncoder[motSide] = 0;
			motor[motSide] = MOT_SPEED;
			while(abs(nMotorEncoder[motSide]) < RESET_TURN)
			{}
			motor[motSide] = STOP_SPEED;

			wait1Msec(100);
		}

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

char pieceEnding(int selectedGamePiece)
{
	if (selectedGamePiece	== RED_COL)
		return RED_WIN_POS;
	else
		return BLUE_WIN_POS;
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
	displayString(1, "%c",playerGuess);
	while(getButtonPress(buttonAny))
	{}
	return playerGuess;
}

void centerGuess(char playerGuess, tMotor rightMotorPort, tMotor leftMotorPort)
{
	if(playerGuess == 'r')
	{
		nMotorEncoder[rightMotorPort] = 0;

		motor[rightMotorPort] = MOT_SPEED;
		while(nMotorEncoder[rightMotorPort] < SWITCH_ROT)
		{}
		motor[rightMotorPort] = STOP_SPEED;
		nMotorEncoder[rightMotorPort] = 0;
		motor[rightMotorPort] = -MOT_SPEED;
		while (abs(nMotorEncoder[rightMotorPort]) < RESET_TURN)
		{}
		motor[rightMotorPort] = STOP_SPEED;
	}

	else if(playerGuess == 'l')
	{
		nMotorEncoder[leftMotorPort] = 0;

		motor[leftMotorPort] = MOT_SPEED;
		while(nMotorEncoder[leftMotorPort] < SWITCH_ROT)
		{}
		motor[leftMotorPort] = STOP_SPEED;
		nMotorEncoder[leftMotorPort] = 0;
		motor[leftMotorPort] = -MOT_SPEED;
		while (abs(nMotorEncoder[leftMotorPort]) < RESET_TURN)
		{}
		motor[leftMotorPort] = STOP_SPEED;
	}
}

bool guessCorrectness(char winPosition, char playerGuess)
{
	if(winPosition == playerGuess)
		return true;
	else
		return false;
}

void pushChoice(tMotor motorPortMedium, tMotor rightMotorPort, tMotor leftMotorPort)
{
		nMotorEncoder[leftMotorPort] = 0;
		motor[leftMotorPort] = -MOT_SPEED;
		while (abs(nMotorEncoder[leftMotorPort]) < OPEN_ARM_ENC)
		{}
		motor[leftMotorPort] = STOP_SPEED;

		nMotorEncoder[rightMotorPort] = 0;
		motor[rightMotorPort] = -MOT_SPEED;
		while (abs(nMotorEncoder[rightMotorPort]) < OPEN_ARM_ENC)
		{}
		motor[rightMotorPort] = STOP_SPEED;

		motor[motorPortMedium] = MOT_SPEED;
		nMotorEncoder[motorPortMedium] = 0;
		while(abs(nMotorEncoder[motorPortMedium]) < KICK_OUT_ENC)
		{}
		motor[motorPortMedium] = STOP_SPEED;
		//Reverse and reset arms for next game
		nMotorEncoder[leftMotorPort] = 0;
		motor[leftMotorPort] = MOT_SPEED;
		while (abs(nMotorEncoder[leftMotorPort]) < OPEN_ARM_ENC)
		{}
		motor[leftMotorPort] = STOP_SPEED;

		nMotorEncoder[rightMotorPort] = 0;
		motor[rightMotorPort] = MOT_SPEED;
		while (abs(nMotorEncoder[rightMotorPort]) < OPEN_ARM_ENC)
		{}
		motor[rightMotorPort] = STOP_SPEED;

		motor[motorPortMedium] = -MOT_SPEED;
		nMotorEncoder[motorPortMedium] = 0;
		while(abs(nMotorEncoder[motorPortMedium]) < KICK_OUT_ENC)
		{}
		motor[motorPortMedium] = STOP_SPEED;
}