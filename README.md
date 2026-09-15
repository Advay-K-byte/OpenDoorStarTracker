
# OpenDoorStarTracker
An extremely budget friendly barn-door based star tracker, based on ESP32 and 28BYJ stepper motor.

This project is aimed to reduce the cost of entry level astrophotography, by providing a simple, highly open source and customizable barn-door star tracker base design.

Features:
  - ~$30 total cost
  - ESP32 webpage controller with easy slewing adjustment, resetting and tracking speed adjustments
  - Powered 100% with a USB battery

OnShape full cad model: https://cad.onshape.com/documents/74c63627717610043aa96174/w/ea7d6111b83551bf0402bd96/e/efb1aef8f325fa2de03cf594 

BOM: https://docs.google.com/spreadsheets/d/1jzOKhSsxTXtr95Q2sw8j4dyO4oUSdCoERQhsF2ek0W0/edit?usp=sharing

YOUTUBE VIDEO: https://youtu.be/rEIXzPVLKTk?si=SgSztPYLGnnmBQIM

<img width="1280" height="720" alt="DIY Telescope (1)" src="https://github.com/user-attachments/assets/a3b90546-4a51-4086-b24c-def3754effcc" />
<img width="3024" height="4032" alt="fullTracker" src="https://github.com/user-attachments/assets/4119238f-ebe6-4511-a58f-abe160add365" />
<img width="1078" height="906" alt="image" src="https://github.com/user-attachments/assets/b3dc1b0c-d716-4b92-b797-7eb9a7f3dc31" />

Camera Mount Printables link: https://www.printables.com/model/81126-tripod-ball-head
Thank you zx82net, @zx82net_107245 on Printables!
# Motion:
The OpenDoor Star Tracker uses a barn-door tracker design with a ¼ inch (20TPI) screw and nut rotating at 1 rpm to move at the sidereal rate.
- MOTOR: 28-BYJ Stepper motor
- SCREW: 1/4" x 20Teeth per Inch Screw hand curved
- GEARS: 5:1 3d printed gear with pushed in nut


# Electronics:
- DRIVER: ULN2003 Driver
- CONTROLLER: ESP32 board with wifi
- POWER: Any USB Battery Bank
- ATTACHMENT: You can solder the driver to the ESP32 to be more compact using a perfboard, like I did, or use normal Arduino jumper wires
- Lots of opportunity for expansion

# WiFi Controller Software:
  - Speed adjustment (automatically saves to esp32 preferences so you dont have to change it every time)
  - Position adjustment (always live updating position value as the tracking moves)
  - Slewing
  - Automatic zero reset
  - Manual zero position setting
<img width="660" height="734" alt="ControllerProgram" src="https://github.com/user-attachments/assets/389ff8c8-9533-42b7-948b-996d3458b4d1" />

I also controlled my camera wirelessly through the Camera Control software made for olympus cameras. If you want your rig to be fully 100% automated, I highly reccomend to use a wire, or wireless setup connected to a laptop running a program, or get an intervalometer.
It feels way more advanced if you do not have to touch anything, and it likely improved image quality, as you are not touching the easily moveable or sensitive ball head.


# Polar Alignment:
  - SCOPE: I recommend to use a cheap toy refractor, and lock the focus at infinity. Can be bought from classified or any old small refractor.
  - ALTERNATIVES: You can simply aim polaris using just the line of the hinge, or add a straw or circle hooks
  - Can be adjusted using the EQ Wedge for alt, and the main base for az.

# Camera Mount:
The Camera Ball Head is fully 3d printed. Thank you to user zx82net, @zx82net_107245 on Printables! I modified the base to make it an easier angle at my latitude to point the camera anywhere.
I mounted this, on the side of the mount that is higher up, to offset the weight of the rest of the tripod so the eq wedge has less force on it, and also to allow for better southern aiming, and general zenith aiming.

# EQ Wedge:
The EQ wedge is fully 3d printed, other than a few hinge screws. my size is for 1/4" but you can adjust in cad using step source cad.
I added some sandpaper inbetween for additional camera safety, but it should work without it.

I managed to get a really good deal on this Bushnell NorthStar 114/900 scope and tripod for 10 USD on Facebook Marketplace. I would highly recommend finding some better tripod than a photography tripod, as this setup can weigh a lot more than just a DSLR camera.
<img width="1242" height="1497" alt="NorthStar" src="https://github.com/user-attachments/assets/990662a4-21a4-4241-8c87-6d8103f56252" />

# Results:
This is my very first acceptable Milky Way shot:
  - Cygnus Region
  - ~35min exposure
  - 30 second exposures
  - Olympus OMD EM10 at 14mm kit lens at f/4.5
  - Bortle 6 light pollution

I will get much better photos soon, when I have time.

# Prototyping:
I went through many different versions, originally starting with a arduino mega, as it was all I had, and using a barely mounted straight screw. It was very bad, and I only got up to 10 second exposures. In this design, there was no polar scope, only a cheap photography tripod for adjustment. The second version slowly got better, adding a scope, and improving the code with the arduino. The final V3, uses and ESp32 and curved screw for best tracking while very little additional cost.
<img width="1242" height="1790" alt="workingPrototype" src="https://github.com/user-attachments/assets/7c2be2b2-acd6-4745-9454-4f01bc8080f6" />
