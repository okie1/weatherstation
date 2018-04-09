#ifndef __INTERRUPTHANDLER_H__
#define __INTERRUPTHANDLER_H__

class InterruptHandler
{
public:
    //Attributes
    //Methods
    InterruptHandler(int pin,unsigned long minTime);
    ~InterruptHandler();
    int GetInterrupts();
    
private:
    //Attributes
    volatile int m_Counter;
    unsigned long m_minTime;
    unsigned long m_lastInterrupt;
    unsigned long m_timeNow;

    
    //Methods
    InterruptHandler(){};//Don't allow external code to call.
    void Handler();
  
    
};


#endif
