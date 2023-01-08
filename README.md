<h1>Ground-Station-Firmware</h1>

<p>PCB: Flight Computer A0005</p>
<p>MCU: STM32H750VBT6 </p>
<p>MPU Architecture: ARM Cortex-M7</p>

<p>Description: The Ground Station Telemetry Unit adds wireless connectivity to the Raspberry-Pi 
based ground station computer. The unit is equipped with a socket for an XBee module, as well as a LoRa module.</p>

<h2>Working Directory Structure</h2>

<p>
app: application code for the flight computer containing source directories

auto: auto-generated code from STM32CubeMX (not compiled into application)

init: Microcontroller initialization and configuration code

lib: third-party libraries for device drivers and middleware, microcontroller pin and peripheral configurations 

mod: Hardware modules containing hardware specific code for SDR boards

test: Test code
</p>

<h2>Source Directories:</h2>
<p>
blink:blinks status LED to test programmer and board setup 

flight: Firmware to be used with the flight computer

hotfire: Firmware to be used during the hotfire of the liquid engine

reciever: Recieves wireless telemetry and outputs to USB 

transmitter: Transmits wireless telemetry recieved from USB

terminal: Remote interface to the terminal application 
</p>
