#ifndef __BARO_H__
#define __BARO_H__

#include "Adafruit_Sensor.h"
#include "Adafruit_BMP280.h"

namespace Baro
{
struct Info
{
    Info():temperature(0),pressure(0),altitude(0){}
    double temperature;
    double pressure;
    double altitude;
};

class Manager
{
public:
    //Attributes
    //Methods
    Manager();
    ~Manager();
    void Start();
    Info Get();

private:
    //Attributes
    Adafruit_BMP280 m_bmp280; // I2C
    Info           m_Info;

    //Methods
    double CtoF(double c);
    double PtoHg(double p);
    double MtoFt(double m);
};
}//end namespace Baro

#endif

