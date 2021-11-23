///
/// @file SDLScene.cpp
/// @brief Manages SDL scene and main game loop

#include "SDLScene.h"
#include "Vec2.h"

#include <iostream>

SDLScene::SDLScene()
{
}

bool SDLScene::Initialise()
{
    bool success = true;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
    }
    else
    {
        // Create window
        m_window = SDL_CreateWindow("Pendulum Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_SCREEN_SIZE, m_SCREEN_SIZE, SDL_WINDOW_SHOWN);
        if (m_window == NULL)
        {
            std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
            success = false;
        }
        else
        {
            // Create renderer for window
			m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
			if (m_renderer == NULL)
			{
				std::cout << "Renderer could not be created! SDL Error: " << SDL_GetError() << "\n";
				success = false;
			}
            else
            {
                // Initialize renderer color
				SDL_SetRenderDrawColor(m_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
            }
        }
    }
    return success;
}

void SDLScene::GameLoop()
{
    bool quit = false;

    // Event handler
	SDL_Event e;

    // Create pendulums
    for (int i = 0; i < m_maxPendulums; ++i)
    {
        Pendulum pendulum(Vec2(256, 0), 512 - (i * 16), 25, i * 8, m_renderer);
        m_pendulums.push_back(pendulum);
    }

    // Time
    float elapsedTime = 0.0f;
    const float frameTime = 1.0f / 60.0f;     // Standardised frame time of 60fps
    Uint32 previousTime = SDL_GetTicks();

	// While application is running
	while (!quit)
	{
        // Calculate time between previous and current frames
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - previousTime) / 1000.0f;
        previousTime = currentTime;

        // Handle events on queue
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // Keyboard input
        m_keyboard.Update();

        // Quit
        if (m_keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        // Pause
        if (m_keyboard.GetKeyDown(SDL_SCANCODE_P))
        {
            if (!m_pause)
            {
                m_pause = true;
            }
            else
            {
                m_pause = false;
            }
        }

        // Reset
        if (m_keyboard.GetKeyDown(SDL_SCANCODE_R))
        {
            for (int i = 0; i < m_pendulums.size(); ++i)
            {
                m_pendulums[i].reset(Vec2(256, 0), 512 - (16 * i), 25);
            }
        }

        if (!m_pause)
        {
            for (int i = 0; i < m_pendulums.size(); ++i)
            {
                m_pendulums[i].update(deltaTime);
            }

            // Clear screen
            SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0x0);
            SDL_RenderClear(m_renderer);

            for (int i = 0; i < m_pendulums.size(); ++i)
            {
                m_pendulums[i].draw();
            }

            // Update screen
            SDL_RenderPresent(m_renderer);

            // Time between frames and quicker than standardised frame time
            if (deltaTime < frameTime)
            {
                SDL_Delay((unsigned int)((frameTime - deltaTime) * 1000.0f));
            }
            elapsedTime += deltaTime;
        }
	}
    Close();
}

void SDLScene::Close()
{
    // Destroy renderer
    SDL_DestroyRenderer(m_renderer);
    m_renderer = NULL;

    // Destroy window
    SDL_DestroyWindow(m_window);
    m_window = NULL;

    // Quit SDL subsystems
    SDL_Quit();
}