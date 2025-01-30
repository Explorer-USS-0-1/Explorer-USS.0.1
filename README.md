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

#### Software
- [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE)
- [MSP432 DriverLib](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02)
- Arduino IDE

>[!IMPORTANT]
>
>The rover is designed to be customizable, our project is just an example.

## SETUP

#### Hardware setup

#### Software setup

## COMMUNICATION

This project is divided into two modules: the **controller** and the **rover**. These modules are connected via ESP-NOW protocol using two ESP32 microcontrollers. All the information originates from the MSP432 microcontroller, which sends it to **ESP1**. ESP1 then transmits the data to **ESP2**, which interprets the messages and sends instructions to the DC motors and servo motors on the rover. The connection between the ESP's is done using the libraries <esp_now.h> and <WiFi.h> in Arduino IDE. 

>[!IMPORTANT]
>
>To set up the ESP's connection you will need to know the IP address of the Rover's ESP. Here's a quick tutorial how: https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/

To establish communication between the MSP432 and ESP1, we had three options: **I2C**, **UART**, and **SPI**. We chose **UART** because it requires fewer pins and is simpler to implement. Due to the pin layout of the MSP432, we are limited to using UART module **A2**, with pins **3.2 (Rx)** and **3.3 (Tx)**, connected to pins **17 (TX)** and **16 (Rx)** respectively. 

The UART is configured to use the **SMCLK (Sub-Main Clock)** for the following reasons:
1. We are using a low baud rate of **9600**, which does not require a high-frequency clock.
2. Using SMCLK allows the **main clock (MCLK)** to operate independently, freeing it up for other tasks.
3. There is no need to use the main clock, as it is less power-efficient compared to SMCLK.

This setup ensures efficient and reliable communication between the MSP432 and ESP1 while optimizing power consumption and resource allocation.

### PIN LAYOUT

 Table 1: ESP32 connected to MSP432
| **ESP32 Pin** | **MSP432 Pin** |   
|---------------|----------------|
| GPIO16 (RX)   | P3.3 (TX)      | 
| GPIO17 (TX)   | P3.2 (RX)      |
| GND           | GND            |
| VIN           | 5V             | 


 Table 2: ESP32 connected to DC motors and servo motors
| **ESP32 Pin** | **DC motors**       | **Servo motors** | **Distribuition power** |
|---------------|---------------------|------------------|-------------------------|
| 21            | Motor1 Pin1         | -                | -                       |
| 33            | Motor1 Pin2         | -                | -                       |
| 16            | Motor2 Pin1         | -                | -                       |
| 17            | Motor2 Pin2         | -                | -                       |
| 18            | Motor3 Pin1         | -                | -                       |
| 19            | Motor3 Pin2         | -                | -                       |
| 22            | Motor4 Pin1         | -                | -                       |
| 23            | Motor4 Pin2         | -                | -                       |
| 27            | Motor1 SPEED        | -                | -                       |
| 26            | MOTOR2 SPEED        | -                | -                       |
| 14            | MOTOR3 SPEED        | -                | -                       |
| 25            | MOTOR4 SPEED        | -                | -                       |
| 13            | -                   | servoX (Signal)  | -                       |
| 32            | -                   | servoY (Signal)  | -                       |
| GND           | -                   | -                | GND                     |
| VIN           | -                   | -                | 5V                      |
| -             | Power               | -                | 12V - 1.5A              |
| -             | GND                 | -                | GND                     |
| -             | -                   | Power            | 5V                      |
| -             | -                   | GND              | GND                     |


The ESP32-CAM is directly powered by an independent powerbank.
The MSP-EXP432P401R is powered by an independent powerbank attached to it.


## SOFTWARE REPRODUCIBILITY

The MSP432 code is written in C and the porpose of it is to send the action that we entended the rover to make.
First thing first is to set up the IDE were we want to test our code, for this purpose we use Code Composer and to do it we have to download it from [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE), create a new project, select our board in the selection tab and give a name to the project.
The second step is download and extract the [MSP432 DriverLib](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02) and save where we want, then do the following action to setup the inviroment and be ready to test and run our code. 

This are the steps to follow to correctly use the DriverLib for the Educational BoosterPack MKII.
1. Extract simplelink_msp432p4_sdk_3_40_01_02.zip file.
2. Open CSS and left click on Project Folder to select Properties
3. Select CSS Build
4. Click ARM Compiler and then Include Options
5. Add "simplelink_msp432p4_sdk_3_40_01_02/source" directory to "Add dir to #include search path" window.  
6. Click ARM Linker and File Search Path
7. Add "simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" to "Include library file..." window

What the code do?
The code put in action the rover. In the code we have a machine state that pop up when we power up the board, it shows a white car and the frase "Welcome Back !" than when if we
want to start move the tank we have to press the S3 button (position right/bottom) of the controller, that will shows the state of the rover and will enable the movement. At this point is necessary only to move the joistick to make the rover move or press the S2 button (position right/top) of the controller to switch mode and make the camera move with the joistick.
All this is possible thanks to the handler, that will listen for the interrupt of an action at any moment. 

### PIN LAYOUT

 Table 3: MSP432 Pins Axis Joystick with Push Buttons
| **Pin Functions**  | **MSP432 Pin** |   
|---------------     |----------------|
| Hori X-axis        | P6.0	          | 
| Verti Y-axis       | P4.4           |
| Select button      | P4.1           |
| S2 button          | P5.1           | 
| S3 button          | P3.5           | 


