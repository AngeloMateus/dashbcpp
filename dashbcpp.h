#ifndef DASHBCPP_H_INCLUDED
#define DASHBCPP_H_INCLUDED

#include "external/cpu.cpp"
#include "external/smc.h"
#include <IOKit/IOKitLib.h>
#include <array>
#include <ctime>
#include <curses.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/statvfs.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>

typedef struct {
  bool running;
} dashbcpp_t;

dashbcpp_t dashbcpp;

#endif
