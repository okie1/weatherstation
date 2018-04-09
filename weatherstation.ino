// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_BMP280.h>

#include "Baro.h"
#include "Wind.h"
#include "Rain.h"
#include "DHT.h"
#include "InterruptHandler.h"//Should not be included here, but the WebIDE yells at you if it is not here.
#include "PietteTech_DHT.h"//Should not be included here, but the WebIDE yells at you if it is not here.

// globals
unsigned long   g_GetWeatherTimer = 0;//Tracks how long it was since we last call GetWeather.
DHT::Manager    g_DHT(D4,DHT22);
Wind::Manager   g_Wind(A0,D3,10);
Rain::Manager   g_Rain(D2,250);
Baro::Manager   g_Baro;


String g_Time = "";
DHT::Info  g_DHTInfo;
Wind::Info g_WindInfo;
Rain::Info g_RainInfo;
Baro::Info g_BaroInfo;

TCPClient client;

//Reset the system after 60 seconds if the application is unresponsive
ApplicationWatchdog wd(60000, System.reset);

void setup()
{
    Particle.variable("Time", &g_Time, STRING);
    Particle.variable("Temperature", &g_DHTInfo.temperature, DOUBLE);
    Particle.variable("Humidity", &g_DHTInfo.humidity, DOUBLE);
    Particle.variable("DewPoint", &g_DHTInfo.dewPoint, DOUBLE);
    Particle.variable("DailyRain", &g_RainInfo.dailyRain, DOUBLE);
    Particle.variable("HourlyRain", &g_RainInfo.rainPerHour, DOUBLE);
    Particle.variable("WindSpeed", &g_WindInfo.instant.speed, DOUBLE);
    Particle.variable("WindDir", &g_WindInfo.instant.direction, DOUBLE);
    Particle.variable("WindGustSpd", &g_WindInfo.gust.speed, DOUBLE);
    Particle.variable("WindGustDir", &g_WindInfo.gust.direction, DOUBLE);
    Particle.variable("AvgWindSpd", &g_WindInfo.avg2Min.speed, DOUBLE);
    Particle.variable("AvgWindDir", &g_WindInfo.avg2Min.direction, DOUBLE);
    Particle.variable("MaxGustSpd", &g_WindInfo.gust10Min.speed, DOUBLE);
    Particle.variable("MaxGustDir", &g_WindInfo.gust10Min.direction, DOUBLE);
    Particle.variable("Pressure", &g_BaroInfo.pressure, DOUBLE);
    Particle.variable("Altitude", &g_BaroInfo.altitude, DOUBLE);
    Particle.variable("Temperature2", &g_BaroInfo.temperature, DOUBLE);

    g_Wind.Start();//Begin wind data collection.
    g_Rain.Start();//Begin Rain data collection.
    g_Baro.Start();//Initialize the sensor.
}


void loop()
{
    //Reset the watchdog timer to prevent the system from rebooting.
    wd.checkin(); // resets the AWDT count
    
    unsigned long timeNow = millis();
    
    //Every 15 seconds, query the weather sensors to get the current readings.
    if((timeNow - g_GetWeatherTimer) >=15000)//if(timeNow > g_GetWeatherTimer+15000)
    {
        g_GetWeatherTimer = timeNow;
        GetWeather();
    
        PublishToWU();
    }
}


void GetWeather()
{
    g_Time          = Time.format(Time.now(), "%F %T");//YYYY-MM-DD HH:MM:SS
    g_DHTInfo       = g_DHT.Get();
    g_WindInfo      = g_Wind.Get();
    g_RainInfo      = g_Rain.Get();
    g_BaroInfo      = g_Baro.Get();
}


void PublishToWU()
{
    client.stop();
    if (client.connect("weatherstation.wunderground.com", 80)) 
    { 
        String temp;
        temp +="GET /weatherstation/updateweatherstation.php?ID=station_id&PASSWORD=password&dateutc=now&action=updateraw"; 
        temp += "&tempf="+String::format("%04.1f",g_DHTInfo.temperature);
        temp += "&dewptf="+String::format("%04.1f",g_DHTInfo.dewPoint);
        temp += "&humidity="+String::format("%04.1f",g_DHTInfo.humidity);
        temp += "&rainin="+String::format("%04.2f",g_RainInfo.rainPerHour);
        temp += "&dailyrainin="+String::format("%04.2f",g_RainInfo.dailyRain);
        temp += "&winddir="+String::format("%03.0f",g_WindInfo.instant.direction); 
        temp += "&windspeedmph="+String::format("%04.1f",g_WindInfo.instant.speed); 
        temp += "&windgustdir="+String::format("%03.0f",g_WindInfo.gust.direction); 
        temp += "&windgustmph="+String::format("%04.1f",g_WindInfo.gust.speed); 
        temp += "&windspdmph_avg2m="+String::format("%04.1f",g_WindInfo.avg2Min.speed); 
        temp += "&winddir_avg2m="+String::format("%03.0f",g_WindInfo.avg2Min.direction); 
        temp += "&windgustmph_10m="+String::format("%04.1f",g_WindInfo.gust10Min.speed); 
        temp += "&windgustdir_10m="+String::format("%03.0f",g_WindInfo.gust10Min.direction); 
        temp += "&baromin="+String::format("%05.2f",g_BaroInfo.pressure); 

        //Particle.publish("Data=",temp);
        client.print(temp);
        client.println(" HTTP/1.0");
        client.println("Accept: text/html");
        client.println("Host: weatherstation.wunderground.com");
        client.println();
    }
    else
    {
        Particle.publish("xDEBUG", "Connection Failed");
    }
}
