This arduino program displays the temperature and barometric pressure on an LCD display
The pressure is displayed in mBar and the temperature in Farenheight

It uses the BMP180 sensor to read the temperature and pressure.

It uses a serial enabled LCD kit for the display.
More info on this LCD kit here: https://github.com/jimblom/Serial-LCD-Kit/wiki/Serial-Enabled-LCD-Kit-Datasheet

It uses two push buttons to increase or decrease the screen brightness.


Information on making connections can be found below:

Connecting Sensor:
  Connect SCL to A5
  Connect SDA to A4
  Connect Vin to 3.3 VDC
  Connect GND to ground

Connecting Display:
  Connect RX to D11
  Connect Vin to 5 VDC
  Connect GND to ground

Connecting Buttons:
  Dim Up Button to D2 and ground (each side of button)
  Dim Down Button to D3 and ground (each side of button)

