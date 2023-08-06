# FaceRec - BTHome (BETA)  - A simple, local and secure face recognition project.

# DISCLAIMER
The module can be easily manipulated to erase and add faces with physical buttons. Please make sure you don't expose them.
DON'T USE THIS PROJECT TO CONTROL SECURITY MEASURES AS LOCKS etc.

It utilizes the [TX510](https://www.hlktech.net/index.php?id=1077&cateid=761) kit module that contains a 3d Camera, a display and a flash.
Build on an Firebeetle ESP32 because of battery support but since this project isn't yet battery friendly, it could be build with any ESP32.
For power saving it uses the ultra cheap ($1.5) and efficient (70uA) 5G mmwave sensor [HLK-LD012-5G](https://hlktech.net/index.php?id=451).
The results will be sent to [Home Assistant](https://www.home-assistant.io/) (HA) via Bluetooth on [BTHome](https://bthome.io/) Integration, using the [BTHome example](https://github.com/Chreece/BTHomeV2-ESP32-example)

# For security reasons please change the BindKey in src/main.cpp

# HOW DOES IT WORK?
If the mmwave sensor detects presence, wakes up the ESP32 and turns on the display on TX510.
Then every 3 sec it starts the recognition procedure and sends a presence entity to HA along with a count sensor.
The count sensor gives the UserId that was recognised (or stays on 0) and the presence sensor changes to on or off if the recognition was successful.

To register new faces (up to 1000 according to docs), you can press the S1 button on the TX510. The UserIds start from 0 (first face registered = 0).

# HOW TO USE?
1) Clone the repository
2) Add in to platformio
3) Change the bind key or remove the encryption (not secure)
4) Change the ESP32 board in platformio.ini if you don't use the Firebeetle32 (you shouldn't)
5) Compile & Upload
6) Register new faces
You can start using it!

# In folder resources you can find the English firmware + manuals and test tools
Please read the comments in the code.

# Battery operation update:
* TX510 needs 5V power and ESP on battery (3.7V LiPo) can provide max Vbat, DC step up is needed
* OUT of the mmWave sensor can't power anything, a relay or latching circuit is needed
Connected a 3V relay on the OUT of the mmWave that powers a DC step up converter, giving all the necessary 5V for TX510 and ESP.

After some identification tries (5-6) the voltage of the battery drops under 3.2V which is not enough for the DC step up, which outputs 2,3V and the TX510 freezes.

I'm not sure what causes the voltage drop (maybe the load is too big to handle and battery shuts off), but that's a no-go for this setup.

If someone has an idea to imporove it please feel free to share!

I will give an update if something changes...

Build on [Platformio](https://platformio.org/)
