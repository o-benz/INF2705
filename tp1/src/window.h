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
    };
    
public:
    Window();
    ~Window();
    
    bool init();
    
    void swap();    
    void pollEvent();
    
    bool getKey(Key k);
    
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
};

#endif // WINDOW_H
