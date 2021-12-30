// include the basic windows header files and the Direct3D header files
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Cube.h"
#include "Mesh.h"
#include "MeshClass.h"
#include "diamondCube.h"
#include "OakTree.h"
#include "Terrain.h"
#include "Quad.h"
#include "snow.h"
#include "SnowMan.h"
#include "SnowManBody.h"
#include "SnowManHat.h"

// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// define the screen resolution
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// global declarations
IDXGISwapChain* swapchain;             // the pointer to the swap chain interface

ID3D11RenderTargetView* backbuffer;    // the pointer to our back buffer
ID3D11DepthStencilView* zbuffer;       // the pointer to our depth buffer
ID3D11InputLayout* pLayout;            // the pointer to the input layout
ID3D11VertexShader* pVS;               // the pointer to the vertex shader
ID3D11PixelShader* pPS;                // the pointer to the pixel shader
//ID3D11Buffer* pVBuffer;                // the pointer to the vertex buffer
//ID3D11Buffer* pIBuffer;                // the pointer to the index buffer

//cameraposition
D3DXVECTOR3 cameraPosition;
D3DXVECTOR3 lookAtPosition;

// state objects
ID3D11RasterizerState* pRS;            // the default rasterizer state
ID3D11SamplerState* pSS;               // the default sampler state
ID3D11BlendState* pBS;                 // a typicl blend state

Cube* cube;
//Cube* test;
diamondCube* test;
MeshClass* meshClass;
Quad* skyBox;

OakTree* tree;
Terrain* terrain;

float rndidxX ;
float rndidxZ;
// function prototypes
void InitD3D(HWND hWnd);    // sets up and initializes Direct3D
void RenderFrame(void);     // renders a single frame
void CleanD3D(void);        // closes Direct3D and releases memory
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void InitStates(void);      // initializes the states

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    srand(time(NULL));//랜덤 시드 초기화.

    HWND hWnd;
    WNDCLASSEX wc;

    ZeroMemory(&wc, sizeof(WNDCLASSEX));

    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WindowClass";

    RegisterClassEx(&wc);

    RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd = CreateWindowEx(NULL,
        L"WindowClass",
        L"눈쌓이는 마을: 아래키를 누르면 눈이 쌓입니다. 어쩌다보면 스노우맨도 나와요!",
        WS_OVERLAPPEDWINDOW,
        300,
        300,
        wr.right - wr.left,
        wr.bottom - wr.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    ShowWindow(hWnd, nCmdShow);

    // set up and initialize Direct3D
    InitD3D(hWnd);

    // enter the main loop:

    MSG msg;

    while (TRUE)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            if (msg.message == WM_QUIT)
                break;
        }

        RenderFrame();
    }

    // clean up DirectX and COM
    CleanD3D();

    return msg.wParam;
}

void ProcessKeyInput(HWND hWnd, WPARAM keyPress)
{
    static float distance; 
    switch (keyPress)
    {
    case  VK_LEFT:
        lookAtPosition.x--;
        break;
    case VK_RIGHT:
        lookAtPosition.x++;
        break;
    case  VK_UP:
        //cameraPosition.z--;
        break;
    case VK_DOWN:
        transTime += 0.1f;
        break;
        // ESC 키가 눌리면 프로그램을 종료한다.
    case VK_ESCAPE:
        PostMessage(hWnd, WM_DESTROY, 0L, 0L);
        break;
    }
}



// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;                       // handle to device context 
    RECT rcClient;                 // client area rectangle 
    POINT ptClientUL;              // client upper left corner 
    POINT ptClientLR;              // client lower right corner 
    static POINTS ptsBegin;        // beginning point 
    static POINTS ptsEnd;          // new endpoint 
    static POINTS ptsPrevEnd;      // previous endpoint 
    static BOOL fPrevLine = FALSE; // previous line flag 

    switch (message)
    {
    case WM_KEYDOWN:
        ProcessKeyInput(hWnd, wParam);
        break;
    
   
    case WM_LBUTTONDOWN:

        // Capture mouse input. 

        SetCapture(hWnd);

        // Retrieve the screen coordinates of the client area, 
        // and convert them into client coordinates. 

        GetClientRect(hWnd, &rcClient);
        ptClientUL.x = rcClient.left;
        ptClientUL.y = rcClient.top;

        // Add one to the right and bottom sides, because the 
        // coordinates retrieved by GetClientRect do not 
        // include the far left and lowermost pixels. 

        ptClientLR.x = rcClient.right + 1;
        ptClientLR.y = rcClient.bottom + 1;
        ClientToScreen(hWnd, &ptClientUL);
        ClientToScreen(hWnd, &ptClientLR);

        // Copy the client coordinates of the client area 
        // to the rcClient structure. Confine the mouse cursor 
        // to the client area by passing the rcClient structure 
        // to the ClipCursor function. 
        //diamondCube* test1 = new diamondCube({ 1,1,1 }, { 1,1,1 });
        /*rndidxX = rand() % (-10 - 10 + 1) + 10;
        rndidxZ = rand() % (-10 - 10 + 1) + 10;
        meshClass->CreateMesh<Cube>(new SnowMan({ rndidxX,0,rndidxZ }, { 1,1,1 }));
        meshClass->CreateMesh<Cube>(new SnowManBody({ rndidxX,0,rndidxZ }, { 1,1,1 }));
        meshClass->CreateMesh<Cube>(new SnowManBody({ rndidxX,1.45,rndidxZ }, { 1,0.3,1.3 }));
        meshClass->CreateMesh<Cube>(new SnowManHat({ rndidxX,1.8,rndidxZ }, { 1, 0.5, 0.5 }));*/


        meshClass->CreateMesh<Cube>(new SnowMan({ 0,0,0 }, { 1,1,1 }));
        meshClass->CreateMesh<Cube>(new SnowManBody({ 0,0,0 }, { 1,1,1 }));
        meshClass->CreateMesh<Cube>(new SnowManBody({ 0,1.45,0 }, { 1,0.3,1.3 }));
        meshClass->CreateMesh<Cube>(new SnowManHat({ 0,1.8,0 }, { 1, 0.5, 0.5 }));
        SetRect(&rcClient, ptClientUL.x, ptClientUL.y,
            ptClientLR.x, ptClientLR.y);
        ClipCursor(&rcClient);

        // Convert the cursor coordinates into a POINTS 
        // structure, which defines the beginning point of the 
        // line drawn during a WM_MOUSEMOVE message. 

        ptsBegin = MAKEPOINTS(lParam);
        return 0;

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    } break;
    }

    return DefWindowProc(hWnd, message, wParam, lParam);
}


// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                   // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;    // use 32-bit color
    scd.BufferDesc.Width = SCREEN_WIDTH;                   // set the back buffer width
    scd.BufferDesc.Height = SCREEN_HEIGHT;                 // set the back buffer height
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;     // how swap chain is to be used
    scd.OutputWindow = hWnd;                               // the window to be used
    scd.SampleDesc.Count = 4;                              // how many multisamples
    scd.Windowed = TRUE;                                   // windowed/full-screen mode
    scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;    // allow full-screen switching

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        NULL,
        D3D11_SDK_VERSION,
        &scd,
        &swapchain,
        &dev,
        NULL,
        &devcon);


    // create the depth buffer texture
    D3D11_TEXTURE2D_DESC texd;
    ZeroMemory(&texd, sizeof(texd));

    texd.Width = SCREEN_WIDTH;
    texd.Height = SCREEN_HEIGHT;
    texd.ArraySize = 1;
    texd.MipLevels = 1;
    texd.SampleDesc.Count = 4;
    texd.Format = DXGI_FORMAT_D32_FLOAT;
    texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* pDepthBuffer;
    dev->CreateTexture2D(&texd, NULL, &pDepthBuffer);

    // create the depth buffer
    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
    ZeroMemory(&dsvd, sizeof(dsvd));

    dsvd.Format = DXGI_FORMAT_D32_FLOAT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

    dev->CreateDepthStencilView(pDepthBuffer, &dsvd, &zbuffer);
    pDepthBuffer->Release();

    // get the address of the back buffer
    ID3D11Texture2D* pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();

    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, zbuffer);


    // Set the viewport
    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

    viewport.TopLeftX = 0;    // set the left to 0
    viewport.TopLeftY = 0;    // set the top to 0
    viewport.Width = SCREEN_WIDTH;    // set the width to the window's width
    viewport.Height = SCREEN_HEIGHT;    // set the height to the window's height
    viewport.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
    viewport.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

    devcon->RSSetViewports(1, &viewport);

    InitPipeline();
    InitGraphics();
    InitStates();

    cameraPosition = D3DXVECTOR3(0.0f, 3.0f, 15.0f);
    lookAtPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}


