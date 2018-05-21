// Derek Broekhoven, Jonathan Gutschon, Martina Nobile, Tyko Van Vilet
// Final project

#include "EV3_FileIO.c"
#include "EV3Servo-lib-UW.c"

const int STRING_LIM_12 = 3;
const int STRING_LIM_3 = 20;
const int STRING_LIM_4 = 30;
const int MOTOR_POWER = 3;

int stringStates[4] = {0, 0, 0, 0};

void selectSong(TFileHandle &songChords, TFileHandle &songMelody)
{
	int MAX_SONG = 1;
	int MIN_SONG = 0;
	int songChoice = 0;

	while(!getButtonPress(buttonEnter))
	{
		eraseDisplay();
		displayString(0, "Please select your song:");
		displayString(songChoice + 2, "   Riptide");
		displayString(3 - songChoice, "   Over the Rainbow");
		displayString(2, "-> ");
		
		while(!getButtonPress(buttonAny))
		{}
		if (getButtonPress(buttonDown))
		{
			songChoice++;
			if (songChoice > MAX_SONG)
				songChoice = MIN_SONG;
		}
		else if (getButtonPress(buttonUp))
		{
			songChoice--;
			if (songChoice < MIN_SONG)
				songChoice = MAX_SONG;
		}
	}
	if (songChoice == 0)
	{
		openReadPC(songChords, "/home/root/lms2012/prjs/rc-data/riptide.txt");
		openReadPC(songMelody, "/home/root/lms2012/prjs/rc-data/riptideFreqs.txt");
	}
	else if (songChoice == 1)
	{
		openReadPC(songChords, "/home/root/lms2012/prjs/rc-data/sotr.txt");
		//openReadPC(songMelody, "/home/root/lms2012/prjs/rc-data/sotrMelody.txt");
	}
	eraseDisplay();
}

void calibrate()
{
	//motor B calibration
	nMotorEncoder[motorB] = 0;
	motor[motorB] = MOTOR_POWER;
	while(nMotorEncoder[motorB] < 27)
	{}
	motor[motorB] = 1;
	wait1Msec(1000);
	nMotorEncoder[motorB] = 0;
  	motor[motorB] = -MOTOR_POWER;
  	wait1Msec(500);
  	while(nMotorEncoder[motorB] > -STRING_LIM_12)
  	{}
	motor[motorB] = 0;

	//motor A calibration
	nMotorEncoder[motorA] = 0;
	motor[motorA] = MOTOR_POWER;
	while(nMotorEncoder[motorA] < 33)
	{}
	motor[motorA] = 1;
	wait1Msec(1000);
	nMotorEncoder[motorA] = 0;
  	motor[motorA] = -MOTOR_POWER;
  	wait1Msec(500);
  	while(nMotorEncoder[motorA] > -STRING_LIM_12)
  	{}
	motor[motorA] = 0;

	//motor D calibration
	nMotorEncoder[motorD] = 0;
	motor[motorD] = -MOTOR_POWER;
	while(nMotorEncoder[motorD] > -45)
	{}
	motor[motorD] = 0;
	wait1Msec(1000);
	nMotorEncoder[motorD] = 0;
  	motor[motorD] = MOTOR_POWER;
  	wait1Msec(500);
  	while(nMotorEncoder[motorD] < STRING_LIM_4)
  	{}
	motor[motorD] = 0;

	//motor C calibration
	nMotorEncoder[motorC] = 0;
	motor[motorC] = 1;
	while (nMotorEncoder[motorC] < 8)
	{}
	nMotorEncoder[motorC] = 0;
	motor[motorC] = MOTOR_POWER;
	while (nMotorEncoder[motorC] < 8)
	{}
	motor[motorC] = 0;
	wait1Msec(1000);
	nMotorEncoder[motorC] = 0;
	motor[motorC] = -MOTOR_POWER;
	wait1Msec(500);
	while(nMotorEncoder[motorC] > -STRING_LIM_3)
	{}
	motor[motorC] = 0;
}

void playChord(string chord)
{
	if(chord == "Am")
	{
		pushFret(1, 2);
		pushFret(2, 0);
		pushFret(3, 0);
		pushFret(4, 0);
  	}
	else if(chord == "C")
		pushFret(4, 3);
	else if(chord == "F")
	{
		pushFret(1, 2);
		pushFret(3, 1);
	}
	else if(chord == "G")
	{
		pushFret(2, 2);
		pushFret(3, 3);
		pushFret(4, 2);
	}
	else if(chord == "Gsus4")
	{
		pushFret(2, 2);
		pushFret(3, 3);
		pushFret(4, 3);
	}
	else if(chord == "Fadd9")
		pushFret(3, 1);
	else
		displayString(0, "Chord is not recognized");
}

