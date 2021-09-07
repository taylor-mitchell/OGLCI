#pragma once
#define NOMINMAX
#include <stdlib.h>
#include <windows.h>

size_t cacheLineSize(int cacheNum);
size_t cacheSize(int cacheNum);