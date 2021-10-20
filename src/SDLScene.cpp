///
/// @file SDLScene.cpp
/// @brief Manages SDL scene and main game loop

#include <SDL_image.h>

#include "SDLScene.h"
#include "Pendulum.h"
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

                // Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << "\n";
					success = false;
				}
            }
        }
    }
    return success;
}

void SDLScene::GameLoop()
{
    bool quit = false;

    // Initialise mouse position
    SDL_GetMouseState(&m_mouseX, &m_mouseY);

    // Event handler
	SDL_Event e;

    // Create pendulum
    Pendulum pendulum(Vec2(256, 0), 256, 25, m_renderer);

    // Time
    double elapsedTime = 0.0f;
    const double frameTime = 1.0f / 60.0f;     // Standardised frame time of 60fps
    double previousTime = SDL_GetTicks();

	// While application is running
	while (!quit)
	{
        // Calculate time between previous and current frames
        double currentTime = SDL_GetTicks();
		double deltaTime = (currentTime - previousTime) / 1000.0f;
		previousTime = currentTime;

		// Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
            /*if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    m_LMBdown = true;
                }
                if (e.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_MMBdown = true;
                }
                if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    m_RMBdown = true;
                }
            }
            if (e.type == SDL_MOUSEBUTTONUP)
            {
                if (e.button.button == SDL_BUTTON_LEFT)
                {
                    m_LMBdown = false;
                }
                if (e.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_MMBdown = false;
                }
                if (e.button.button == SDL_BUTTON_RIGHT)
                {
                    m_RMBdown = false;
                }
            }*/
		}

        // Keyboard input
        m_keyboard.Update();
        if (m_keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

        // Mouse button input
        /*if (m_MMBdown)
        {
            UpdateMousePosition();
            CalculateVelocity();
        }
        else if (m_LMBdown)
        {
            UpdateMousePosition();
        }
        else if (m_RMBdown)
        {
            UpdateMousePosition();
            CalculateVelocity();
        }*/

        pendulum.update(deltaTime);

        // Clear screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0x0);
		SDL_RenderClear(m_renderer);

        pendulum.draw();

        // Update screen
		SDL_RenderPresent(m_renderer);

        // Time between frames and quicker than standardised frame time
        if (deltaTime < frameTime)
		{
			SDL_Delay((unsigned int)((frameTime - deltaTime) * 1000.0f));
		}
        elapsedTime += deltaTime;
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
    IMG_Quit();
    SDL_Quit();
}

void SDLScene::UpdateMousePosition()
{
    m_prevMouseX = m_mouseX;
    m_prevMouseY = m_mouseY;
    SDL_GetMouseState(&m_mouseX, &m_mouseY);
}

void SDLScene::CalculateVelocity()
{
    m_xVel = float(m_mouseX - m_prevMouseX);
    m_yVel = float(m_mouseY - m_prevMouseY);
}