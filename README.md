<h1>Ground-Station-Firmware</h1>

<p>PCB: Flight Computer A0005</p>
<p>MCU: STM32H750VBT6 </p>
<p>MPU Architecture: ARM Cortex-M7</p>

<p>Description: The Ground Station Telemetry Unit adds wireless connectivity to the Raspberry-Pi 
based ground station computer. The unit is equipped with a socket for an XBee module, as well as a LoRa module.</p>

<p><b>Source Directories:</b></p>
<p>
blink:blinks status LED to test programmer and board setup 

flight: firmware to be run during normal usage 

terminal: firmware to allow terminal access to all PCB hardware 
</p>

<p><b>Working Directory Structure</b></p>

<p>
doc: documentation

src: source code files and build files

lib: libraries for device drivers and external functions
</p>
