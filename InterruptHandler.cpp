#include <Particle.h>
#include "InterruptHandler.h"

InterruptHandler::InterruptHandler(int pin,unsigned long minTime=10):
m_Counter(0)
,m_minTime(minTime)
,m_lastInterrupt(0)
,m_timeNow(0)
{
    pinMode(pin, INPUT_PULLDOWN); 
    attachInterrupt(pin, &InterruptHandler::Handler,this, FALLING);
}

InterruptHandler::~InterruptHandler()
{
}


int InterruptHandler::GetInterrupts()
{
    int temp = m_Counter;
    m_Counter = 0;
    return temp;
}

    
void InterruptHandler::Handler()
{
    m_timeNow = millis();
    
    //There could be a bounce in the circuit, so ignore anything faster than minTime.
	if ((m_timeNow-m_lastInterrupt) > m_minTime)
	{
	    m_Counter += 1;
		m_lastInterrupt = m_timeNow; // set up for next interrupt.
	}
}

