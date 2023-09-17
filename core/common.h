#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <ctype.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>
#include <functional>
#include <memory>
#include <map>

#include "../utilities/header.h"
#include "apu.h"
#include "cpu.h"
#include "memory.h"
#include "ppu.h"

typedef enum {FALSE = 0, TRUE = 1} boolean;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;

