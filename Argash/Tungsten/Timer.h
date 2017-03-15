#pragma once
class Timer
{
private:
	__int64 zeroTime;
	__int64 previousTime;
	__int64 delta;
	__int64 totalTime;
	__int64 countsPerSec;
	__int64 pausedTime;
	bool pause;
public:
	Timer(void);
	~Timer(void);
	void Tick();			//Recalculates delta
	float GetDelta();		//Difference between two ticks , in seconds
	__int64 GetTotal();		//Total time in seconds,as integer
	float GetTotalFloat();
	void Pause();
	void Start();
};

