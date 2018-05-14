#include <stdio.h>
#include <math.h>

typedef struct PID 
{
	float sensor;
	float setpoint;
	float kp;
	float ki;
	float kd;
	float prev_error;
	float acc_error;
	float pid_correction;
} PID_t;

void compute_pid(PID_t *myPID);

int main()
{
	PID_t yaw_pid =
	{
			.sensor = 60.0,
			.setpoint = 0.0,
			.kp = 5.0,
			.ki = 0.0,
			.kd = 0.5,
			.prev_error = 0.0,
			.acc_error = 0.0,
			.pid_correction = 0.0
	};

	compute_pid(&yaw_pid);
}

void compute_pid(PID_t *myPID)
{
	float currentError = myPID->sensor - myPID->setpoint;
	myPID->acc_error = myPID->acc_error + currentError;

	myPID->pid_correction = (myPID->kp * currentError) + (myPID->kd * (currentError - myPID->prev_error));

	myPID->prev_error = currentError;
}
