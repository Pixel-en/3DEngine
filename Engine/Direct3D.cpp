#include <d3dcompiler.h>
#include "Direct3D.h"


//変数
namespace Direct3D
{
    ID3D11Device* pDevice;    //デバイス
    ID3D11DeviceContext* pContext;    //デバイスコンテキスト
    IDXGISwapChain* pSwapChain;    //スワップチェイン
    ID3D11RenderTargetView* pRenderTargetView;  //レンダーターゲットビュー

    ID3D11VertexShader* pVertexShader = nullptr;  //頂点シェーダー
    ID3D11PixelShader* pPixelShader = nullptr;    //ピクセルシェーダー
    ID3D11InputLayout* pVertexLayout = nullptr;  //頂点インプットレイアウト
    ID3D11RasterizerState* pRasterizerState = nullptr;  //ラスタライザー

    ID3D11Texture2D* pDepthStencil;			//深度ステンシル
    ID3D11DepthStencilView* pDepthStencilView;		//深度ステンシルビュー

    struct SHADER_BUNDLE
    {
        ID3D11VertexShader* pVertexShader_ = nullptr;  //頂点シェーダー
        ID3D11PixelShader* pPixelShader_ = nullptr;    //ピクセルシェーダー
        ID3D11InputLayout* pVertexLayout_ = nullptr;  //頂点インプットレイアウト
        ID3D11RasterizerState* pRasterizerState_ = nullptr;  //ラスタライザー
    };

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;
    SHADER_BUNDLE shaderBundle[SHADER_MAX];

    DirectX::XMINT2 WindowSize;

}



//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
    HRESULT hr;

    ///////////////////////////いろいろ準備するための設定///////////////////////////////
  //いろいろな設定項目をまとめた構造体
    DXGI_SWAP_CHAIN_DESC scDesc;

    //とりあえず全部0
    ZeroMemory(&scDesc, sizeof(scDesc));

    //描画先のフォーマット
    scDesc.BufferDesc.Width = winW;    //画面幅
    scDesc.BufferDesc.Height = winH;  //画面高さ
    scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 何色使えるか

    //FPS（1/60秒に1回）
    scDesc.BufferDesc.RefreshRate.Numerator = 60;
    scDesc.BufferDesc.RefreshRate.Denominator = 1;

    //その他
    scDesc.Windowed = TRUE;      //ウィンドウモードかフルスクリーンか
    scDesc.OutputWindow = hWnd;    //ウィンドウハンドル
    scDesc.BufferCount = 1;      //バックバッファの枚数
    scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  //バックバッファの使い道＝画面に描画するために
    scDesc.SampleDesc.Count = 1;    //MSAA（アンチエイリアス）の設定
    scDesc.SampleDesc.Quality = 0;    // 〃


    ////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
    D3D_FEATURE_LEVEL level;
    hr = D3D11CreateDeviceAndSwapChain(
        nullptr,        // どのビデオアダプタを使用するか？既定ならばnullptrで
        D3D_DRIVER_TYPE_HARDWARE,    // ドライバのタイプを渡す。ふつうはHARDWARE
        nullptr,        // 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
        0,          // 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
        nullptr,        // デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
        0,          // 上の引数でレベルを何個指定したか
        D3D11_SDK_VERSION,      // SDKのバージョン。必ずこの値
        &scDesc,        // 上でいろいろ設定した構造体
        &pSwapChain,        // 無事完成したSwapChainのアドレスが返ってくる
        &pDevice,        // 無事完成したDeviceアドレスが返ってくる
        &level,          // 無事完成したDevice、Contextのレベルが返ってくる
        &pContext);        // 無事完成したContextのアドレスが返ってくる
    if (FAILED(hr))
    {
        MessageBox(NULL, L"Device, Swapchain, DeviceContextの作成に失敗", NULL, MB_OK);
        return hr;
    }

    ///////////////////////////レンダーターゲットビュー作成///////////////////////////////
    //スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
    ID3D11Texture2D* pBackBuffer;
    pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    //レンダーターゲットビューを作成
    hr = pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRenderTargetView);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"レンダーターゲットビューの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //一時的にバックバッファを取得しただけなので解放
    pBackBuffer->Release();


    ///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
  //レンダリング結果を表示する範囲
    D3D11_VIEWPORT vp;
    vp.Width = (float)winW;  //幅
    vp.Height = (float)winH;//高さ
    vp.MinDepth = 0.0f;  //手前
    vp.MaxDepth = 1.0f;  //奥
    vp.TopLeftX = 0;  //左
    vp.TopLeftY = 0;  //上

    //深度ステンシルビューの作成
    D3D11_TEXTURE2D_DESC descDepth;
    descDepth.Width = winW;
    descDepth.Height = winH;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D32_FLOAT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);
    pDevice->CreateDepthStencilView(pDepthStencil, NULL, &pDepthStencilView);

    //データを画面に描画するための一通りの設定（パイプライン）
    pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
    pContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);            // 描画先を設定
    pContext->RSSetViewports(1, &vp);

    //シェーダー準備
    hr = InitShader();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"シェーダの初期化に失敗", NULL, MB_OK);
    }

    WindowSize = { winW,winH };

    return hr;

}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
    if (FAILED(InitShader3D()))
    {
        return E_FAIL;
    }

    if (FAILED(InitShader2D()))
    {
        return E_FAIL;
    }

    if (FAILED(InitShaderCRT()))
    {
        return E_FAIL;
    }
    return S_OK;
}