void pushFret (int stringNum, int fretNum)
{
	int currentMotor = motorA;
	if (stringNum == 2)
		currentMotor = motorB;
	else if (stringNum == 3)
		currentMotor = motorC;
	else if (stringNum == 4)
		currentMotor = motorD;

	// handles if string is 1 or 2
	if (currentMotor == motorA || currentMotor == motorB)
	{
		if (stringStates[stringNum] == 0)
		{
			stringStates[stringNum] = 2;
			motor [currentMotor] = MOTOR_POWER;
  			while (nMotorEncoder[currentMotor] < STRING_LIM_12)
  			{}
  		}
  		else if (stringStates[stringNum] == 2)
		{
			stringStates[stringNum] = 0;
			motor[currentMotor] = -MOTOR_POWER;
			while (nMotorEncoder[currentMotor] >= 0)
  			{}
		}
  		motor[currentMotor] = 0;
	}

	else if (currentMotor == motorC)
	{
		if (stringStates[stringNum] == 0 && fretNum == 1)
		{
			stringStates[stringNum] = 1;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 0 && fretNum == 3)
		{
			stringStates[stringNum] = 3;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 1 && fretNum == 0)
		{
			stringStates[stringNum] = 0;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -1)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 1 && fretNum == 3)
		{
			stringStates[stringNum] = 3;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * 2)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 3 && fretNum == 1)
		{
			stringStates[stringNum] = 1;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -2)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 3 && fretNum == 0)
		{
			stringStates[stringNum] = 0;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -3)
			{}
			motor[currentMotor] = 0;
		}
	}

	else if (currentMotor == motorD)
	{
		if (stringStates[stringNum] == 0 && fretNum == 2)
		{
			stringStates[stringNum] = 2;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * 2)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 0 && fretNum == 3)
		{
			stringStates[stringNum] = 3;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * 3)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 2 && fretNum == 0)
		{
			stringStates[stringNum] = 0;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -2)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 2 && fretNum == 3)
		{
			stringStates[stringNum] = 3;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 3 && fretNum == 2)
		{
			stringStates[stringNum] = 1;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -1)
			{}
			motor[currentMotor] = 0;
		}
		else if (stringStates[stringNum] == 3 && fretNum == 0)
		{
			stringStates[stringNum] = 0;
			motor[currentMotor] = MOTOR_POWER;
			while (nMotorEncoder[currentMotor] < STRING_LIM_3 * -3)
			{}
			motor[currentMotor] = 0;
		}
	}
}

void strum (int strumMultiplier)
{
	SensorType[S1] = sensorI2CCustom9V;
	if(strumMultiplier == 1)
		setServoPosition(S1, 1, 65);
	else
		setServoPosition(S1, 1, -8);
}

bool nextChord (TFileHandle & fin)
{
	bool checkNext = true;
	string nextChord = " ";

	readTextPC(fin, nextChord);
	if(nextChord == "n")
		checkNext = false;

	return checkNext;
}

task main()
{
	TFileHandle finChords;
	TFileHandle finNotes;

	string songName = "";
	int waitTime = 0;

	int strumMultiplier = 1;

	setServoPosition (S1, 1, -5);

	selectSong(finChords, finNotes);

	readTextPC(finChords, songName);
	displayString(0, "%s", songName);

	readIntPC(finChords, waitTime);
	waitTime = (1.0 / waitTime) * 60000; // time between eigths
	displayString(1, "%d", waitTime);

	///////////////////////////////////////////////////////////

	string chord = "";
	int freq = 0, noteLength = 0, strumMuiltiplier = 1;

	const int NOTE_GAP = 10; //  delay between notes in ms
	nVolume = 4;

	time1[T1] = 0;
	time1[T2] = 0;

	calibrate();

	while(readTextPC(finChords, chord))
	{
		playChord(chord);
		if (finNotes && time1[T2] >= noteLength)
		{
			readIntPC(finNotes, freq);
			readIntPC(finNotes, noteLength);
			wait1Msec(NOTE_GAP);
			if(freq > 0)
			{
				playTone(freq, (noteLength) / 10);
			}
			else if (freq == -1)
				displayString(5, "Invalid Note");
			time1[T2] = 0;
		}
		strum(strumMuiltiplier);
		strumMuiltiplier *= -1;
		wait1Msec(waitTime - T1); // wait until next eigth
		time1[T1] = 0;
	}

	closeFilePC(finChords);
	closeFilePC(finNotes);
}
