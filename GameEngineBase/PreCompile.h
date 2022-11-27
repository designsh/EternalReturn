#pragma once

// 이안에 들어있고 중복이 되는 헤더는 다시 빌드하지 않는다.

// #include <Windows.h>
#include <WS2tcpip.h> // inet_pton 함수를 사용하기 위한 헤더
#include <WinSock2.h>
#include <stdint.h>
#include <sstream>
#include <Windows.h>

#pragma comment (lib, "ws2_32")


// data struct
#include <vector>
#include <list>
#include <map>

// std::
#include <string>
#include <functional>

#include <io.h>
#include <assert.h>
#include <crtdbg.h>

// Math
#include <math.h>
#include <random>
#include <directxmath.h>

// 마소에서 제공하는 simd 연산용 함수들을 사용할수 있다.
#include <DirectXPackedVector.h>

// io
#include <filesystem>

// Thread
#include <mutex>
