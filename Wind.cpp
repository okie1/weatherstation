#include <Particle.h>
#include "Wind.h"

namespace Wind
{

//Purpose:    This is the default constructor which is private, so it should never be called.
//Parameters: None
//Returns:    None
Manager::Manager():
m_pinDirection(0)
,m_SpeedInterruptHandler(D0,10)
,m_ScheduledReader(1000,&Manager::ScheduledRead,*this)
,m_LastScheduledRead(0)
,m_Info()
,m_Avg2MinArray()
,m_Avg2MinIndex(-1)
,m_Avg2Min(false)
,m_Gust10MinArray()
,m_Gust10MinIndex(-1)
{
}


//Purpose:   This is the parameterized constructor that is used to instanciate this class.
//Parameter: pinDirection - The analog pin hooked to the direction sensor.
//           pinSpeed - The digital pin hooked to the speed sensor.
//           minTime - The time in milliseconds to ignore reports from the speed sensor (ignore "bounces").
//Returns:   None
Manager::Manager(int pinDirection, int pinSpeed, unsigned long minTime):
m_pinDirection(pinDirection)
,m_SpeedInterruptHandler(pinSpeed,minTime)
,m_ScheduledReader(1000,&Manager::ScheduledRead,*this)
,m_LastScheduledRead(0)
,m_Info()
,m_Avg2MinArray()
,m_Avg2MinIndex(-1)
,m_Avg2Min(false)
,m_Gust10MinArray()
,m_Gust10MinIndex(-1)
{
    m_LastScheduledRead = millis();
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
    //Get the amount of time that has passed since the last read.
    unsigned long timeNow = millis();
    double secondsSinceLastCall = (double)(timeNow - m_LastScheduledRead)/1000.0;
    m_LastScheduledRead = timeNow;

    //Determine how many hits we have got on the indicator.
    double interrupts = m_SpeedInterruptHandler.GetInterrupts();

    //Sample is in MPH.
    m_Info.instant.speed     = interrupts/secondsSinceLastCall * 1.492;
    m_Info.instant.direction = GetDirection();

    //Check to see if this is the new gust record.
    if(m_Info.instant.speed > m_Info.gust.speed)
    {
        m_Info.gust.speed     = m_Info.instant.speed;
        m_Info.gust.direction = m_Info.instant.direction;
    }

    //Add the current sample data to the 2 minute array.
    m_Avg2MinIndex++;
    if(m_Avg2MinIndex == 120)//If we have reached the end of the 2 minute buffer, wrap around and start overwriting old data.
    {
        m_Avg2MinIndex = 0;
        m_Avg2Min = true;//We now have a full 2 minutes of data.
    }
    m_Avg2MinArray[m_Avg2MinIndex].speed     = m_Info.instant.speed;
    m_Avg2MinArray[m_Avg2MinIndex].direction = m_Info.instant.direction;
    
    //Add the current sample to the 10 minute gust array.
    m_Gust10MinIndex++;
    if(m_Gust10MinIndex == 600)//If we have reached the end of the 10 minute buffer, wrap around and start overwriting old data.
    {
        m_Gust10MinIndex = 0;
    }
    m_Gust10MinArray[m_Gust10MinIndex].speed     = m_Info.instant.speed;
    m_Gust10MinArray[m_Gust10MinIndex].direction = m_Info.instant.direction;
}


//Purpose: Used by external world to retrieve the stored up wind data.
//Parameter: None
//Return:  Instance of the info struct populated with wind data.
Info Manager::Get()
{
    Info info = m_Info;
    
    //Reset data
    m_Info.instant.speed = 0;
    m_Info.instant.direction = 0;
    m_Info.gust.speed = 0; 
    m_Info.gust.direction = 0;
    
    //Calculate the average speed and direction for past 2 minutes.
    {
        //Determine how many samples of data we have to work with.
        int samples = 0;
        if(m_Avg2Min)
            samples = 120;
        else if(m_Avg2MinIndex > -1)
            samples = m_Avg2MinIndex+1;
            
        //Calculate the average wind speed.
        for(int a=0;a<samples;++a)
            info.avg2Min.speed += m_Avg2MinArray[a].speed;
        if(samples > 0)//Prevent divide by zero if a get is done before the first sample(second)
            info.avg2Min.speed /= samples;
        
        //**************************************************************************************************************************************
        //Calculate the average wind direction.
        //Thank you Nathan Seidle.... I owe you a beer. (https://github.com/sparkfun/Wimp_Weather_Station/blob/master/Wimp_Weather_Station.ino)
        //**************************************************************************************************************************************

    	//You can't just take the average. Google "mean of circular quantities" for more info
    	//We will use the Mitsuta method because it doesn't require trig functions
    	//And because it sounds cool.
	    //Based on: http://abelian.org/vlf/bearings.html
	    //Based on: http://stackoverflow.com/questions/1813483/averaging-angles-again
    	double sum = m_Avg2MinArray[0].direction;
    	double D   = m_Avg2MinArray[0].direction;
    	for(int a=1;a < samples;++a)
    	{
    		double delta = m_Avg2MinArray[a].direction - D;
    
    		if(delta < -180)
    			D += delta + 360;
    		else if(delta > 180)
    			D += delta - 360;
    		else
    			D += delta;
    
    		sum += D;
    	}
    	info.avg2Min.direction = sum / samples;
    	if(info.avg2Min.direction >= 360) 
    	    info.avg2Min.direction -= 360;
    	if(info.avg2Min.direction < 0)
    	    info.avg2Min.direction += 360;
        //**************************************************************************************************************************************
    }
    
    //Get the highest gust direction and speed for last 10 minutes.
    for(int a=0;a<600;++a)
    {
        if(m_Gust10MinArray[a].speed > info.gust10Min.speed)
        {
            info.gust10Min.speed = m_Gust10MinArray[a].speed;
            info.gust10Min.direction = m_Gust10MinArray[a].direction;
        }
    }

    return info;
}


//Purpose: Used to get in immediate wind direction.
//Parameter: None
//Return:  The wind direction in degrees.
double Manager::GetDirection()
{
    //Read from the wind direction pin.
    double resistance = analogRead(m_pinDirection);

    //Convert the read resistor value into degrees.
    if(resistance < 273) return 112.5;//East/Southeast
    if(resistance < 345) return 67.5;//East/Northeast
    if(resistance < 382) return 90;//East/Northeast
    if(resistance < 516) return 157.5;//South/Southeast
    if(resistance < 748) return 135;//Southeast
    if(resistance < 989) return 202.5;//South/Southwest
    if(resistance < 1159) return 180;//South
    if(resistance < 1633) return 22.5;	//North/Northeast
    if(resistance < 1855) return 45;	//Northeast
    if(resistance < 2408) return 247.5;	//West/Southwest
    if(resistance < 2530) return 225;	//Southwest
    if(resistance < 2821) return 337.5;	//North/Northwest
    if(resistance < 3153) return 360;	//North
    if(resistance < 3320) return 292.5;	//West/Northwest
    if(resistance < 3558) return 315;	//Northwest
    if(resistance < 3790) return 270;	//West
}



}//end Wind namespace

