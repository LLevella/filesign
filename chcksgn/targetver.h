#pragma once

// Включение SDKDDKVer.h обеспечивает определение самой последней доступной платформы Windows.

// Если требуется выполнить сборку приложения для предыдущей версии Windows, включите WinSDKVer.h и
// задайте для макроса _WIN32_WINNT значение поддерживаемой платформы перед включением SDKDDKVer.h.
#include <WinSDKVer.h>
#ifndef _WIN32_WINNT          
#define _WIN32_WINNT 0x0500       // Указывает, что минимально необходимой версией платформы является Windows XP.
#endif
#include <SDKDDKVer.h>
