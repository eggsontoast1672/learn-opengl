#pragma once

#include <csignal>                      // std::raise
#include <string>                       // std::string

#define SNBL_ASSERT(x) \
    if (!(x)) \
        std::raise(SIGTRAP)

#define SNBL_GLCALL(function) \
    snbl::clearError(); function; \
    SNBL_ASSERT(snbl::logCall(__FILE__, __LINE__, #function))

namespace snbl {

void clearError();
bool logCall(const char *, int, const char *);

} // namespace snbl