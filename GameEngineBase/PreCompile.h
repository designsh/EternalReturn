#pragma once

// �̾ȿ� ����ְ� �ߺ��� �Ǵ� ����� �ٽ� �������� �ʴ´�.

// #include <Windows.h>
#include <WS2tcpip.h> // inet_pton �Լ��� ����ϱ� ���� ���
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

// ���ҿ��� �����ϴ� simd ����� �Լ����� ����Ҽ� �ִ�.
#include <DirectXPackedVector.h>

// io
#include <filesystem>

// Thread
#include <mutex>
