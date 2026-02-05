# PicoPlanty
This is a (simple) passion and learning project designed to gather data on the amount of moisture in the soil of one of my plants (**Maurice** the Monstera plant!).
## Materials
The project uses a few components to make everything work together, everything listed below
- A **Raspberrypi Pico 2 W** as microcontroler (https://www.cytron.io/p-raspberry-pi-pico-2-wireless)
- A **Capacitive Soil Moisture Sensor** to measure the amount of water in the soil (https://nl.aliexpress.com/item/32832538686.html)
- A Laptop capable of running vscode
- vscode
- A microusb cable to connect the Pico to the laptop
## Pins
In this project the wires of the moisture sensor were connected in the following way :
- the **Red** wire was connected to pin **37, 3V3_EN**
- the **Black** wire was connected to pin **8, GND**
- the **Yellow** wire was connected to pin **32, GP27, ADC1**
## Thought behind the code
Concidering I'm running the pico locally and that the data is not sensitive in any way I decided that broadcasting it to the entire network was the simpelest, shortest and thus best way to go. This ofcourse has some drawbacks like possible loss of data and the data just being up for grabs for just about anyone connected to the network. These drawbacks are negligable for me but you might want to set up better networking if your using more sensitive data or need every bit of data. I am also not really processing the data, only converting it to a intiger and broadcasting it, this is because im planning to recieve and process the data on a raspberry pi 5 where i can do more with it, you might want to change and process data for your own purposes.
## Conciderations for follow up projects
Though this is a fun project its far from perfect! if your looking to learn and play around with your pico you might be interested in implamenting the following things : Better Networking, Making everything async or even recoding the thing in micropython for extended reach
# Thanks for reading <3 & take care of your plants!