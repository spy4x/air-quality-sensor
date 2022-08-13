#ifndef WIFI_H
#define WIFI_H

#include "_config.h"
#include "_shared.h"

void wifiConnect(bool shouldWaitForConnect = false);
void wifiDisconnect();
bool wifiIsConnected();
String wifiGetIP();
String wifiGetSSID();
int8_t wifiGetSignal();

#endif