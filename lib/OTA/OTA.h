#pragma once

#include <stddef.h>

class OTA {
public:
    void onStart();
    void onProgress(size_t current, size_t final);
    void onEnd(bool success);
};

