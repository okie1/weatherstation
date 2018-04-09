#ifndef __DHT_H__
#define __DHT_H__

#include "PietteTech_DHT.h"  

namespace DHT
{
struct Info
{
    Info():temperature(0),dewPoint(0),humidity(0){}
    double temperature;
    double dewPoint;
    double humidity;
};

class Manager
{
public:
    //Attributes
    //Methods
    Manager(int pin, int type);
    ~Manager();
    Info Get();

private:
    //Attributes
    PietteTech_DHT m_PietteTech_DHT;
    unsigned int   m_LastSampleTime;
    Info           m_Info;

    //Methods
    Manager();//Don't allow the default constructor to be called publicly.
    void QuerySensor();
    double CtoF(double c);
};
}//end namespace DHT

#endif