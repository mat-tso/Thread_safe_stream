#pragma once

#include <iostream>
#include <safelog.hpp>

/** The singleton log instance to use for info log level. */
extern Log logInfo;

/** The log interface, the logInfo instance should no be called directly. */
#define LOGI(...) \
    logInfo , logInfo.begin , ##__VA_ARGS__ , logInfo.end

