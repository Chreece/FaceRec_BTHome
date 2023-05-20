#define SYNCWORD1 0xEF
#define SYNCWORD2 0xAA

#define IDENT 0x12
#define REGIS 0x13
#define DEL 0x20
#define CLR 0x21
#define BACKLIT 0xC0
#define DISPL 0xC1
#define FLSH 0xC2
#define VERS 0x30
#define REBT 0xC3
#define BAUDRT 0x51
#define NUMBERREC 0xC4

#define BAUD9600 0x00
#define BAUD19200 0x01
#define BAUD38400 0x02
#define BAUD57600 0x03
#define BAUD115200 0x04

#define SUCC 0x00
#define FAIL 0x01
#define ANGL 0x03
#define FAIL2D 0x06
#define FAIL3D 0x07
#define NOMATCH 0x08
#define EXIST 0x09

#define TURNON 0x01
#define TURNOFF 0x00

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