<h1>Ground-Station-Firmware</h1>

<p>PCB: Flight Computer A0005</p>
<p>MCU: STM32H750VBT6 </p>
<p>MPU Architecture: ARM Cortex-M7</p>

<p>Description: The Ground Station Telemetry Unit adds wireless connectivity to the Raspberry-Pi 
based ground station computer. The unit is equipped with a socket for an XBee module, as well as a LoRa module.</p>

<p><b>Source Directories:</b></p>
<p>
blink:blinks status LED to test programmer and board setup 

reciever: Recieves wireless telemetry and outputs to USB 

transmitter: Transmits wireless telemetry recieved from USB

terminal: Remote interface to the terminal application 

hotfire: Firmware to be used during the hotfire of the liquid engine

flight: Firmware to be used with the flight computer
</p>

<p><b>Working Directory Structure</b></p>

<p>
doc: documentation

src: source code files and build files

lib: libraries for device drivers and external functions
</p>
