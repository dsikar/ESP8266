# Wiring up Catalex MicroSD Card Adapter to NodeMCU

Code is taken from the Arduino SD listfiles example. To work with the NodeMCU, some changes in the wiring is required, as well as powering the Catalex MicroSD Adapter with 5V.

## Wiring Diagram
```
    NodeMCU                                           Catalax MicroSD
+-------------+                                      +--------------+
|             |                                      |              |
|      D5(14) | +--------+ SCK         D8   +------+ | CS           |
|             |                                      |              |
|      D6(12) | +--------+ MISO        D5   +------+ | SCK          |
|             |                                      |              |
|      D7(13) | +--------+ MOSI        D7   +------+ | MOSI         |
|             |                                      |              |
|      D8(15) | +--------+ CS          D6   +------+ | MISO         |
|             |                                      |              |
|      GND    | +--------+ GND         5V   +------+ | VCC          |
|             |                                      |              |
|             |                        GND  +------+ | GND          |
+-------------+                                      |              |
                                                     +--------------+
```

## Code

[Arduino Sketch](mySD-card.ino)


