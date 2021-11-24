//#include "ETH_Helper.h"

#include <Arduino.h>
#include <MIDI.h>
#include <AppleMIDI.h>
#include <Ethernet.h>
//#include <Ethernet3.h>
#include <EthernetBonjour.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned long t1 = millis();
int8_t isConnected = 0;

APPLEMIDI_CREATE_INSTANCE(EthernetUDP, MIDI_RTP, "AppleMIDI-Arduino", DEFAULT_CONTROL_PORT);


void setup() {
  //ETH_startup();

   if (Ethernet.begin(mac) == 0)
    for (;;);

  MIDI_RTP.begin(MIDI_CHANNEL_OMNI);

  AppleMIDI_RTP.setHandleConnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc, const char* name) {
    isConnected++;

  });
  AppleMIDI_RTP.setHandleDisconnected([](const APPLEMIDI_NAMESPACE::ssrc_t & ssrc) {
    isConnected--;

  });


  EthernetBonjour.begin("arduino");

  EthernetBonjour.addServiceRecord("Arduino._apple-midi",
                                   AppleMIDI_RTP.getPort(),
                                   MDNSServiceUDP);

  MIDI_RTP.begin();
}

void loop() {
  MIDI_RTP.read();

  EthernetBonjour.run();

 /*#ifndef ETHERNET3
    EthernetBonjour.run();
  #endif */
}