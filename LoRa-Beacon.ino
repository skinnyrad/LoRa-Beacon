#include <SPI.h>
#include <RH_RF95.h>


// Check for the various typs of devices

#if defined (__AVR_ATmega32U4__)  // Feather 32u4 w/Radio
  #define RFM95_CS    8
  #define RFM95_INT   7
  #define RFM95_RST   4

#elif defined(ADAFRUIT_FEATHER_M0) || defined(ADAFRUIT_FEATHER_M0_EXPRESS) || defined(ARDUINO_SAMD_FEATHER_M0)  // Feather M0 w/Radio
  #define RFM95_CS    8
  #define RFM95_INT   3
  #define RFM95_RST   4
  
#endif


// ################################### Configuration ###################################

#define FREQ 915.0 // Transmission Frequency, generally between 868 and 920 
#define PWR 20 // Power between 2 (weakest) and 20 (strongest)
#define BANDWIDTH 125000 //Bandwidth Options: 7800,10400,15600,20800,31250,41700,62500,125000,250000,500000
#define CR 5 // Setup Coding Rate Options: 5 (4/5), 6 (4/6), 7 (4/7), or 8 (4/8)
#define SF 7 // Spreading Factor: Valid values are 6 through 12
uint8_t message[] = "Device ID: 0001; Status: Active;"; // Message you want to beacon
// ################################### Configuration ###################################

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  Serial.begin(115200);
  while (!Serial) delay(1);
  delay(100);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }


  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Freq set to: "); Serial.println(FREQ);  // Default is 915.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, (sf7) Sf = 128chips/symbol, CRC on
  rf95.setTxPower(PWR, false); //
  rf95.setSignalBandwidth(BANDWIDTH);
  rf95.setCodingRate4(CR);
  rf95.setSpreadingFactor(SF);

}

void transmit(uint8_t* msg, unsigned int msg_length){
  Serial.println("Transmitting...");
  rf95.send(msg, msg_length);
  rf95.waitPacketSent();
}


// Basic Example of how to do encryption.  Modify as needed.
void encryptedTransmit(uint8_t* msg, unsigned int msg_length){
  uint32_t key = 0xDA94E6E2; // Corrected key to 32-bit
  uint8_t encryptedMsg[msg_length];

  // XOR encryption
  for(unsigned int i = 0; i < msg_length; i++){
    // Use the i-th byte of the key for XOR operation. This cycles through the 4 bytes of the key
    encryptedMsg[i] = msg[i] ^ ((key >> (8 * (i % 4))) & 0xFF);
  }

  // Transmit the encrypted message
  Serial.println("Transmitting encrypted message...");
  rf95.send(encryptedMsg, msg_length); // Use the encrypted message length
  rf95.waitPacketSent();
}



void loop() {
  // This code will run every second1
  transmit(message, sizeof(message));
  delay(1000); 

  // Example of how to send an encrypted message instead
  //encryptedTransmit(message, sizeof(message)); 

}
