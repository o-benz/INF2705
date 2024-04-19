#include "window.h"

#include <iostream>


Window::Window()
: m_window(nullptr)
, m_context(nullptr)
, m_shouldClose(false)
, m_shouldResize(false)
, m_width(0), m_height(0)
, m_mouseX(0)
, m_mouseY(0)
, m_scroll(0)
{

}

Window::~Window()
{
    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

bool Window::init()
{
    const Uint32 flags = SDL_INIT_VIDEO | SDL_INIT_EVENTS;
    if (SDL_Init(flags) < 0)
    {
        std::cout << "Could not initialize SDL! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    m_window = SDL_CreateWindow("INF2705 - Tp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (!m_window)
    {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    SDL_GetWindowSize(m_window, &m_width, &m_height);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    m_context = SDL_GL_CreateContext(m_window);
    if (!m_context)
    {
        std::cout << "Context could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_ClearError();
        return false;
    }

    const int VSYNC = 1; // 1 on, 0 off, -1 adaptive
    SDL_GL_SetSwapInterval(VSYNC);

    return true;
}

void Window::swap()
{
    SDL_GL_SwapWindow(m_window);
}

void Window::pollEvent()
{
    m_scroll = 0;
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            m_shouldClose = true;
            break;
        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                m_width  = e.window.data1;
                m_height = e.window.data2;
                m_shouldResize = true;
            }
            else if (e.window.event == SDL_WINDOWEVENT_SHOWN)
            {
                SDL_GetWindowSize(m_window, &m_width, &m_height);
                m_shouldResize = true;
            }
            break;
        case SDL_KEYDOWN:
            if (e.key.repeat) break; // disable key hold for now
            m_keys[(Key)e.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            m_keys[(Key)e.key.keysym.sym] = false;
            break;
        case SDL_MOUSEMOTION:
            m_mouseX += e.motion.xrel;
            m_mouseY += e.motion.yrel;
            break;
        case SDL_MOUSEWHEEL:
            if (e.wheel.y > 0) m_scroll = 1;
            else if (e.wheel.y < 0) m_scroll = -1;
            break;
        }
    }
}


void Window::getMouseMotion(int& x, int& y)
{
    x = m_mouseX; y = m_mouseY;
    m_mouseX = m_mouseY = 0;
}

int Window::getMouseScrollDirection()
{
    return m_scroll;
}

bool Window::getKeyHold(Key k)
{
    return m_keys[k];
}

bool Window::getKeyPress(Key k)
{
    bool state = m_keys[k];
    m_keys[k] = false;
    return state;
}

unsigned int Window::getTick()
{
    return SDL_GetTicks();
}

bool Window::shouldClose()  { return m_shouldClose; }
bool Window::shouldResize() { return m_shouldResize; }

int Window::getWidth()  { return m_width;  }
int Window::getHeight() { return m_height; }
