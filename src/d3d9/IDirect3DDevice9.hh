#pragma once

#include <atomic>
#include <d3d9.h>

class wrapped_IDirect3DDevice9 : public IDirect3DDevice9 {

private:
    IDirect3DDevice9* m_device;

    std::atomic<ULONG> m_reference_count;

public:
    wrapped_IDirect3DDevice9(IDirect3DDevice9* device) : m_device(device), m_reference_count(1) {}

    virtual ~wrapped_IDirect3DDevice9() = default;

    HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj) noexcept override {
        if (riid == IID_IDirect3DDevice9) {
            *ppvObj = this; AddRef();

            return S_OK;
        }

        return m_device->QueryInterface(riid, ppvObj);
    }
    ULONG __stdcall AddRef() noexcept override {
        m_device->AddRef();

        return ++m_reference_count;
    }
    ULONG __stdcall Release() noexcept override {
        auto count = m_device->Release();

        if (--m_reference_count == 0) {
            delete this;

            return 0;
        }

        return count;
    }
    HRESULT __stdcall TestCooperativeLevel() noexcept override {
        return m_device->TestCooperativeLevel();
    }
    UINT __stdcall GetAvailableTextureMem() noexcept override {
        return m_device->GetAvailableTextureMem();
    }
    HRESULT __stdcall EvictManagedResources() noexcept override {
        return m_device->EvictManagedResources();
    }
    HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9) noexcept override {
        return m_device->GetDirect3D(ppD3D9);
    }
    HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps) noexcept override {
        return m_device->GetDeviceCaps(pCaps);
    }
    HRESULT __stdcall GetDisplayMode(UINT iSwapChain, D3DDISPLAYMODE* pMode) noexcept override {
        return m_device->GetDisplayMode(iSwapChain, pMode);
    }
    HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) noexcept override {
        return m_device->GetCreationParameters(pParameters);
    }
    HRESULT __stdcall SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9* pCursorBitmap) noexcept override {
        return m_device->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);
    }
    void __stdcall SetCursorPosition(int X, int Y, DWORD Flags) noexcept override {
        m_device->SetCursorPosition(X, Y, Flags);
    }
    BOOL __stdcall ShowCursor(BOOL bShow) noexcept override {
        return m_device->ShowCursor(bShow);
    }
    HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) noexcept override {
        return m_device->CreateAdditionalSwapChain(pPresentationParameters, pSwapChain);
    }
    HRESULT __stdcall GetSwapChain(UINT iSwapChain, IDirect3DSwapChain9** pSwapChain) noexcept override {
        return m_device->GetSwapChain(iSwapChain, pSwapChain);
    }
    UINT __stdcall GetNumberOfSwapChains() noexcept override {
        return m_device->GetNumberOfSwapChains();
    }
    HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters) noexcept override {
        return m_device->Reset(pPresentationParameters);
    }
    HRESULT __stdcall Present(CONST RECT* pSourceRect, CONST RECT* pDestRect, HWND hDestWindowOverride, CONST RGNDATA* pDirtyRegion) noexcept override {
        return m_device->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
    }
    HRESULT __stdcall GetBackBuffer(UINT iSwapChain, UINT iBackBuffer, D3DBACKBUFFER_TYPE Type, IDirect3DSurface9** ppBackBuffer) noexcept override {
        return m_device->GetBackBuffer(iSwapChain, iBackBuffer, Type, ppBackBuffer);
    }
    HRESULT __stdcall GetRasterStatus(UINT iSwapChain, D3DRASTER_STATUS* pRasterStatus) noexcept override {
        return m_device->GetRasterStatus(iSwapChain, pRasterStatus);
    }
    HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs) noexcept override {
        return m_device->SetDialogBoxMode(bEnableDialogs);
    }
    void __stdcall SetGammaRamp(UINT iSwapChain, DWORD Flags, CONST D3DGAMMARAMP* pRamp) noexcept override {
        m_device->SetGammaRamp(iSwapChain, Flags, pRamp);
    }
    void __stdcall GetGammaRamp(UINT iSwapChain, D3DGAMMARAMP* pRamp) noexcept override {
        m_device->GetGammaRamp(iSwapChain, pRamp);
    }
    HRESULT __stdcall CreateTexture(UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
    }
    HRESULT __stdcall CreateVolumeTexture(UINT Width, UINT Height, UINT Depth, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DVolumeTexture9** ppVolumeTexture, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture, pSharedHandle);
    }
    HRESULT __stdcall CreateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture, pSharedHandle);
    }
    HRESULT __stdcall CreateVertexBuffer(UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
    }
    HRESULT __stdcall CreateIndexBuffer(UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
    }
    HRESULT __stdcall CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Lockable, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateRenderTarget(Width, Height, Format, MultiSample, MultisampleQuality, Lockable, ppSurface, pSharedHandle);
    }
    HRESULT __stdcall CreateDepthStencilSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality, BOOL Discard, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality, Discard, ppSurface, pSharedHandle);
    }
    HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, CONST POINT* pDestPoint) noexcept override {
        return m_device->UpdateSurface(pSourceSurface, pSourceRect, pDestinationSurface, pDestPoint);
    }
    HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) noexcept override {
        return m_device->UpdateTexture(pSourceTexture, pDestinationTexture);
    }
    HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) noexcept override {
        return m_device->GetRenderTargetData(pRenderTarget, pDestSurface);
    }
    HRESULT __stdcall GetFrontBufferData(UINT iSwapChain, IDirect3DSurface9* pDestSurface) noexcept override {
        return m_device->GetFrontBufferData(iSwapChain, pDestSurface);
    }
    HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, CONST RECT* pSourceRect, IDirect3DSurface9* pDestSurface, CONST RECT* pDestRect, D3DTEXTUREFILTERTYPE Filter) noexcept override {
        return m_device->StretchRect(pSourceSurface, pSourceRect, pDestSurface, pDestRect, Filter);
    }
    HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface, CONST RECT* pRect, D3DCOLOR color) noexcept override {
        return m_device->ColorFill(pSurface, pRect, color);
    }
    HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width, UINT Height, D3DFORMAT Format, D3DPOOL Pool, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle) noexcept override {
        return m_device->CreateOffscreenPlainSurface(Width, Height, Format, Pool, ppSurface, pSharedHandle);
    }
    HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) noexcept override {
        return m_device->SetRenderTarget(RenderTargetIndex, pRenderTarget);
    }
    HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) noexcept override {
        return m_device->GetRenderTarget(RenderTargetIndex, ppRenderTarget);
    }
    HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) noexcept override {
        return m_device->SetDepthStencilSurface(pNewZStencil);
    }
    HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) noexcept override {
        return m_device->GetDepthStencilSurface(ppZStencilSurface);
    }
    HRESULT __stdcall BeginScene() noexcept override {
        return m_device->BeginScene();
    }
    HRESULT __stdcall EndScene() noexcept override {
        return m_device->EndScene();
    }
    HRESULT __stdcall Clear(DWORD Count, CONST D3DRECT* pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) noexcept override {
        return m_device->Clear(Count, pRects, Flags, Color, Z, Stencil);
    }
    HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) noexcept override {
        return m_device->SetTransform(State, pMatrix);
    }
    HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX* pMatrix) noexcept override {
        return m_device->GetTransform(State, pMatrix);
    }
    HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX* pMatrix) noexcept override {
        return m_device->MultiplyTransform(State, pMatrix);
    }
    HRESULT __stdcall SetViewport(CONST D3DVIEWPORT9* pViewport) noexcept override {
        return m_device->SetViewport(pViewport);
    }
    HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport) noexcept override {
        return m_device->GetViewport(pViewport);
    }
    HRESULT __stdcall SetMaterial(CONST D3DMATERIAL9* pMaterial) noexcept override {
        return m_device->SetMaterial(pMaterial);
    }
    HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial) noexcept override {
        return m_device->GetMaterial(pMaterial);
    }
    HRESULT __stdcall SetLight(DWORD Index, CONST D3DLIGHT9* pLight) noexcept override {
        return m_device->SetLight(Index, pLight);
    }
    HRESULT __stdcall GetLight(DWORD Index, D3DLIGHT9* pLight) noexcept override {
        return m_device->GetLight(Index, pLight);
    }
    HRESULT __stdcall LightEnable(DWORD Index, BOOL Enable) noexcept override {
        return m_device->LightEnable(Index, Enable);
    }
    HRESULT __stdcall GetLightEnable(DWORD Index, BOOL* pEnable) noexcept override {
        return m_device->GetLightEnable(Index, pEnable);
    }
    HRESULT __stdcall SetClipPlane(DWORD Index, CONST float* pPlane) noexcept override {
        return m_device->SetClipPlane(Index, pPlane);
    }
    HRESULT __stdcall GetClipPlane(DWORD Index, float* pPlane) noexcept override {
        return m_device->GetClipPlane(Index, pPlane);
    }
    HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) noexcept override {
        return m_device->SetRenderState(State, Value);
    }
    HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State, DWORD* pValue) noexcept override {
        return m_device->GetRenderState(State, pValue);
    }
    HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type, IDirect3DStateBlock9** ppSB) noexcept override {
        return m_device->CreateStateBlock(Type, ppSB);
    }
    HRESULT __stdcall BeginStateBlock() noexcept override {
        return m_device->BeginStateBlock();
    }
    HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB) noexcept override {
        return m_device->EndStateBlock(ppSB);
    }
    HRESULT __stdcall SetClipStatus(CONST D3DCLIPSTATUS9* pClipStatus) noexcept override {
        return m_device->SetClipStatus(pClipStatus);
    }
    HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus) noexcept override {
        return m_device->GetClipStatus(pClipStatus);
    }
    HRESULT __stdcall GetTexture(DWORD Stage, IDirect3DBaseTexture9** ppTexture) noexcept override {
        return m_device->GetTexture(Stage, ppTexture);
    }
    HRESULT __stdcall SetTexture(DWORD Stage, IDirect3DBaseTexture9* pTexture) noexcept override {
        return m_device->SetTexture(Stage, pTexture);
    }
    HRESULT __stdcall GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD* pValue) noexcept override {
        return m_device->GetTextureStageState(Stage, Type, pValue);
    }
    HRESULT __stdcall SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) noexcept override {
        return m_device->SetTextureStageState(Stage, Type, Value);
    }
    HRESULT __stdcall GetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD* pValue) noexcept override {
        return m_device->GetSamplerState(Sampler, Type, pValue);
    }
    HRESULT __stdcall SetSamplerState(DWORD Sampler, D3DSAMPLERSTATETYPE Type, DWORD Value) noexcept override {
        return m_device->SetSamplerState(Sampler, Type, Value);
    }
    HRESULT __stdcall ValidateDevice(DWORD* pNumPasses) noexcept override {
        return m_device->ValidateDevice(pNumPasses);
    }
    HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY* pEntries) noexcept override {
        return m_device->SetPaletteEntries(PaletteNumber, pEntries);
    }
    HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY* pEntries) noexcept override {
        return m_device->GetPaletteEntries(PaletteNumber, pEntries);
    }
    HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber) noexcept override {
        return m_device->SetCurrentTexturePalette(PaletteNumber);
    }
    HRESULT __stdcall GetCurrentTexturePalette(UINT *PaletteNumber) noexcept override {
        return m_device->GetCurrentTexturePalette(PaletteNumber);
    }
    HRESULT __stdcall SetScissorRect(CONST RECT* pRect) noexcept override {
        return m_device->SetScissorRect(pRect);
    }
    HRESULT __stdcall GetScissorRect(RECT* pRect) noexcept override {
        return m_device->GetScissorRect(pRect);
    }
    HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware) noexcept override {
        return m_device->SetSoftwareVertexProcessing(bSoftware);
    }
    BOOL __stdcall GetSoftwareVertexProcessing() noexcept override {
        return m_device->GetSoftwareVertexProcessing();
    }
    HRESULT __stdcall SetNPatchMode(float nSegments) noexcept override {
        return m_device->SetNPatchMode(nSegments);
    }
    float __stdcall GetNPatchMode() noexcept override {
        return m_device->GetNPatchMode();
    }
    HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) noexcept override {
        return m_device->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
    }
    HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount) noexcept override {
        return m_device->DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
    }
    HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) noexcept override {
        return m_device->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
    }
    HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinVertexIndex, UINT NumVertices, UINT PrimitiveCount, CONST void* pIndexData, D3DFORMAT IndexDataFormat, CONST void* pVertexStreamZeroData, UINT VertexStreamZeroStride) noexcept override {
        return m_device->DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
    }
    HRESULT __stdcall ProcessVertices(UINT SrcStartIndex, UINT DestIndex, UINT VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, DWORD Flags) noexcept override {
        return m_device->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer, pVertexDecl, Flags);
    }
    HRESULT __stdcall CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) noexcept override {
        return m_device->CreateVertexDeclaration(pVertexElements, ppDecl);
    }
    HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) noexcept override {
        return m_device->SetVertexDeclaration(pDecl);
    }
    HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) noexcept override {
        return m_device->GetVertexDeclaration(ppDecl);
    }
    HRESULT __stdcall SetFVF(DWORD FVF) noexcept override {
        return m_device->SetFVF(FVF);
    }
    HRESULT __stdcall GetFVF(DWORD* pFVF) noexcept override {
        return m_device->GetFVF(pFVF);
    }
    HRESULT __stdcall CreateVertexShader(CONST DWORD* pFunction, IDirect3DVertexShader9** ppShader) noexcept override {
        return m_device->CreateVertexShader(pFunction, ppShader);
    }
    HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader) noexcept override {
        return m_device->SetVertexShader(pShader);
    }
    HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader) noexcept override {
        return m_device->GetVertexShader(ppShader);
    }
    HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) noexcept override {
        return m_device->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }
    HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) noexcept override {
        return m_device->GetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }
    HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) noexcept override {
        return m_device->SetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }
    HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) noexcept override {
        return m_device->GetVertexShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }
    HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) noexcept override {
        return m_device->SetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }
    HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) noexcept override {
        return m_device->GetVertexShaderConstantB(StartRegister, pConstantData, BoolCount);
    }
    HRESULT __stdcall SetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9* pStreamData, UINT OffsetInBytes, UINT Stride) noexcept override {
        return m_device->SetStreamSource(StreamNumber, pStreamData, OffsetInBytes, Stride);
    }
    HRESULT __stdcall GetStreamSource(UINT StreamNumber, IDirect3DVertexBuffer9** ppStreamData, UINT* pOffsetInBytes, UINT* pStride) noexcept override {
        return m_device->GetStreamSource(StreamNumber, ppStreamData, pOffsetInBytes, pStride);
    }
    HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber, UINT Setting) noexcept override {
        return m_device->SetStreamSourceFreq(StreamNumber, Setting);
    }
    HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber, UINT* pSetting) noexcept override {
        return m_device->GetStreamSourceFreq(StreamNumber, pSetting);
    }
    HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData) noexcept override {
        return m_device->SetIndices(pIndexData);
    }
    HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData) noexcept override {
        return m_device->GetIndices(ppIndexData);
    }
    HRESULT __stdcall CreatePixelShader(CONST DWORD* pFunction, IDirect3DPixelShader9** ppShader) noexcept override {
        return m_device->CreatePixelShader(pFunction, ppShader);
    }
    HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader) noexcept override {
        return m_device->SetPixelShader(pShader);
    }
    HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader) noexcept override {
        return m_device->GetPixelShader(ppShader);
    }
    HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister, CONST float* pConstantData, UINT Vector4fCount) noexcept override {
        return m_device->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }
    HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount) noexcept override {
        return m_device->GetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);
    }
    HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister, CONST int* pConstantData, UINT Vector4iCount) noexcept override {
        return m_device->SetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }
    HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister, int* pConstantData, UINT Vector4iCount) noexcept override {
        return m_device->GetPixelShaderConstantI(StartRegister, pConstantData, Vector4iCount);
    }
    HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT BoolCount) noexcept override {
        return m_device->SetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }
    HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister, BOOL* pConstantData, UINT BoolCount) noexcept override {
        return m_device->GetPixelShaderConstantB(StartRegister, pConstantData, BoolCount);
    }
    HRESULT __stdcall DrawRectPatch(UINT Handle, CONST float* pNumSegs, CONST D3DRECTPATCH_INFO* pRectPatchInfo) noexcept override {
        return m_device->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
    }
    HRESULT __stdcall DrawTriPatch(UINT Handle, CONST float* pNumSegs, CONST D3DTRIPATCH_INFO* pTriPatchInfo) noexcept override {
        return m_device->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
    }
    HRESULT __stdcall DeletePatch(UINT Handle) noexcept override {
        return m_device->DeletePatch(Handle);
    }
    HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery) noexcept override {
        return m_device->CreateQuery(Type, ppQuery);
    }
};

