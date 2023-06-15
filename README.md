# Freetronics LED pannel displaying MQTT messages via an ESP32.


#### EP32 Pin out

Pin out for ESP32

Note ESP Pin 18 is actually SCK its named incorrect on the pin header
|Arduino    |DMD Header   |ESP Pin| 
|-----------|-------------|-------|
|AREF       |REF A        |       |
|GND        |GND          |GND    |
|D13        |CLK          |18     |
|D12        |D12          |       |
|D11        |R            |23     |
|D10        |D10          |       |
|D9         |NOE          |14     |
|D8         |SCLK         |25     |
|           |             |       |
|D7         |B            |26     |
|D6         |A            |27     |

