# Exploration Rover
Here we can put whatever we make.

We are gonna put here also all the details of the project, to reference check the projects of the previous years, like this one: https://github.com/Clown-Machine/ClawMachine?tab=readme-ov-file


So we have to say a lot of things also about how to vreate and build the project.

==========================================================================

This project tries to create and learn how a exploration rover can be, how can be built and what we can do with it. We liked the idea to do something that could move everywhere, on every type of soil and very far away, so we projected and created our personal rover from scratch.

Because we couldn't see it everywhere, we gave to the rover an eye (a camera), so we can see what it sees.

Maybe is not a professional rover, or a competitive one, but for sure with a very limited budget and almost zero starting knowledge, we learned a lot about embedded software and IOT.

>[!Note]
>
>In the print file folder there are a lot of files, cause during the desing phase we scraped and redesinged a lot of time the rover prototype.


## Requirements

#### Hardware

To build and use this project, you need the following hardware:

- 1x Texas Instruments MSP432P401R microcontroller
- 1x [Educational BoosterPack MKII](https://www.ti.com/tool/BOOSTXL-EDUMKII)
- 2x [Esp32-Wroom-32D](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
- 1x [Esp32-CAM](https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/DFR0602_Web.pdf)
- 4x DC Motors
- 2x L298N motor's driver
- 2x micro servomotor
- A 12 volt 1.5 Amp powersource (for the motor controller board)
- 2x 5 volt powersource (1 for the Esp-Cam, 1 for the Servomotors and Esp32)
  - If you want, you can use a voltage step down board (we did this due to time and budget costrains)
- 2x [rover track outer frame](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/ProtoTank_TrackOuterFrame.stl)
- 2x [rover track inner frame](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/ProtoTank_trackinnerFrame.stl)
- 2x [full rover tracks](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/track_1_5.stl)
- 6x [cog](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/ProtoTank_Cog.stl)
- [Rover base frame](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/ProtoTank_baseFrame.stl)
- 2x [L298N driver holder](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/L298N%20holder%20V3.stl)
- [Rover top frame](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/ProtoTank_TopFrame.stl)
- [Base for camera structure](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/CAMTANKFINAL.stl)
- [Pan & tilt arm base](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/Pan-tilt-plataforma_finale.stl)
- [Pan part for "turret"](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/Pan-tilt-tapa_finale.stl)
- [Tilt part for "turret"](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/platformafinalv2.3.stl)
- [Esp32-Cam holder](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/finalemnte%20v2.stl)
- Jumper wires of all types (M/F , M/M , F/F)
- A lot of screws and bolts
- 1 Usb-C cable
- PLA of two different colors ( one for tracks, one for everything else)

>[!IMPORTANT]
>
>The rover is designed to be customizable, our project is just an example.

**COMMUNICATION**

This project is divided into two modules: the **controller** and the **rover**. These modules are connected via Wi-Fi using two ESP32 microcontrollers. All the information originates from the MSP432 microcontroller, which sends it to **ESP1**. ESP1 then transmits the data to **ESP2**, which interprets the messages and sends instructions to the DC motors and servo motors on the rover.

To establish communication between the MSP432 and ESP1, we had three options: **I2C**, **UART**, and **SPI**. We chose **UART** because it requires fewer pins and is simpler to implement. Due to the pin layout of the MSP432, we are limited to using UART module **A2**, with pins **3.2 (Rx)** and **3.3 (Tx)**. 

The UART is configured to use the **SMCLK (Sub-Main Clock)** for the following reasons:
1. We are using a low baud rate of **9600**, which does not require a high-frequency clock.
2. Using SMCLK allows the **main clock (MCLK)** to operate independently, freeing it up for other tasks.
3. There is no need to use the main clock, as it is less power-efficient compared to SMCLK.

This setup ensures efficient and reliable communication between the MSP432 and ESP1 while optimizing power consumption and resource allocation.
#### Software
- [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE)
- [MSP432 DriverLib](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02)
- Arduino IDE