HRESULT Direct3D::InitShader3D()
{
    HRESULT hr;

    // 頂点シェーダの作成
    ID3DBlob* pCompileVS = nullptr;
    D3DCompileFromFile(L"Engine/simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL,
        &shaderBundle[SHADER_TYPE::SHADER_3D].pVertexShader_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点シェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //頂点インプットレイアウト
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
      { "NORMAL",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMVECTOR) * 2 ,  D3D11_INPUT_PER_VERTEX_DATA, 0 },//法線
    };
    hr = pDevice->CreateInputLayout(layout, 3, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(),
        &shaderBundle[SHADER_TYPE::SHADER_3D].pVertexLayout_);

    pCompileVS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点インプットレイアウトの作成に失敗", NULL, MB_OK);
        return hr;
    }

    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = nullptr;
    D3DCompileFromFile(L"Engine/simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL,
        &shaderBundle[SHADER_TYPE::SHADER_3D].pPixelShader_);
    pCompilePS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ピクセルシェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode = D3D11_CULL_BACK;  //多角形の裏側は描画しない（カリング）
    rdc.FillMode = D3D11_FILL_SOLID; //多角形の内部を塗りつぶす
    rdc.FrontCounterClockwise = FALSE; //反時計回りを表にするかどうか（がfalseなので時計回りが表）
    hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_3D].pRasterizerState_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ラスタライザステートの作成に失敗", NULL, MB_OK);
        return hr;
    }
    //3D用のシェーダー設定の皆さん ここまで
    return S_OK;
}

HRESULT Direct3D::InitShader2D()
{
      // 頂点シェーダの作成（コンパイル）
    HRESULT hr;
    ID3DBlob* pCompileVS = nullptr;
    D3DCompileFromFile(L"Engine/simple2D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_2D].pVertexShader_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点シェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //頂点インプットレイアウト
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
    };
    hr = pDevice->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_TYPE::SHADER_2D].pVertexLayout_);

    pCompileVS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点インプットレイアウトの作成に失敗", NULL, MB_OK);
        return hr;
    }

    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = nullptr;
    D3DCompileFromFile(L"Engine/simple2D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_2D].pPixelShader_);
    pCompilePS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ピクセルシェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode =D3D11_CULL_MODE::D3D11_CULL_NONE;  //多角形の裏側は描画しない（カリング）
    rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; //多角形の内部を塗りつぶす
    rdc.FrontCounterClockwise = FALSE; //反時計回りを表にするかどうか（がfalseなので時計回りが表）
    hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_2D].pRasterizerState_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ラスタライザステートの作成に失敗", NULL, MB_OK);
        return hr;
    }
    //２D用のシェーダー設定の皆さん ここまで

    return S_OK;
}