// this is the function used to render a single frame
void RenderFrame(void)
{   
    //
    // create a view matrix cameraPosition
    D3DXMatrixLookAtLH(&matView,
        &cameraPosition,    // the camera position
        &lookAtPosition,    // the look-at position
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));   // the up direction

    // create a projection matrix
    D3DXMatrixPerspectiveFovLH(&matProjection,
        (FLOAT)D3DXToRadian(45),                    // field of view
        (FLOAT)SCREEN_WIDTH / (FLOAT)SCREEN_HEIGHT, // aspect ratio
        1.0f,                                       // near view-plane
        100.0f);

    // set the various states
    devcon->RSSetState(pRS);
    devcon->PSSetSamplers(0, 1, &pSS);
    devcon->OMSetBlendState(pBS, 0, 0xffffffff);

    // clear the back buffer to a deep blue
    devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.545f, 0.862f, 0.933f, 1.0f));

    // clear the depth buffer
    devcon->ClearDepthStencilView(zbuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);


    //오브젝트 업데이트
    //trees->update();
    tree->update();
    terrain->update();
    meshClass->update();
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}


// this is the function that cleans up Direct3D and COM
void CleanD3D(void)
{
    swapchain->SetFullscreenState(FALSE, NULL);    // switch to windowed mode

    // close and release all existing COM objects
    zbuffer->Release();
    pLayout->Release();
    pVS->Release();
    pPS->Release();
    //pVBuffer->Release();
    //pIBuffer->Release();
    pCBuffer->Release();
    //getTexture()->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics()
{
    meshClass = new MeshClass();


    tree = new OakTree({ 1,0,0 });
    terrain = new Terrain({ 0,-2,0 }, { 20,1,20 }, L"ground.png");
 
    meshClass->CreateMesh(new diamondCube({ 1,-1,0 }, { 1,1,1 }));
    
    meshClass->CreateMesh(new Snow({ 0,-2.3,0 }, { 35,1,35 }));
    meshClass->CreateMesh(new Quad({ 5,4,-1 }, { 100,100,1 }, L"sky.png"));
    
}


// this function loads and prepares the shaders
void InitPipeline()
{
    // compile the shaders
    ID3D10Blob* VS, * PS;
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
    D3DX11CompileFromFile(L"shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

    // create the shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input element object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    // use the input element descriptions to create the input layout
    dev->CreateInputLayout(ied, 3, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);

    // create the constant buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = 176;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

    dev->CreateBuffer(&bd, NULL, &pCBuffer);

    devcon->VSSetConstantBuffers(0, 1, &pCBuffer);
}


// initializes the states
void InitStates()
{
    D3D11_RASTERIZER_DESC rd;
    rd.FillMode = D3D11_FILL_SOLID;
    rd.CullMode = D3D11_CULL_BACK;
    rd.FrontCounterClockwise = FALSE;
    rd.DepthClipEnable = TRUE;
    rd.ScissorEnable = FALSE;
    rd.AntialiasedLineEnable = FALSE;
    rd.MultisampleEnable = FALSE;
    rd.DepthBias = 0;
    rd.DepthBiasClamp = 0.0f;
    rd.SlopeScaledDepthBias = 0.0f;

    dev->CreateRasterizerState(&rd, &pRS);

    D3D11_SAMPLER_DESC sd;
    sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sd.MaxAnisotropy = 16;
    sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sd.BorderColor[0] = 0.0f;
    sd.BorderColor[1] = 0.0f;
    sd.BorderColor[2] = 0.0f;
    sd.BorderColor[3] = 0.0f;
    sd.MinLOD = 0.0f;
    sd.MaxLOD = FLT_MAX;
    sd.MipLODBias = 0.0f;

    dev->CreateSamplerState(&sd, &pSS);

    D3D11_BLEND_DESC bd;
    bd.RenderTarget[0].BlendEnable = FALSE;
    bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
    bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
    bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    bd.IndependentBlendEnable = FALSE;
    bd.AlphaToCoverageEnable = FALSE;

    dev->CreateBlendState(&bd, &pBS);
}


