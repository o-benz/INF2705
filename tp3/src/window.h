#ifndef WINDOW_H
#define WINDOW_H

#include <unordered_map>
#include <SDL.h>


class Window
{
public:
    enum Key
    {
        ESC = SDLK_ESCAPE,
        T = SDLK_t,
        W = SDLK_w,
        A = SDLK_a,
        S = SDLK_s,
        D = SDLK_d,
        Q = SDLK_q,
        E = SDLK_e,
        SPACE = SDLK_SPACE
    };
    
public:
    Window();
    ~Window();
    
    bool init();
    
    void swap();    
    void pollEvent();
    
    bool getKeyHold(Key k);
    bool getKeyPress(Key k);
    
    void getMouseMotion(int& x, int& y);
    int getMouseScrollDirection();

    void showMouse();
    void hideMouse();
    
    unsigned int getTick();
    
    bool shouldClose();
    bool shouldResize();
    
    int getWidth();
    int getHeight();

private:
    SDL_Window* m_window;
    SDL_GLContext m_context;
    
    bool m_shouldClose;
    bool m_shouldResize;
    int m_width, m_height;
    
    std::unordered_map<Key, bool> m_keys;
    int m_mouseX, m_mouseY, m_scroll;
};

#endif // WINDOW_H
