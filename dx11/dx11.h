
// #include <dxgidebug.h>
// dxguid.lib

#ifndef NDEBUG
  // D3D11 Debug Info Queue
  ID3D11InfoQueue* info = null;
  HRESULT hr = device->QueryInterface(__uuidof(ID3D11InfoQueue), (void**)&info);

  if (SUCCEEDED(hr))
  {
    info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
    info->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, TRUE);
    info->Release();
  }
#endif


static void ActivateD3D11DebugInfo(ID3D11Device *Device)
{
    ID3D11InfoQueue *Info;
    if(SUCCEEDED(IProvideClassInfo_QueryInterface(Device, &IID_ID3D11InfoQueue, (void**)&Info)))
    {
        ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_CORRUPTION, TRUE);
        ID3D11InfoQueue_SetBreakOnSeverity(Info, D3D11_MESSAGE_SEVERITY_ERROR, TRUE);

        ID3D11InfoQueue_Release(Info);
    }
}