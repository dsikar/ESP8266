# Microphone sound detection module

Sound detection using the Keyes KY-037 sound detection module and the ESP8266 arduino compatible NodeMCU.

## Setting up

To run the example:

1. Wire up NodeMCU and KY-037 as per wiring diagram
2. Run sketch KY-037.ino - note there is an analogue and digital section, comment/uncomment as required
3. Monitor output via Serial Monitor
4. Adjust pot for threshold - note pot will turn built in led on and off helping to identify threshold

## Wiring diagram

```
                   NodeMCU                                                  KY-037
               +------------+                                            +----------+
               |            |                                            |          |
    A0 +-----+ |            | +-----+ D2          BLACK (to GND) +-----+ |          |
               |            |                                            |          |
               |            |                  RED (to D2 or A0) +-----+ |          |
               |            |                                            |          |
               |            |                   YELLOW (to 3.3V) +-----+ |          |
               |            |                                            |          |
               |            |                                            +----------+
               |            |
               |            | +-----+ GND
               |            |
               |    USB     | +-----+ 3.3V
               +------------+
```

## Node MCU pinout

![NodeMCU](https://pradeepsinghblog.files.wordpress.com/2016/04/nodemcu_pins.png?w=616)

## KY-037 pinout

Top
![KY-037 top](https://cdn.shopify.com/s/files/1/2252/3227/products/BDAA100116_Sound_20Detection_20Module_20Keyes_20Red_20inc_203_20Wire_1_e3658e00-5358-4918-a0cc-505a269b7709_x700.jpg?v=1504170216)

Bottom
![KY-037 bottom](https://cdn.shopify.com/s/files/1/2252/3227/products/BDAA100116_Sound_20Detection_20Module_20Keyes_20Red_20inc_203_20Wire_3_75ca706c-c804-4344-b9d4-a6e497b3c5a7_x700.jpg?v=1504170224)

## Product pages

[Sound detection module](https://fluxworkshop.com/products/bdaa100116-sound-detection-module-keyes-red-inc-3-wire)  
[NodeMCU](https://www.ebay.co.uk/i/162867196959?chn=ps&adgroupid=13585920426&rlsatarget=pla-142405565586&abcId=&adtype=pla&merchantid=6995734&poi=&googleloc=9046631&device=c&campaignid=207297426&crdt=0)

