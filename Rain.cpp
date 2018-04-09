#include <Particle.h>
#include "Rain.h"

namespace Rain
{

//Purpose:    This is the default constructor which is private, so it should never be called.
//Parameters: None
//Returns:    None
Manager::Manager():
m_InterruptHandler(D0,10)
,m_ScheduledReader(60000,&Manager::ScheduledRead,*this)
,m_Info()
,m_rainPerHourArray()
,m_rainPerHourIndex(-1)
{
}


//Purpose:   This is the parameterized constructor that is used to instanciate this class.
//Parameter: pin - The digital pin hooked to the rain sensor.
//           minTime - The time in milliseconds to ignore reports from the rain sensor (ignore "bounces").
//Returns:   None
Manager::Manager(int pin, unsigned long minTime):
m_InterruptHandler(pin,minTime)
,m_ScheduledReader(60000,&Manager::ScheduledRead,*this)
,m_Info()
,m_rainPerHourArray()
,m_rainPerHourIndex(-1)
{
}


//Purpose:   Special code to happen on destruction/exit of this instance.
//Parameter: None
//Returns:   None
Manager::~Manager()
{
}


//Purpose:   Called once to start the internal data collection.
//Parameter: None
//Returns:   None
void Manager::Start()
{
    m_ScheduledReader.start();
}


//Purpose:   Called regularly (each second) by a software timer to collect data.
//Parameter: None
//Returns:   None
void Manager::ScheduledRead()
{
    //Test to see if it is midnght.  If so, reset the daily tally.
    if(Time.hour() == 5 && Time.minute() == 0)
    {
        m_Info.dailyRain = 0;
    }
        
    //Determine how much rain we have received since the last read.
    double interrupts = m_InterruptHandler.GetInterrupts();
    double rainfall = 0;

    for(int i=0;i<interrupts;++i)
    {
        rainfall += 0.011;//Each interrupt represents a single dump which is 0.011" of rainfall.
    }

    //Add this amount to the daily total.
    m_Info.dailyRain += rainfall;

    //Also add this amount to the buffer that tracks the amount of rain in the past hour.
    m_rainPerHourIndex++;
    if(m_rainPerHourIndex == 60)//If we have reached the end of the 60 minute buffer, wrap around and start overwriting old data.
        m_rainPerHourIndex = 0;
    m_rainPerHourArray[m_rainPerHourIndex] = rainfall;
}


//Purpose: Used by external world to retrieve the stored up rain data.
//Parameter: None
//Return:  Instance of the info struct populated with rain data.
Info Manager::Get()
{
    //Calculate the average rainfall in last hour.
    m_Info.rainPerHour = 0;
    for(int a=0;a<60;++a)
        m_Info.rainPerHour += m_rainPerHourArray[a];

    return m_Info;
}


}//end Rain namespace
