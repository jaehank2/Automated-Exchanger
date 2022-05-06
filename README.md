<h1 align="center"> G2G Automated Exchanger </h1>

[Overivew](#overview)  
[PCB](#pcb-printed-circuit-board)  
[Components](#components)  
[Physical Design](#physical-design)


## Overview
An autonomous G2G-container exchanger is a machine for students at the University of Illinois to use with their iCards. It is a system designed to improve the current state of the dining hall. The G2G (good 2 go) system originally required human supervision to receive take-out containers. In exchange for returning these used take-out containers, students would receive a token to then use to get a new container. The G2G machine automates this process and offers another layer of direct exchange between new and old containers while digitalizing the tokens.

<p align = "center">
<img src = "https://cdn.discordapp.com/attachments/903401697957789716/971312833566486598/retrieve.gif">
<img src = "https://cdn.discordapp.com/attachments/903401697957789716/971312807532458034/dispense.gif">
</p>

<p align = "center">
 <strong>Retrieval (Left) and Dispensing (Right)</strong>
</p>


**Contributors**:  
Ariocie Liang (arliang2), Henry Guan (henryg3)


## PCB (Printed Circuit Board)
The PCB schematic includes 3 microcontrollers (1 ATmega, 2 ATtinys), a ESP8266 WiFi module, HCT138 Demux, and pin layouts for other [components](#components)

pcb layout                                                     | pcb schematic
---------------------------------------------------------------|-------------------------------------------------------------
<img src="/images/pcb_schematic.png" width="350" height="500"> | <img src="/images/pcb_layout.png" width="300" height="300">

## Components
* Sensor Subsystem
  * QR scanner module
  * Loadcell
  * MSR90 card reader
* Control Subsystem
  * Continuous servo motor
  * Non-continuous servo motor
* UI Subsystem
  * Maintain-contact push buttons (2)
  * LEDs (6)
  * LCD screen (optional)
* Power Subsystem
  * AC-DC converter (120V -> 12V)
  * Voltage regulator (12V -> 5V)

## Physical Design

<p align="center">
 <img src="/images/front_view.png" width="300" height="300">
 <img src="/images/topdown_view.png" width="300" height="300">
 <img src="/images/back_view.png" width="300" height="300">
</p>

<p align="center">
 <strong>Front View (Left), Top-down View (Center), Back View (Right)</strong>
</p>
