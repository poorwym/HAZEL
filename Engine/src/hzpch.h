#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>


#ifdef HAZEL_PLATFORM_WINDOWS
    #include <Windows.h>
#endif

#ifdef HAZEL_PLATFORM_LINUX
    #include <X11/Xlib.h>
#endif