#include <Particle.h>
#include "Baro.h"

namespace Baro
{

//Purpose:    This is the default constructor.
//Parameters: None
//Returns:    None
Manager::Manager():
m_bmp280()
,m_Info()
{
}


//Purpose:   Special code to happen on destruction/exit of this instance.
//Parameter: None
//Returns:   None
Manager::~Manager()
{
}


//Purpose:   Called once to kickstart the bmp280 library.
//Parameter: None
//Returns:   None
void Manager::Start()
{
    m_bmp280.begin();
}


//Purpose: Used by external world to retrieve the sensor data.
//Parameter: None
//Return:  Instance of the info struct populated with sensor data.
Info Manager::Get()
{
    //Get the data from the sensor and convert into the units we are expecting.
    m_Info.temperature = CtoF(m_bmp280.readTemperature());  
    m_Info.pressure    = PtoHg(m_bmp280.readPressure());
    m_Info.altitude    = MtoFt(m_bmp280.readAltitude(1013.25));//pressure at sea level

    return m_Info;
}

double Manager::CtoF(double c)
{
   return c * 1.8 + 32; 
}

double Manager::PtoHg(double p)
{
  return p/3389.39;  
}

double Manager::MtoFt(double m)
{
    return 3.28084 * m;
}


}//end Baro namespace


