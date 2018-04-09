#include <Particle.h>
#include "DHT.h"

#define DHT_SAMPLE_INTERVAL   10000  // Sample every ten seconds

namespace DHT
{

//Purpose:    This is the default constructor which is private, so it should never be called.
//Parameters: None
//Returns:    None
Manager::Manager():
m_PietteTech_DHT(D0, DHT22)
,m_LastSampleTime(0)
,m_Info()
{
}


//Purpose:   This is the parameterized constructor that is used to instanciate this class.
//Parameter: pin - The digital pin hooked to the DHT22 sensor.
//           type - describes the type of sensor in use (DHT11, DHT22, ...)
//Returns:   None
Manager::Manager(int pin, int type):
m_PietteTech_DHT(pin, type)
,m_LastSampleTime(0)
,m_Info()
{
}


//Purpose:   Special code to happen on destruction/exit of this instance.
//Parameter: None
//Returns:   None
Manager::~Manager()
{
}


//Purpose: Used by external world to retrieve the temperature, humidity, and dewPoint data.
//Parameter: None
//Return:  Instance of the info struct populated with the temperature, humidity, and dewPoint data.
Info Manager::Get()
{
    unsigned long timeNow = millis();
    
    // Check if we need to start the next sample
    if((timeNow - m_LastSampleTime) >= DHT_SAMPLE_INTERVAL)//if(timeNow > m_NextSampleTime)
    {
        m_LastSampleTime = timeNow;
        m_PietteTech_DHT.acquireAndWait(3000);
        // get DHT status
        //int result = m_DHT.getStatus();

        m_Info.temperature = m_PietteTech_DHT.getFahrenheit();
        m_Info.humidity = m_PietteTech_DHT.getHumidity();
        m_Info.dewPoint = CtoF(m_PietteTech_DHT.getDewPointSlow());//`c - m_PietteTech_DHT.getDewPoint();
        //m_Info.dewPoint = m_Info.temperature + m_PietteTech_DHT.getDewPointSlow();
        //if(m_Info.dewPoint > m_Info.temperature)//Use rough calculation.
            //m_Info.dewPoint = m_Info.humidity/100.0 * m_Info.temperature;
            
        //m_Info.dewPoint = m_Info.temperature - (0.36 * (100 - m_Info.humidity));
        
        
      
        //m_NextSampleTime = timeNow + DHT_SAMPLE_INTERVAL;  // set the time for next sample
    }

    return m_Info;
}


//Purpose: Used to convert celcius values to fahrenheit
//Parameter: the value in celcius
//Return:  the value converted to fahrenheit.
double Manager::CtoF(double c)
{
   return c * 1.8 + 32; 
}

}//end DHT namespace

    
