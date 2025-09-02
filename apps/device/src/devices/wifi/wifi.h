#ifndef WIFI_H
#define WIFI_H

#include "_config.h"
#include "_shared.h"

void wifiConnect();
void wifiReconnect();
void wifiDisconnect();
void wifiPowerOff();
void wifiWaitForConnection();
bool wifiIsConnected();
String wifiGetIP();
String wifiGetSSID();
int8_t wifiGetSignal();

#endif