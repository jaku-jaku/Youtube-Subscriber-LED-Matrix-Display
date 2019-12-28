#ifndef TASK_HELPER_H_
#define TASK_HELPER_H_

typedef struct
{
	unsigned long t_ms;
	unsigned int  interval_ms;

	void init(unsigned int interval)
	{
		interval_ms = interval;
	}

	bool onTick(void)
	{
		bool result = false;
		unsigned long c_time = millis();
		if (c_time - t_ms > interval_ms)
		{
			t_ms = c_time;
			result = true;
		}
		return result;
	}

    void reset()
    {
        t_ms = millis();
    }
} LoopWatchDog_t;

#endif //TASK_HELPER_H_
