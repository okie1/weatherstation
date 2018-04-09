#ifndef __RAIN_H__
#define __RAIN_H__

#include "InterruptHandler.h"

namespace Rain
{

struct Info
{
    Info():rainPerHour(0),dailyRain(0){}
    double rainPerHour;
    double dailyRain;
};

class Manager
{
public:
    //Attributes
    //Methods
    Manager(int pin, unsigned long minTime);
    ~Manager();
    void Start();
    Info Get();

private:
    //Attributes
    InterruptHandler    m_InterruptHandler;
    Timer               m_ScheduledReader;
    Info                m_Info;
    double              m_rainPerHourArray[60];
    int                 m_rainPerHourIndex;

    //Methods
    Manager();//Don't allow the default constructor to be called publicly.
    void ScheduledRead();
};
}//end Rain namespace

#endif