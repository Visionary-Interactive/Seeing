#pragma once

#if defined(_WIN32) || defined(_WIN64)
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#define _WINSOCKAPI_

#include <winsock2.h>
#include <ws2tcpip.h>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static void sleep_ms(int ms) { Sleep(ms); }
#else
#include <unistd.h>
static void sleep_ms(int ms) { usleep(ms * 1000); }
#endif

// Ensure the ws2_32 lib is linked
#ifdef _MSC_VER
#pragma comment(lib, "Ws2_32.lib")
#endif
#endif // _WIN32

#define NBNET_IMPL

#include "nbnet.h"
#include "net_drivers/udp.h"

#include <stdio.h>