HRESULT Direct3D::InitShaderCRT()
{

      // 頂点シェーダの作成（コンパイル）
    HRESULT hr;
    ID3DBlob* pCompileVS = nullptr;
    D3DCompileFromFile(L"Engine/CRTRender.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_CRT].pVertexShader_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点シェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //頂点インプットレイアウト
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },  //位置
      { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMVECTOR) , D3D11_INPUT_PER_VERTEX_DATA, 0 },//UV座標
    };
    hr = pDevice->CreateInputLayout(layout, 2, pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &shaderBundle[SHADER_TYPE::SHADER_CRT].pVertexLayout_);

    pCompileVS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"頂点インプットレイアウトの作成に失敗", NULL, MB_OK);
        return hr;
    }

    // ピクセルシェーダの作成（コンパイル）
    ID3DBlob* pCompilePS = nullptr;
    D3DCompileFromFile(L"Engine/CRTRender.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
    assert(pCompileVS != nullptr);
    hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &shaderBundle[SHADER_TYPE::SHADER_CRT].pPixelShader_);
    pCompilePS->Release();
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ピクセルシェーダの作成に失敗", NULL, MB_OK);
        return hr;
    }

    //ラスタライザ作成
    D3D11_RASTERIZER_DESC rdc = {};
    rdc.CullMode =D3D11_CULL_MODE::D3D11_CULL_NONE;  //多角形の裏側は描画しない（カリング）
    rdc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID; //多角形の内部を塗りつぶす
    rdc.FrontCounterClockwise = FALSE; //反時計回りを表にするかどうか（がfalseなので時計回りが表）
    hr = pDevice->CreateRasterizerState(&rdc, &shaderBundle[SHADER_TYPE::SHADER_CRT].pRasterizerState_);
    if (FAILED(hr))
    {
        MessageBox(NULL, L"ラスタライザステートの作成に失敗", NULL, MB_OK);
        return hr;
    }
    //２D用のシェーダー設定の皆さん ここまで

    return S_OK;
}

void Direct3D::SetShader(SHADER_TYPE type)
{
    //それぞれをデバイスコンテキストにセット
    pContext->VSSetShader(shaderBundle[type].pVertexShader_, NULL, 0);  //頂点シェーダー
    pContext->PSSetShader(shaderBundle[type].pPixelShader_, NULL, 0);  //ピクセルシェーダー
    pContext->IASetInputLayout(shaderBundle[type].pVertexLayout_);  //頂点インプットレイアウト
    pContext->RSSetState(shaderBundle[type].pRasterizerState_);    //ラスタライザー
}

//描画開始
void Direct3D::BeginDraw()
{
    //背景の色
    float clearColor[4] = { 255 / 255.0f, 250 / 255.0f, 205 / 255.0f, 1.0f };//R,G,B,A

    //画面をクリア
    pContext->ClearRenderTargetView(pRenderTargetView, clearColor);

    //深度バッファクリア
    pContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

//描画終了
HRESULT Direct3D::EndDraw()
{
	if(FAILED(pSwapChain->Present(0, 0)))
		return E_FAIL;
}



//解放処理
void Direct3D::Release()
{
    //解放処理
    SAFE_RELEASE(pRasterizerState);
    SAFE_RELEASE(pVertexLayout);
    SAFE_RELEASE(pPixelShader);
    SAFE_RELEASE(pVertexShader);
    SAFE_RELEASE(pRenderTargetView);
    SAFE_RELEASE(pSwapChain);
    SAFE_RELEASE(pContext);
    SAFE_RELEASE(pDevice);
}