#pragma once

#include <iostream>
#include <safelog.hpp>

/** The singleton log instance to use for info log level.
  * Use it *only* though the LOGI macro to prevent any dead lock. 
  */
extern Log logInfo;

/** The log interface, the logInfo instance should *not* be called directly. */
#define LOGI(...) \
    do{logInfo , logInfo.begin , ##__VA_ARGS__ , logInfo.end;} while(false)

