///
/// @file SDLScene.cpp
/// @brief Manages SDL scene and main game loop

#include <SDL_image.h>

#include "SDLScene.h"

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

	// While application is running
	while (!quit)
	{
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
        if (m_MMBdown)
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
        }

        // Clear screen
		SDL_SetRenderDrawColor(m_renderer, 0x0, 0x0, 0x0, 0x0);
		SDL_RenderClear(m_renderer);

        // Update

        // Draw



        // Update screen
		SDL_RenderPresent(m_renderer);
	}
    // Destroy
    
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