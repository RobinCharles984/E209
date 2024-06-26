HMODULE libgl = LoadLibraryA("opengl32.dll");
PFNGLCULLFACEPROC glCullFace = GetProcAddress((HMODULE)libgl, "glCullFace");

namespace Dummy
{
    HWND hwnd;
    HDC hdc;
    HGLRC hrc;
    WNDCLASSEX wcl;
}

LRESULT CALLBACK DummyGLWndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch (msg)
    {
    case WM_CREATE:
    {
        Dummy::hdc = GetDC( hWnd );

        if (!Dummy::hdc)
        {
            return -1;
        }
    }
        break;

    case WM_DESTROY:
        if (Dummy::hdc)
        {
            if (Dummy::hrc)
            {
                wglMakeCurrent( Dummy::hdc, 0 );
                wglDeleteContext( Dummy::hrc );
                Dummy::hrc = 0;
            }

            ReleaseDC( hWnd, Dummy::hdc );
            Dummy::hdc = 0;
        }

        PostQuitMessage( 0 );
        return 0;

    default:
        break;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

void CreateDummyGLWindow()
{
    Dummy::wcl.cbSize = sizeof( Dummy::wcl );
    Dummy::wcl.style = CS_OWNDC;
    Dummy::wcl.lpfnWndProc = DummyGLWndProc;
    Dummy::wcl.hInstance = reinterpret_cast<HINSTANCE>(GetModuleHandle( nullptr ));
    Dummy::wcl.lpszClassName = "DummyGLWindowClass";

    if (!RegisterClassEx( &Dummy::wcl ))
    {
        OutputDebugStringA( "Failed to register dummy window class!\n" );
    }

    Dummy::hwnd = CreateWindow( Dummy::wcl.lpszClassName, "", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, Dummy::wcl.hInstance, 0 );

    Dummy::hdc = GetDC( Dummy::hwnd );

    PIXELFORMATDESCRIPTOR pfd = { 0 };

    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    int pf = ChoosePixelFormat( Dummy::hdc, &pfd );

    if (!SetPixelFormat( Dummy::hdc, pf, &pfd ))
    {
        OutputDebugStringA( "Failed to set dummy window pixel format!\n" );
    }

    Dummy::hrc = wglCreateContext( Dummy::hdc );

    if (!wglMakeCurrent( Dummy::hdc, Dummy::hrc ))
    {
        OutputDebugStringA( "Failed to make current dummy OpenGL context!\n" );
    }
}

void DestroyDummyGLWindow()
{
    if (Dummy::hwnd)
    {
        PostMessage( Dummy::hwnd, WM_CLOSE, 0, 0 );

        BOOL bRet;
        MSG msg;

        while ((bRet = GetMessage( &msg, 0, 0, 0 )) != 0)
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }

    UnregisterClass( Dummy::wcl.lpszClassName, Dummy::wcl.hInstance );
}

void ChooseAntiAliasingPixelFormat( int& outPixelFormat, int samples )
{
    const int attributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_ACCELERATION_ARB, WGL_FULL_ACCELERATION_ARB,
        WGL_COLOR_BITS_ARB, 24,
        WGL_ALPHA_BITS_ARB, 8,
        WGL_DEPTH_BITS_ARB, 24,
        WGL_STENCIL_BITS_ARB, 8,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_SAMPLE_BUFFERS_ARB, samples > 0 ? 1 : 0,
        WGL_SAMPLES_ARB, samples,
        WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
        0, 0
    };

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress( "wglChoosePixelFormatARB" );
    int returnedPixelFormat = 0;
    UINT numFormats = 0;
    BOOL status = wglChoosePixelFormatARB( Dummy::hdc, attributes, 0, 1, &returnedPixelFormat, &numFormats );

    if (status != FALSE && numFormats > 0)
    {
        outPixelFormat = returnedPixelFormat;
    }
}

void CreateRenderer( int samples )
{
    PIXELFORMATDESCRIPTOR pfd = { 0 };
    pfd.nSize = sizeof( pfd );
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_SUPPORT_COMPOSITION;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 24;
    pfd.cDepthBits = 24;
    pfd.iLayerType = PFD_MAIN_PLANE;

    CreateDummyGLWindow();
    int pixelFormat = 0;
    ChooseAntiAliasingPixelFormat( pixelFormat, samples );
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress( "wglCreateContextAttribsARB" );
    DestroyDummyGLWindow();

    if (!SetPixelFormat( WindowGlobal::hdc, pixelFormat, &pfd ))
    {
        OutputDebugStringA( "Failed to set pixel format!\n" );
    }

    int otherBits = 0;

    const int contextAttribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | otherBits,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    HANDLE context = wglCreateContextAttribsARB( WindowGlobal::hdc, 0, contextAttribs );

    if (!wglMakeCurrent( WindowGlobal::hdc, (HGLRC)context ))
    {
        OutputDebugStringA( "Failed to activate forward compatible context!\n" );
    }
}