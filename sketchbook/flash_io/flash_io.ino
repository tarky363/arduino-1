

#include <flash.h>
#include <bencode.h>
#include <radionet.h>

  /*
   *
   */

// needed by the watchdog code
EMPTY_INTERRUPT(WDT_vect);

    /*
     *  I2C Interface.
     */

static PinIo d4 = PinIoD(4);
static PinIo a0 = PinIoC(0);

static I2C i2c = {
    & d4,   //  SDA
    & a0,   //  SCL
    0x50 << 1,
};

static FlashIO flash = { & i2c, };

static Bencode parser;
static Packet packet;

#define RD_LED 6
#define WR_LED 7
#define XX_LED 9

static int rd_led, wr_led, xx_led;

static void poll_led(int pin, int* t)
{
    if (*t) {
        *t -= 1;
        if (!*t) {
            digitalWrite(pin, HIGH);
        }
    }
}

static void set_led(int pin, int* t)
{
    digitalWrite(pin, LOW);
    *t = 1000;
}

static void init_led(int pin)
{
    pinMode(pin, OUTPUT);    
    digitalWrite(pin, HIGH);
}

    /*
    *
    */

static void send_fn(const void* data, int length)
{
    //Serial.print("send(");
    //Serial.print((int)data);
    //Serial.print(",");
    //Serial.print(length);
    //Serial.print(")\r\n");
    set_led(WR_LED, & wr_led);
    uint8_t node = 0xaa;
    Bencode::to_host(node, (const uint8_t*) data, length);
}

static void debug_fn(const char* text)
{
    //Serial.print(text);
}

static void decode_command(uint8_t* data, int length)
{
    Message msg(data);

    set_led(RD_LED, & rd_led);

    uint8_t* payload = (uint8_t*) msg.payload(); 
    uint8_t cmd = 0;
    if (msg.extract(Message::FLASH, & cmd, sizeof(cmd))) {
        flash_req_handler(& flash, cmd, payload);
        return;
    }
}

void setup()
{
    Serial.begin(57600);

    flash_init(& flash, debug_fn, send_fn);
    packet.reset();

    init_led(RD_LED);
    init_led(WR_LED);
    init_led(XX_LED);
}

void loop()
{
    if (Serial.available()) {
        set_led(XX_LED, & xx_led);
        if (parser.parse(& packet, Serial.read())) {
            decode_command(packet.data, packet.length);
            parser.reset(& packet);
        }
    }

    poll_led(RD_LED, & rd_led);
    poll_led(WR_LED, & wr_led);
    poll_led(XX_LED, & xx_led);
}

// FIN
