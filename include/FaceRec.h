#define SYNCWORD1 0xEF
#define SYNCWORD2 0xAA
// Commands:
#define IDENT 0x12  // Identify
#define REGIS 0x13  // Register Face
#define DEL 0x20  // Delete Face
#define CLR 0x21  // Clear All Faces
#define BACKLIT 0xC0  // Backlight control
#define DISPL 0xC1  // Display Control
#define FLSH 0xC2 // Flash control
#define VERS 0x30 // Query Version
#define REBT 0xC3 // Reboot
#define BAUDRT 0x51 // Change Baudrate
#define NUMBERREC 0xC4  // Query number recorded entries
#define TURNON 0x01
#define TURNOFF 0x00
// Baudrates:
#define BAUD9600 0x00
#define BAUD19200 0x01
#define BAUD38400 0x02
#define BAUD57600 0x03
#define BAUD115200 0x04
// Responds from TX510
#define SUCC 0x00 // Successful
#define FAIL 0x01 // Failed
#define ANGL 0x03 // Angle failure
#define FAIL2D 0x06 // 2D failure
#define FAIL3D 0x07 // 3D failure
#define NOMATCH 0x08  // No mactching entry
#define EXIST 0x09  // Entry already exist

class FaceRec{
    private:
      uint8_t ByteLen = 12;
      byte Data[12];
      boolean rcvComplete = false;
      void paketReady();

    public:
      void sendBattery();
      void rcvRecData();
      void BTHomeInit(String device, bool encrypt=false, String bind_key="");
      void sendCommand(uint8_t command, uint8_t param1 = 0x00, uint8_t param2 = 0x00);
};