# My Weather Station

This project documents a weather station powered by a particle photon that publishes to weather underground.
![alt text](https://github.com/okie1/weatherstation/images/splash.PNG "My Weather Station")

## Overview
I live in a rural area where we pride ourselves in knowing how much rain we got with the last bit of weather that rolled through. I always would use one of those free glass rain gauges that our local electric cooperative would give out each year. This past year, I forgot and left the gauge out after the first freeze and it busted. This spring, I decided I wanted to upgrade my rain gauge to a more accurate one. Being the computer nerd that I am I thought it would be cool to have a rain gauge that would keep a tally of all the rain I received throughout the year. So, I set out to build something that would do just that. One thing led to another and before I knew it, I had built a nice little weather station.

## Parts
* Particle Photon
* Argent Wind/Rain Sensor Assembly
* Adafruit AM2302 (wired DHT22) temperature-humidity sensor
* Adafruit BMP280 I2C or SPI Barometric Pressure & Altitude Sensor
* Solar Panel
* Solar Charger Controller
* Rechargeable 12V battery

I wanted to use the Particle Photon microcontroller because I had such good luck with it on my last project. The board costs around $20 and has several great features that I wanted, several analog and digital I/O pins, WIFI, over the air updates, good online documentation, and a web-based development environment.
* [https://www.adafruit.com/product/2721](https://www.adafruit.com/product/2721)

While searching the internet for ideas on the rain gauge, I was worried about having to go out and dump the rain gauge once it was full. That is where I stumbled across a rather clever way to measure rain. It was an automatic dumping rain gauge (aka tipping bucket).
* [https://en.wikipedia.org/wiki/Rain_gauge#Tipping_bucket_rain_gauge](https://en.wikipedia.org/wiki/Rain_gauge#Tipping_bucket_rain_gauge)

A little more searching for one of these devices I ran across one for sale on SparkFun’s website:
* [https://www.sparkfun.com/products/8942](https://www.sparkfun.com/products/8942)

But this setup was more than a rain gauge, it also would allow me to measure wind speed and direction. While I was at it, I decided to throw in a temperature, humidity, and barometric sensors as well:
* [https://www.adafruit.com/product/393](https://www.adafruit.com/product/393)
* [https://www.adafruit.com/product/2651](https://www.adafruit.com/product/2651)

This covered me for the basic weather sensing parts, but there were a few other parts that I would need. The weather station would be located in my garden, far away from electricity. So, I decided to power the unit via solar. I had a 10-watt solar panel collecting dust on my shop shelf, so I decided to use it for this project. In addition to this, I purchased a solar charger controller, a 12-volt battery, and a short USB cable.
* [https://www.amazon.com/gp/product/B000KGUZHW](https://www.amazon.com/gp/product/B000KGUZHW)
* [https://www.amazon.com/gp/product/B01MU0WMGT](https://www.amazon.com/gp/product/B01MU0WMGT)
* [https://www.amazon.com/ExpertPower-EXP1270-Rechargeable-Lead-Battery/dp/B003S1RQ2S](https://www.amazon.com/ExpertPower-EXP1270-Rechargeable-Lead-Battery/dp/B003S1RQ2S)
* [https://www.amazon.com/StarTech-com-Inch-Micro-USB-Cable/dp/B003YKX6WM](https://www.amazon.com/StarTech-com-Inch-Micro-USB-Cable/dp/B003YKX6WM)

With the power covered, I started looking for something to house my project in. I had used an enclosure from BUD Industries in the past, so I grabbed a water-proof box from them. I also wanted something to mount the components to, so I ordered some acrylic as well.
* [https://www.amazon.com/gp/product/B005UPBNWE](https://www.amazon.com/gp/product/B005UPBNWE)
* [https://www.amazon.com/gp/product/B00YV5M7EM](https://www.amazon.com/gp/product/B00YV5M7EM)

I wondered about placing the temperature sensor directly inside this enclosure. After a bit of research online, I came across a Stevenson Screen. So, I decided to construct one of these myself.
* [https://en.wikipedia.org/wiki/Stevenson_screen](https://en.wikipedia.org/wiki/Stevenson_screen)

To do this, I ran to Lowes and picked up a single slat of wood fence and a few soffit vents.
* [https://www.lowes.com/pd/Severe-Weather-Natural-Cedar-Fence-Picket-Common-19-32-in-x-5-1-2-inx-6-ft-Actual-0-59-in-x-5-5-in-x-6-ft/4323595](https://www.lowes.com/pd/Severe-Weather-Natural-Cedar-Fence-Picket-Common-19-32-in-x-5-1-2-inx-6-ft-Actual-0-59-in-x-5-5-in-x-6-ft/4323595)
* [https://www.lowes.com/pd/Air-Vent-8-in-L-White-Aluminum-Soffit-Vent/3122213](https://www.lowes.com/pd/Air-Vent-8-in-L-White-Aluminum-Soffit-Vent/3122213)

I also repurposed an old self-closing cabinet hinge I had for this task. (one like these:
[https://www.lowes.com/pd/Style-Selections-10-Pack-2-3-4-in-x-1-3-4-in-Satin-Nickel-Self-Closing-FlushCabinet-Hinges/50419386](https://www.lowes.com/pd/Style-Selections-10-Pack-2-3-4-in-x-1-3-4-in-Satin-Nickel-Self-Closing-FlushCabinet-Hinges/50419386)
) All of this would be mounted to a 2” square tube steel pole I had that was 12’ long. I also used 6’ of 2” angle iron. The angle iron is what was cemented into the ground. I sandwiched the steel tube in between the two pieces of angle and drilled out and placed two bolts though it all. This allows me to remove the top bolt and swing the entire thing down to the ground for any maintenance tasks that may be needed.

A few more miscellaneous parts that were used were:
* Plastic zip ties ([https://www.harborfreight.com/11-in-black-cable-ties-100-pk-60277.html](https://www.harborfreight.com/11-in-black-cable-ties-100-pk-60277.html))
* Thermostat wire ([https://www.lowes.com/pd/Southwire-50-ft-18-AWG-5-ConductorThermostat-Wire-By-the-Roll/3128633](https://www.lowes.com/pd/Southwire-50-ft-18-AWG-5-ConductorThermostat-Wire-By-the-Roll/3128633))
* Hose clamps ([https://www.harborfreight.com/automotive-motorcycle/hoses-lines/20-pc-largehose-clamp-assortment-63280.html](https://www.harborfreight.com/automotive-motorcycle/hoses-lines/20-pc-largehose-clamp-assortment-63280.html))
* Hookup wire ([https://www.adafruit.com/product/3111](https://www.adafruit.com/product/3111))
* Breadboard ([http://www.gikfun.com/electronic-pcb-board-c-60/3x-solderable-breadboardgold-plated-finish-proto-board-pcb-p-725.html](http://www.gikfun.com/electronic-pcb-board-c-60/3x-solderable-breadboardgold-plated-finish-proto-board-pcb-p-725.html))

## Circuit

## Code

## Assembly

## Results

## Credits/Sources

## Hindsight
