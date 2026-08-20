#pragma once

namespace util { namespace vtables { namespace IDirectInputDeviceA {
    inline constexpr int QueryInterface       =  0;
    inline constexpr int AddRef               =  1;
    inline constexpr int Release              =  2;
    inline constexpr int GetCapabilities      =  3;
    inline constexpr int EnumObjects          =  4;
    inline constexpr int GetProperty          =  5;
    inline constexpr int SetProperty          =  6;
    inline constexpr int Acquire              =  7;
    inline constexpr int Unacquire            =  8;
    inline constexpr int GetDeviceState       =  9;
    inline constexpr int GetDeviceData        = 10;
    inline constexpr int SetDataFormat        = 11;
    inline constexpr int SetEventNotification = 12;
    inline constexpr int SetCooperativeLevel  = 13; 
    inline constexpr int GetObjectInfo        = 14;
    inline constexpr int GetDeviceInfo        = 15;
    inline constexpr int RunControlPanel      = 16;
    inline constexpr int Initialize           = 17;
}}} // util::vtables::IDirectInputDeviceA