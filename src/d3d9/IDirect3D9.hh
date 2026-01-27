#pragma once

#include "IDirect3DDevice9.hh"
#include "../banana.hh"

class wrapped_IDirect3D9 : public IDirect3D9 {
private:
    IDirect3D9* m_d3d9;

    std::atomic<ULONG> m_reference_count;

public:
    wrapped_IDirect3D9(IDirect3D9* d3d9) : m_d3d9(d3d9),
                                           m_reference_count(1) {}

    virtual ~wrapped_IDirect3D9() = default; 

    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) noexcept override {
        if (riid == IID_IDirect3DDevice9) {
            *ppvObj = this; AddRef();

            return S_OK;
        }

        return m_d3d9->QueryInterface(riid, ppvObj);
    }
    ULONG __stdcall AddRef() noexcept override {
        m_d3d9->AddRef();

        return ++m_reference_count;
    }
    ULONG __stdcall Release() noexcept override {
        auto count = m_d3d9->Release();

        if (--m_reference_count == 0) {
            delete this;

            return 0;
        }

        return count;
    }

    HRESULT __stdcall CreateDevice(UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DDevice9** ppReturnedDeviceInterface) noexcept override {
        IDirect3DDevice9* device = nullptr;
        
        HRESULT result = m_d3d9->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, &device);
        
        if (SUCCEEDED(result) && device != nullptr) {
            *ppReturnedDeviceInterface = new wrapped_IDirect3DDevice9(device);
            
            LOG.dbg("wrapped IDirect3DDevice9");
        } else
            *ppReturnedDeviceInterface = nullptr;
        
        return result;
    }

    HRESULT __stdcall RegisterSoftwareDevice(void* pInitializeFunction) noexcept override {
        return m_d3d9->RegisterSoftwareDevice(pInitializeFunction);
    }
    UINT __stdcall GetAdapterCount() noexcept override {
        return m_d3d9->GetAdapterCount();
    }
    HRESULT __stdcall GetAdapterIdentifier(UINT Adapter, DWORD Flags, D3DADAPTER_IDENTIFIER9* pIdentifier) noexcept override {
        return m_d3d9->GetAdapterIdentifier(Adapter, Flags, pIdentifier);
    }
    UINT __stdcall GetAdapterModeCount(UINT Adapter, D3DFORMAT Format) noexcept override {
        return m_d3d9->GetAdapterModeCount(Adapter, Format);
    }
    HRESULT __stdcall EnumAdapterModes(UINT Adapter, D3DFORMAT Format, UINT Mode, D3DDISPLAYMODE* pMode) noexcept override {
        return m_d3d9->EnumAdapterModes(Adapter, Format, Mode, pMode);
    }
    HRESULT __stdcall GetAdapterDisplayMode(UINT Adapter, D3DDISPLAYMODE* pMode) noexcept override {
        return m_d3d9->GetAdapterDisplayMode(Adapter, pMode);
    }
    HRESULT __stdcall CheckDeviceType(UINT Adapter, D3DDEVTYPE DevType, D3DFORMAT AdapterFormat, D3DFORMAT BackBufferFormat, BOOL bWindowed) noexcept override {
        return m_d3d9->CheckDeviceType(Adapter, DevType, AdapterFormat, BackBufferFormat, bWindowed);
    }
    HRESULT __stdcall CheckDeviceFormat(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType, D3DFORMAT CheckFormat) noexcept override {
        return m_d3d9->CheckDeviceFormat(Adapter, DeviceType, AdapterFormat, Usage, RType, CheckFormat);
    }
    HRESULT __stdcall CheckDeviceMultiSampleType(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SurfaceFormat, BOOL Windowed, D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels) noexcept override {
        return m_d3d9->CheckDeviceMultiSampleType(Adapter, DeviceType, SurfaceFormat, Windowed, MultiSampleType, pQualityLevels);
    }
    HRESULT __stdcall CheckDepthStencilMatch(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat, D3DFORMAT DepthStencilFormat) noexcept override {
        return m_d3d9->CheckDepthStencilMatch(Adapter, DeviceType, AdapterFormat, RenderTargetFormat, DepthStencilFormat);
    }
    HRESULT __stdcall CheckDeviceFormatConversion(UINT Adapter, D3DDEVTYPE DeviceType, D3DFORMAT SourceFormat, D3DFORMAT TargetFormat) noexcept override {
        return m_d3d9->CheckDeviceFormatConversion(Adapter, DeviceType, SourceFormat, TargetFormat);
    }
    HRESULT __stdcall GetDeviceCaps(UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps) noexcept override {
        return m_d3d9->GetDeviceCaps(Adapter, DeviceType, pCaps);
    }
    HMONITOR __stdcall GetAdapterMonitor(UINT Adapter) noexcept override {
        return m_d3d9->GetAdapterMonitor(Adapter);
    }
};