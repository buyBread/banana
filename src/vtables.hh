#pragma once

namespace banana {

namespace vtables {

namespace IDirect3D9 {
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

} // IDirect3D9

namespace IDirect3DDevice9 {
    inline constexpr int QueryInterface              =   0;
    inline constexpr int AddRef                      =   1;
    inline constexpr int Release                     =   2;
    inline constexpr int TestCooperativeLevel        =   3;
    inline constexpr int GetAvailableTextureMem      =   4;
    inline constexpr int EvictManagedResources       =   5;
    inline constexpr int GetDirect3D                 =   6;
    inline constexpr int GetDeviceCaps               =   7;
    inline constexpr int GetDisplayMode              =   8;
    inline constexpr int GetCreationParameters       =   9;
    inline constexpr int SetCursorProperties         =  10;
    inline constexpr int SetCursorPosition           =  11;
    inline constexpr int ShowCursor                  =  12;
    inline constexpr int CreateAdditionalSwapChain   =  13;
    inline constexpr int GetSwapChain                =  14;
    inline constexpr int GetNumberOfSwapChains       =  15;
    inline constexpr int Reset                       =  16;
    inline constexpr int Present                     =  17;
    inline constexpr int GetBackBuffer               =  18;
    inline constexpr int GetRasterStatus             =  19;
    inline constexpr int SetDialogBoxMode            =  20;
    inline constexpr int SetGammaRamp                =  21;
    inline constexpr int GetGammaRamp                =  22;
    inline constexpr int CreateTexture               =  23;
    inline constexpr int CreateVolumeTexture         =  24;
    inline constexpr int CreateCubeTexture           =  25;
    inline constexpr int CreateVertexBuffer          =  26;
    inline constexpr int CreateIndexBuffer           =  27;
    inline constexpr int CreateRenderTarget          =  28;
    inline constexpr int CreateDepthStencilSurface   =  29;
    inline constexpr int UpdateSurface               =  30;
    inline constexpr int UpdateTexture               =  31;
    inline constexpr int GetRenderTargetData         =  32;
    inline constexpr int GetFrontBufferData          =  33;
    inline constexpr int StretchRect                 =  34;
    inline constexpr int ColorFill                   =  35;
    inline constexpr int CreateOffscreenPlainSurface =  36;
    inline constexpr int SetRenderTarget             =  37;
    inline constexpr int GetRenderTarget             =  38;
    inline constexpr int SetDepthStencilSurface      =  39;
    inline constexpr int GetDepthStencilSurface      =  40;
    inline constexpr int BeginScene                  =  41;
    inline constexpr int EndScene                    =  42;
    inline constexpr int Clear                       =  43;
    inline constexpr int SetTransform                =  44;
    inline constexpr int GetTransform                =  45;
    inline constexpr int MultiplyTransform           =  46;
    inline constexpr int SetViewport                 =  47;
    inline constexpr int GetViewport                 =  48;
    inline constexpr int SetMaterial                 =  49;
    inline constexpr int GetMaterial                 =  50;
    inline constexpr int SetLight                    =  51;
    inline constexpr int GetLight                    =  52;
    inline constexpr int LightEnable                 =  53;
    inline constexpr int GetLightEnable              =  54;
    inline constexpr int SetClipPlane                =  55;
    inline constexpr int GetClipPlane                =  56;
    inline constexpr int SetRenderState              =  57;
    inline constexpr int GetRenderState              =  58;
    inline constexpr int CreateStateBlock            =  59;
    inline constexpr int BeginStateBlock             =  60;
    inline constexpr int EndStateBlock               =  61;
    inline constexpr int SetClipStatus               =  62;
    inline constexpr int GetClipStatus               =  63;
    inline constexpr int GetTexture                  =  64;
    inline constexpr int SetTexture                  =  65;
    inline constexpr int GetTextureStageState        =  66;
    inline constexpr int SetTextureStageState        =  67;
    inline constexpr int GetSamplerState             =  68;
    inline constexpr int SetSamplerState             =  69;
    inline constexpr int ValidateDevice              =  70;
    inline constexpr int SetPaletteEntries           =  71;
    inline constexpr int GetPaletteEntries           =  72;
    inline constexpr int SetCurrentTexturePalette    =  73;
    inline constexpr int GetCurrentTexturePalette    =  74;
    inline constexpr int SetScissorRect              =  75;
    inline constexpr int GetScissorRect              =  76;
    inline constexpr int SetSoftwareVertexProcessing =  77;
    inline constexpr int GetSoftwareVertexProcessing =  78;
    inline constexpr int SetNPatchMode               =  79;
    inline constexpr int GetNPatchMode               =  80;
    inline constexpr int DrawPrimitive               =  81;
    inline constexpr int DrawIndexedPrimitive        =  82;
    inline constexpr int DrawPrimitiveUP             =  83;
    inline constexpr int DrawIndexedPrimitiveUP      =  84;
    inline constexpr int ProcessVertices             =  85;
    inline constexpr int CreateVertexDeclaration     =  86;
    inline constexpr int SetVertexDeclaration        =  87;
    inline constexpr int GetVertexDeclaration        =  88;
    inline constexpr int SetFVF                      =  89;
    inline constexpr int GetFVF                      =  90;
    inline constexpr int CreateVertexShader          =  91;
    inline constexpr int SetVertexShader             =  92;
    inline constexpr int GetVertexShader             =  93;
    inline constexpr int SetVertexShaderConstantF    =  94;
    inline constexpr int GetVertexShaderConstantF    =  95;
    inline constexpr int SetVertexShaderConstantI    =  96;
    inline constexpr int GetVertexShaderConstantI    =  97;
    inline constexpr int SetVertexShaderConstantB    =  98;
    inline constexpr int GetVertexShaderConstantB    =  99;
    inline constexpr int SetStreamSource             = 100;
    inline constexpr int GetStreamSource             = 101;
    inline constexpr int SetStreamSourceFreq         = 102;
    inline constexpr int GetStreamSourceFreq         = 103;
    inline constexpr int SetIndices                  = 104;
    inline constexpr int GetIndices                  = 105;
    inline constexpr int CreatePixelShader           = 106;
    inline constexpr int SetPixelShader              = 107;
    inline constexpr int GetPixelShader              = 108;
    inline constexpr int SetPixelShaderConstantF     = 109;
    inline constexpr int GetPixelShaderConstantF     = 110;
    inline constexpr int SetPixelShaderConstantI     = 111;
    inline constexpr int GetPixelShaderConstantI     = 112;
    inline constexpr int SetPixelShaderConstantB     = 113;
    inline constexpr int GetPixelShaderConstantB     = 114;
    inline constexpr int DrawRectPatch               = 115;
    inline constexpr int DrawTriPatch                = 116;
    inline constexpr int DeletePatch                 = 117;
    inline constexpr int CreateQuery                 = 118;
} // IDirect3DDevice9

namespace IDirectInputDeviceA {
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
} // IDirectInputDeviceA

} // vtables

} // banana