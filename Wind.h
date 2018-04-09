#ifndef __WIND_H__
#define __WIND_H__

#include "InterruptHandler.h"


namespace Wind
{
struct Pair
{
    Pair():speed(0),direction(0){};
    double speed;
    double direction;
};

struct Info
{
    Info():instant(),gust(),avg2Min(),gust10Min(){}
    Pair instant;
    Pair gust;
    Pair avg2Min;
    Pair gust10Min;
};

class Manager
{
public:
    //Attributes
    //Methods
    Manager(int pinDirection, int pinSpeed, unsigned long minTime);
    ~Manager();
    void Start();
    Info Get();

private:
    //Attributes
    int                 m_pinDirection;
    InterruptHandler    m_SpeedInterruptHandler;
    Timer               m_ScheduledReader;
    unsigned long       m_LastScheduledRead;
    Info                m_Info;
    Pair                m_Avg2MinArray[120];
    int                 m_Avg2MinIndex;
    bool                m_Avg2Min;
    Pair                m_Gust10MinArray[600];
    int                 m_Gust10MinIndex;

    //Methods
    Manager();//Don't allow the default constructor to be called publicly.
    void ScheduledRead();
    double GetDirection();
};
}

#endif