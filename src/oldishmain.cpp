#include <windows.h>
#include <gdiplus.h>
#include "pug.png.h" // from xxd -i

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

ULONG_PTR gdiplusToken;

// Function to load PNG bytes into a GDI+ Bitmap
Bitmap *LoadPngFromMemory(const unsigned char *data, size_t size)
{
    HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, size);
    memcpy(GlobalLock(hMem), data, size);
    GlobalUnlock(hMem);

    IStream *pStream = nullptr;
    CreateStreamOnHGlobal(hMem, TRUE, &pStream);

    Bitmap *bmp = Bitmap::FromStream(pStream);
    pStream->Release();
    return bmp;
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    static Bitmap *bmp = nullptr;

    switch (msg)
    {
    case WM_CREATE:
        bmp = LoadPngFromMemory(src_pug_png, src_pug_png_len);
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        if (bmp)
        {
            Graphics g(hdc);
            g.Clear(Color(255, 30, 30, 30));
            g.SetInterpolationMode(InterpolationModeNearestNeighbor);
            g.SetPixelOffsetMode(PixelOffsetModeHalf);
            g.DrawImage(bmp, 0, 0, 100, 100);
        }

        EndPaint(hwnd, &ps);
    }
    break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nShowCmd)
{
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

    // Register a simple window class
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = "PngWindowClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0, wc.lpszClassName, "Embedded PNG Viewer",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400,
        nullptr, nullptr, hInst, nullptr);

    // Run message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return 0;
}
