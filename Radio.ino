#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
//#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 

RF24 radio(9, 10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };


void setup_radio() {
//    printf_begin();

  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);

  // optionally, reduce the payload size.  seems to improve reliability
  radio.setPayloadSize(8);

  radio_open();

  // Start listening
//  radio.startListening();

  // Dump the configuration of the rf unit for debugging
//  radio.printDetails();
}

void radio_close() {
  radio.powerDown();
}

void radio_open() {
  // Open pipes to other nodes for communication
  radio.openWritingPipe(pipes[0]);
//  radio.openReadingPipe(1, pipes[1]);
}

void radio_wakeup() {
  radio.startListening();
}

//void loop_radio() {
//    // First, stop listening so we can talk.
//  radio.stopListening();
//
//  // Take the time, and send it.  This will block until complete
//  unsigned long time = millis();
//  bool ok = radio.write(&time, sizeof(unsigned long));
//  
////  printf(ok? "ok...": "failed.\n\r");
//
//  // Now, continue listening
//  radio.startListening();
//
//  // Wait here until we get a response, or timeout (250ms)
//  unsigned long started_waiting_at = millis();
//  bool timeout = false;
//  while (!radio.available() && !timeout)
//    if (millis() - started_waiting_at > 200)
//      timeout = true;
//
//  // Describe the results
//  if (timeout)
//  {
////    printf("Timed out.\n\r");
//  }
//  else
//  {
//    // Grab the response, compare, and send to debugging spew
//    unsigned long got_time;
//    radio.read(&got_time, sizeof(unsigned long));
//
//    // Spew it
////    printf("Got response %lu, round-trip delay: %lu\n\r", got_time, millis() - got_time);
//  }
//
//  // Try again 1s later
//  delay(1000);
//}

bool radio_write(byte* bytes, byte length) {
//  radio.stopListening();
  bool ret = radio.write(bytes, length);
//  if (!ret) {
//    bool tx_ok = true;
//    bool tx_fail = true;
//    bool rx_ready = true;
//    radio.whatHappened(tx_ok, tx_fail, rx_ready);
//    if (tx_ok)
//      Serial.println("TX ok");
//    if (tx_fail)
//      Serial.println("TX fail");
//    if (rx_ready)
//      Serial.println("RX ready");
//
////    radio_close();
////    delay(1000);
////    radio_open();
//  }
//  radio.startListening();
//  delay(10);
//  radio.stopListening();
  return ret;
}


