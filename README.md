# <p align="center"> EXPLORER USS.0.1 </p>

<p align="center">
<img src="https://github.com/user-attachments/assets/65f4ffe8-abba-4ce2-b804-021d666b2570" width=50% height=50%> 
</p>


===========================================================================

The goal of this project is to create and learn how a exploration rover can be, how can be built and what we can do with it. We liked the idea to do something that could move everywhere, on every type of soil and very far away, so we designed and created our personal rover from scratch.

Because we couldn't see it everywhere, we gave to the rover an eye (a camera), so we can see what it sees.

Maybe is not a professional rover, or a competitive one, but for sure with a very limited budget and almost zero starting knowledge, we learned a lot about embedded software and IOT.

>[!Note]
>
>In the print file folder there are a lot of files, cause during the desing phase we scraped and redesinged a lot of time the rover prototype.


## REQUIREMENTS

#### Hardware

To build and use this project, you need the following hardware:

- 1x Texas Instruments MSP432P401R microcontroller
- 1x [Educational BoosterPack MKII](https://www.ti.com/tool/BOOSTXL-EDUMKII)
- 2x [Esp32-Wroom-32D](https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf)
- 1x [Esp32-CAM](https://media.digikey.com/pdf/Data%20Sheets/DFRobot%20PDFs/DFR0602_Web.pdf)
- 4x DC Motors
- 2x L298N motor's driver
- 2x micro servomotor
- A 12 volt 3.5 Amp powersource (for the motor controller board)
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
- [Power System Holder](https://github.com/Dennis-Alberti/IoT-project/blob/main/Rover_chassis_and_tools_printfiles/power%20supply%20holder%20v4.stl)
- Jumper wires of all types (M/F , M/M , F/F)
- A lot of screws and bolts
- 1 Usb-C cable
- PLA of two different colors ( one for tracks, one for everything else)
<p align="center">
  <img src="https://github.com/user-attachments/assets/b9918c82-8b12-4716-a93a-99824b7287e7" width=15% height=15%>

  <img src="https://github.com/user-attachments/assets/5cdeec9a-8a6c-414c-9cd5-5fc2740a3cf2" width=30% height=30%>

  <img src="https://github.com/user-attachments/assets/39ba9576-ec70-47a5-b851-bb9a8926c996" width=20% height=20%>
</p>


#### Software
- [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE)
- [MSP432 DriverLib](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02)
- Arduino IDE

>[!IMPORTANT]
>
>The rover is designed to be customizable, our project is just an example.

## SETUP

### Hardware setup

The hardware setup requires time and planning, because **all the structural part** of the rover are **3D printed**. We decided to do this to have a light but very strong structure.

>[!Note]
>
>The prints of our project were made with 100% reciled PLA.

#### ROVER

After printing all the files linked in the [**hardware requirements**](#REQUIEREMENTS) section ( or any other file in the [Rover_chassis_and_tools_printfile](https://github.com/Dennis-Alberti/IoT-project/tree/main/Rover_chassis_and_tools_printfiles) folder), you can start build up the chassis in this order:
1. Build the track frames (whit the cogs), and put the DC motors in them;
   <p align="center">
      <img src="https://github.com/user-attachments/assets/a3ed375e-4350-420f-856f-41f0282823ee" width=50% height=50%>  

2. Put the track frames on the baseframe, using screws and bolts to secure the 2 parts, and then put the L298N driver holders in the holes of the base (they should perfectly fit);
3. Use screw and bolts to secure the L298N divers to their holder frames; also connect the motors to the drivers and be sure that the opposing engines have the color order of the cables reversed. At the same time be sure that the motors sharing the track frame have the same cables color order;
    <p align="center">
      <img src="https://github.com/user-attachments/assets/0adfb10b-93e5-46bb-99fb-dd0abeab3f99" width=40% height=40%>  
    </p>
4. Use the F/F jumper wires to connect all the motor speed pins to the ESP32-2 (displayed in the [**PIN LAYOUT**](#PIN_LAYOUT) section, ESP32-2 table);
5. Close the chassis whit the rover top frame, then put the tracks in place;
6. Put inside the ESP32-Cam holder the ESP32-Cam, and then build up the "trurret" structure with the servomotors;
    <p align="center">
      <img src="https://github.com/user-attachments/assets/d7ae5354-3afa-43f3-bd1f-aad0ffc31e94" width=30% height=30%>
    </p>
8. Mount on the rover chassis the "turret" and connect to the Esp32-2 the servomotors (following the same table of the point 4);
    <p align="center">
      <img src="https://github.com/user-attachments/assets/8f971599-abff-492b-ba06-9f16f164475b" width=40% height=40%> 
      <img src="https://github.com/user-attachments/assets/cd103219-1910-41a4-b3d5-4605beed4cd2" width=30% height=30%> 
    </p>
9. Now is possible put any additional accessory or tool ( from our printfile folder or any custom-made one ).

>[!IMPORTANT]
>
>**To power up everything the same way we did it, check out the [POWER SYSTEM MANAGEMENT](#POWER_SYSTEM_MANAGEMENT) section**

#### JOYSTICK

Connect the MSP432P401R microcontroller whit the ESP32-1 using F/F Jumper wires, following the [ESP32-1 table pin layout](#PIN_LAYOUT).
  <p align="center">
   <img src="https://github.com/user-attachments/assets/1ccb45c1-9b9d-47cd-8836-0c30a3ca162b" width=40% height=40%>
  </p>

### Software setup

##### Msp432 Setup
1. Go to the section below (MSP432 DriverLib Setup) to setup the DriverLib to be able to use the [Educational BoosterPack MKII](https://www.ti.com/tool/BOOSTXL-EDUMKII).
2. Connect the MSP with a computer.
3. Go to [`Rover/MSP + ESP FINAL/main.c`](Rover/MSP%20%2B%20ESP%20FINAL/main.c), [`Rover/MSP + ESP FINAL/functions.h`](Rover/MSP%20%2B%20ESP%20FINAL/functions.h) and [`Rover/MSP + ESP FINAL/functions.c`](Rover/MSP%20%2B%20ESP%20FINAL/functions.c) and download the files.
4. Open it in the projects previously created at point 1.
5. Upload and run the code.

##### Esp32-1 Setup  
1. Connect the ESP with a computer.  
2. Go to [`Rover/Final/EspSender.ino`](Rover/Final/EspSender.ino) ,and download the file.   
3. Open it with Arduino IDE and change the variable `broadcastAddress[]` with the address of the ESP connected to the rover (*see [Communication Section](#COMMUNICATION)*).  
4. Upload and run the code.  

##### Esp32-2 Setup  
1. Connect the ESP with a computer.
2. Go to [`Rover/Final/DcandServo.ino`](Rover/Final/DcandServo.ino), and download the file. 
3. Open it with Arduino IDE and install the library: **"ESP32Servo"** by *Kevin Harrington, John K. Bennett*.  
4. Upload and run the code.

##### Esp32-Cam Setup
1. Connect the Esp-cam whit a computer.
2. Go to [`Rover/Final/EspSender.ino`](Rover/Esp32Cam) ,download all the files in the folder and create a folder with all of them inside.
3. Open the CameraWebServerEsp32Cam.ino file with Arduino IDE. In the "const char *ssid" and "const char *password" variables (line 39 and 40 of the code) put the name and the password of the wi-fi you're going to use to host the webserver;
4. Upload and run the code.
5. Only for the firs time (or everytime you change the host wi-fi), you have to copy the IP ADRESS ,printed in output on the Serial monitor of your pc, and paste it on a browser tab. 

Once you have ensured the connection between the 2 ESP32 (see [COMMUNICATION](#COMMUNICATION) section to know how), and the webserver for the ESP32-CAM is working, you can start to use the rover.
   
## HOW TO USE

The MSP432 code put in action the rover. The initial state shows a white car and the phrase "Welcome Back !". 

<p align="center">
<img src="https://github.com/user-attachments/assets/3f374e63-f392-430f-9573-3f1e41f39e16" width=50% height=50%>
</p>

To start moving the tank, press the S3 button (position right/bottom of the controller), that will put the joystick in TANK mode, displaying the current state on the screen. Now you can move the rover using the joystick.
* UP: move the rover forward.
* DOWN: move the rover backward.
* RIGHT: move the rover right.
* LEFT: move the rover left

<p align="center"> 
<img src="https://github.com/user-attachments/assets/8842cbce-7701-402d-b01e-eaa041b66429" width=50% height=50%>
</p>

To use the camera, press the S2 button (position right/top of the controller) to switch that will put the joystick in CAMERA mode, and use the joystick to point in the wanted direction. 
* UP: move the camera up.
* DOWN: move the camera down.
* RIGHT: move the camera right.
* LEFT: move the camera left.
* JOYSTICK BUTTON: reset the camera position to the center.

Pressing the S2 button will the switch again to the TANK mode, and so on.
All this is possible thanks to the handler, that will listen for the interrupt of an action at any moment and send the message to the Esp32-1.

Now you can use EXPLORER USS.0.1 to go everywhere you want, and see what your eyes could never!

>[!Note]
>
> Is possible to test every part of the project separatley

## COMMUNICATION

This project is divided into two modules: the **controller** and the **rover**. These modules are connected via ESP-NOW protocol using two ESP32 microcontrollers. All the information originates from the MSP432 microcontroller, which sends it to **ESP32-1**. ESP32-1 then transmits the data to **ESP32-2**, which interprets the messages and sends instructions to the DC motors and servo motors on the rover. The connection between the ESP's is done using the libraries <esp_now.h> and <WiFi.h> in Arduino IDE. 

The decision to use the ESP-NOW protocol is due to the reliability and long connection distance it can maintain.

>[!IMPORTANT]
>
>To set up and use ESP-NOW you need to know the IP address of the Rover's ESP32. Here's a quick tutorial how: https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/

To establish communication between the MSP432 and ESP32-1, we had three options: **I2C**, **UART**, and **SPI**. We chose **UART** because it requires fewer pins and is simpler to implement. Due to the pin layout of the MSP432, we are limited to using UART module **A2**, with pins **3.2 (Rx)** and **3.3 (Tx)**, connected to pins **17 (TX)** and **16 (Rx)** respectively. 

The UART is configured to use the **SMCLK (Sub-Main Clock)** for the following reasons:
1. We are using a low baud rate of **9600**, which does not require a high-frequency clock.
2. Using SMCLK allows the **main clock (MCLK)** to operate independently, freeing it up for other tasks.
3. There is no need to use the main clock, as it is less power-efficient compared to SMCLK.

This setup ensures efficient and reliable communication between the MSP432 and ESP1 while optimizing power consumption and resource allocation.

## PIN_LAYOUT

 Table 1: ESP32 connected to MSP432
| **ESP32 Pin** | **MSP432 Pin** |   
|---------------|----------------|
| GPIO16 (RX)   | P3.3 (TX)      | 
| GPIO17 (TX)   | P3.2 (RX)      |
| GND           | GND            |
| VIN           | 5V             | 


 Table 2: ESP32 connected to DC motors and servo motors
| **ESP32 Pin** | **DC motors**       | **Servo motors** | **Power System**        |
|---------------|---------------------|------------------|-------------------------|
| 21            | Motor1 Pin1         | -                | -                       |
| 33            | Motor1 Pin2         | -                | -                       |
| 16            | Motor2 Pin1         | -                | -                       |
| 17            | Motor2 Pin2         | -                | -                       |
| 18            | Motor3 Pin1         | -                | -                       |
| 19            | Motor3 Pin2         | -                | -                       |
| 22            | Motor4 Pin1         | -                | -                       |
| 23            | Motor4 Pin2         | -                | -                       |
| 27            | MOTOR1 SPEED        | -                | -                       |
| 26            | MOTOR2 SPEED        | -                | -                       |
| 14            | MOTOR3 SPEED        | -                | -                       |
| 25            | MOTOR4 SPEED        | -                | -                       |
| 13            | -                   | servoX (Signal)  | -                       |
| 32            | -                   | servoY (Signal)  | -                       |
| GND           | -                   | -                | GND                     |
| VIN           | -                   | -                | 5V                      |

 Table 3: MSP432 Pins Axis Joystick with Push Buttons
| **Pin Functions**  | **MSP432 Pin** |   
|---------------     |----------------|
| Hori X-axis        | P6.0	          | 
| Verti Y-axis       | P4.4           |
| Select button      | P4.1           |
| S2 button          | P5.1           | 
| S3 button          | P3.5           | 


## MSP432 DriverLib Setup

First thing first set up the IDE , for this purpose use [Code Composer Studio](https://www.ti.com/tool/CCSTUDIO) Integrated Development Environment (IDE) and:
- Create a new project, select the MSP432P401R board in the selection tab and give a name to the project.
- Download and extract the [MSP432 DriverLib](https://www.ti.com/tool/download/SIMPLELINK-MSP432-SDK/3.40.01.02) and save in a reachable directory, then do the following action to setup the environment:
   
1. Extract simplelink_msp432p4_sdk_3_40_01_02.zip file.
2. Open CCS and left click on Project Folder to select Properties
3. Select CCS Build
4. Click ARM Compiler and then Include Options
5. Add "simplelink_msp432p4_sdk_3_40_01_02/source" directory to "Add dir to #include search path" window.  
6. Click ARM Linker and File Search Path
7. Add "simplelink_msp432p4_sdk_3_40_01_02/source/ti/devices/msp432p4xx/driverlib/ccs/msp432p4xx_driverlib.lib" to "Include library file..." window

## POWER_SYSTEM_MANAGEMENT

The power system of our rover it's composed by 4 modules:
1. A 12 Volt 3.5 Ah module to power up the L298N drivers for DC motors. This was obtained by putting in series 8 batteries of 1.5 Volt;
2. A 9 Volt battery connected to a step down volatge board  to power up the servomotors and the ESP32-2;
3. A 5 Volt output powerbank to power up the ESP32-Cam ( we decided to do this because the camera drains a lot of power)
4. A 5 volt output powerbank to power up the MSP432 and the connected ESP32-1. 

The first 2 modules are powering up motors connected and controlled by the ESP32-2, so they have to share in common the ground. To do it, we placed the stepdown voltage board on a breadboard using the external channels, which are connected per row throughout the breadboard.
In this way we made a row where the electricity output of the step down board was off, putting instaed the external 12 Volt module , and connecting in the parallel row the grounds from the ESP32-2, from the L298N drivers and from the servomotors. In the other external row we setted a 5V electricity output so we could power up the ESP32-2 trough the VIN pin and the 2 servomotors ( the ESP32 pinout is already described in the [*Communication*](#COMMUNICATION) section).
<p align="center">
<img src="https://github.com/user-attachments/assets/79885017-e6bd-48d6-806f-7ef1022e6a69" width=50% height=50%>
</p>

Table 4: Rover power system management  
| **DC motors**       | **Servo motors** | **Stepdown board**      |
|---------------------|------------------|-------------------------|
| Power               | -                | 12V - 3.5A (output off) |
| GND                 | -                | GND                     |          
| -                   | Power            | 5V  (output on)         |
| -                   | GND              | GND                     |

## Link to powerpoint and Youtube video
- Youtube https://youtu.be/4N75zm8ps9I
- Powerpoint

## ISSUES.

In this section we want to explain the major difficulties encountered during the project

One of the main drawback was using the MSP432 as a controller. In fact we noticed that the command input had some lag, approximately 500ms. This was also due to the UART connection between the MSP432 and ESP32, followed by the connection via the ESP-NOW protocol. Also using the MSP432 boosterpack limited the possibility to have simultameous movement of camera and rover. But we decided to use the board anyway to challenge us into understanding more complicated boards than an Arduino.

Another problem was the lack of knowledge in the field of electronic engeniering. In fact we had no prior knowledge about the hardware and power management parts, so we had to study from zero how these worked.

The last drawback was the limited budget, that forced us to use low-cost hardware.

## FUTURE IMPROVEMENTS

- Rebuilding the power system, using only a rechargable Lipo battery to power every part of the rover;
- Upgrading the quality of the sensors, of the motors and of the camera;
- Using only 2 ESP's, one for the controller and one for the movement;
- Implementing simultaneous movement of camera and rover, and also freer movements;
- Implementing an automated movement system using ultrasonic sensors and algorithms;
- Implementing an object detection project using the camera;
- Implementing an automated garage for the rover, working as an homestation where it can recharge;
- Implementing some led light, to use the rover also in darker places.

## TEAM MEMBERS

The project was developed in a team and the code was divided into many different areas (Rover, Communications, Graphics, Hardware, ...); each member focused therefore only on certain aspects of the project.

- Riccardo Ferro (Leader)
  - Contributed mainly to the creation of the physical part of the rover, desining the layout of the chassis, organizing the power system and setting up all the connections of the rover. He also contributed a lot to write the ESP32-2 (for motor control) and also the ESP32-1 communication part about the ESP-NOW protocol. He also setted-up the ESP-32 Camera.

- Henrique Graça
  - Contributed mainly to: setup the ESP32-2 connection of the rover; write the code of the ESP32-2 and ESP32-1; UART connection between the MSP432 and ESP32-1. 

- Dennis Alberti
  - Contributed mainly in the creation of the MSP432 code.

- Tiago Silva
  - Contributed mainly in the creation of the MSP432 code.





