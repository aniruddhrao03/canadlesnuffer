/*
ME 101 Course Project

*/

// Initialises sensors
void configureAllSensors()
{
	SensorMode[S2] = modeEV3IR_Seeker;
	wait1Msec(200);
	SensorType[S3] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
}

// DRIVING FUNCTIONS //

void driveDistance(int motorPower, int distance)
{
	nMotorEncoder[motorA] = 0;
	distance = (distance*(360/(2*PI*2.75)));

	if (distance > 0)
	{
		motor[motorA] = motor[motorD] = motorPower;
		while (nMotorEncoder[motorA] < distance)
		{}
	}
	else
	{
		motor[motorA] = motor[motorD] = -motorPower;
		while (nMotorEncoder[motorA] < distance)
		{}
	}
	motor[motorA] = motor[motorD] = 0;
}

void driveUntil(int motorPower, int distance)
{
	nMotorEncoder[motorA] = 0;
	motor[motorA] = motor[motorD] = motorPower;
	while (SensorValue[S3] > distance)
	{}
	motor[motorA] = motor[motorD] = 0;
}

// SPINNING FUNCTIONS //

void spinRobot(int motorPower, int angle)
{
	resetGyro(S4);
	motor[motorA] = -motorPower;
	motor[motorD] = motorPower;
	while (abs(getGyroDegrees(S4)) < angle)
	{}
	motor[motorA] = motor[motorD] = 0;
}

void spinRobotPause(int motorPower, int angle)
{
	resetGyro(S4);
	motor[motorA] = -motorPower;
	motor[motorD] = motorPower;
	while (abs(getGyroDegrees(S4)) < angle)
	{}
	displayString(10, "%d", SensorValue[S2]);
	motor[motorA] = motor[motorD] = 0;
	wait1Msec(1000);
}

void spinUntilTime(int motorPower, int untilTime)
{
	int angle = 15;
	time1[T1] = 0;
	while (time1[T1] < untilTime && SensorValue[S2] == 0)
	{
		displayString(10, "%d", SensorValue[S2]);
		spinRobot(motorPower, angle);
	}
	motor[motorA] = motor[motorD] = 0;
}

void findBeacon(int motorPower)
{
	int angle = 3;
	if(SensorValue[S2] > 0)
	{
			spinRobotPause(motorPower, angle);
	}
	else
	{
			spinRobotPause(-motorPower, angle);
	}
	motor[motorA] = motor[motorD] = 0;
	displayString(10, "%f", SensorValue[S2]);
	wait1Msec(1000);
}

// SCISSOR LIFT FUNCTIONS //

int findHeight(int motorPower, int distanceFromBeacon)
{
	nMotorEncoder[motorC] = 0;
	while (SensorValue[S3] > distanceFromBeacon - 1 && SensorValue[S3] < distanceFromBeacon + 1 )
	{
		motor[motorC] = -motorPower;
	}
	motor[motorC] = 0;

	displayString(5, "%d", nMotorEncoder[motorC]);
	wait1Msec(1000);
	return nMotorEncoder[motorC];
}

void snuffCandle(int motorCountValue, int motorPower, int addedMotorCounts)
{
	while (nMotorEncoder[motorC] > motorCountValue - addedMotorCounts)
	{
		motor[motorC] = -motorPower;
	}
	motor[motorC]=0;

	wait1Msec(1000);
	driveDistance(5, 10);

	nMotorEncoder[motorC]=0;

	while (nMotorEncoder[motorC] < (addedMotorCounts))
	{
		motor[motorC] = motorPower;
	}
	motor[motorC]=0;
	wait1Msec(5000);

	nMotorEncoder[motorC]=0;
	while (nMotorEncoder[motorC] > motorCountValue - addedMotorCounts)
	{
		motor[motorC] = -motorPower;
	}
	motor[motorC]=0;

}

void scissorLiftDown(int motorPower, int travelDistance)
{
	while (nMotorEncoder[motorC] < travelDistance)
	{
		motor[motorC] = motorPower;
	}
	motor[motorC]=0;

}

task main()
{
	while(SensorValue[S2] != 0 )
	{
		displayString(1, "Starting program.");
		wait1Msec(3000);

		nMotorEncoder[motorD] = 0;


		displayString(1, "Running program.");
		configureAllSensors();
		const int MOTOR_POWER_DRIVE = 15, SPIN_TIME = 15000;

	  spinUntilTime(MOTOR_POWER_DRIVE, SPIN_TIME);
		while (SensorValue[S2] >= 1 || SensorValue[S2] <= -1)
		{
			findBeacon(MOTOR_POWER_DRIVE);
		}

		nMotorEncoder[motorD] = 0;

		driveUntil(MOTOR_POWER_DRIVE, 20);

		int height = 0;
		height = findHeight(5, 20);
		wait1Msec(2000);

		snuffCandle(height, 5, 180);
		wait1Msec(5000);

		int totalDistance = abs(nMotorEncoder[motorD] *((PI*2.75)/180));

		spinRobot(12,180);
		wait1Msec(2000);

		scissorLiftDown(10, height);
		driveDistance(12,totalDistance);

		spinUntilTime(MOTOR_POWER_DRIVE, SPIN_TIME);
	}
}
