
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