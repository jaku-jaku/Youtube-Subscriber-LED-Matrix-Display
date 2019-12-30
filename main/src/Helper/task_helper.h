#ifndef TASK_HELPER_H_
#define TASK_HELPER_H_

class LoopWatchDog
{
    public:
        LoopWatchDog(unsigned int interval)
        {
            t_ms = 0;
            interval_ms = interval;
        }

        bool onTick(void)
        {
            bool result = false;
            if (millis() - t_ms > interval_ms)
            {
                t_ms = millis();
                result = true;
            }
            return result;
        }

        void reset()
        {
            t_ms = millis();
        }
    private:
    	unsigned long t_ms;
	    unsigned int  interval_ms;
};

#endif //TASK_HELPER_H_
