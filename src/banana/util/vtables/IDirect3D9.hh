#pragma once

namespace util { namespace vtables { namespace IDirect3D9 {
    inline constexpr int QueryInterface              =  0;
    inline constexpr int AddRef                      =  1;
    inline constexpr int Release                     =  2;
    inline constexpr int RegisterSoftwareDevice      =  3;
    inline constexpr int GetAdapterCount             =  4;
    inline constexpr int GetAdapterIdentifier        =  5;
    inline constexpr int GetAdapterModeCount         =  6;
    inline constexpr int EnumAdapterModes            =  7;
    inline constexpr int GetAdapterDisplayMode       =  8;
    inline constexpr int CheckDeviceType             =  9;
    inline constexpr int CheckDeviceFormat           = 10;
    inline constexpr int CheckDeviceMultiSampleType  = 11;
    inline constexpr int CheckDepthStencilMatch      = 12;
    inline constexpr int CheckDeviceFormatConversion = 13;
    inline constexpr int GetDeviceCaps               = 14;
    inline constexpr int GetAdapterMonitor           = 15;
    inline constexpr int CreateDevice                = 16;
}}} // util::vtables::IDirect3D9