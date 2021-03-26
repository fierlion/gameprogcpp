// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Game.h"

const int thickness = 15;
const int edgeBuffer = 10;
const float paddleH = 100.0f;

Game::Game()
:mWindow(nullptr)
,mRenderer(nullptr)
,mTicksCount(0)
,mIsRunning(true)
{
	
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	
	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Chapter 1)", // Window title
		100,	// Top left x-coordinate of window
		100,	// Top left y-coordinate of window
		1024,	// Width of window
		768,	// Height of window
		0		// Flags (0 for no flags set)
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}
	
	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	paddle1 = Paddle{};
	paddle1.PaddlePos.x = 10.0f;
	paddle1.PaddlePos.y = 768.0 / 2.0f;
	paddle1.PaddleDir = 0;

	paddle2 = Paddle{};
	paddle2.PaddlePos.x = 1024 - thickness - edgeBuffer;
	paddle2.PaddlePos.y = 768.0f / 2.0f;
	paddle1.PaddleDir = 0;

	ball1.BallPos.x = 1024.0f / 2.0f;
	ball1.BallPos.y = 768.0f / 2.0f;
	ball1.BallVel.x = -200.0f;
	ball1.BallVel.y = 235.0f;

	ball2.BallPos.x = 768.0f / 2.0f;
	ball2.BallPos.y = 1024.0f / 2.0f;
	ball2.BallVel.x = -200.0f;
	ball2.BallVel.y = 235.0f;

	balls.push_back(&ball1);
	balls.push_back(&ball2);

	return true;
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}
	
	// Get state of keyboard
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If escape is pressed, also end loop
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	// Update paddle1 direction based on W/S keys
	// Update paddle2 direction based on R/F keys
	paddle1.PaddleDir = 0;
	paddle2.PaddleDir = 0;

	if (state[SDL_SCANCODE_W])
	{
		paddle1.PaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_S])
	{
		paddle1.PaddleDir += 1;
	}
	if (state[SDL_SCANCODE_R])
	{
		paddle2.PaddleDir -= 1;
	}
	if (state[SDL_SCANCODE_F])
	{
		paddle2.PaddleDir += 1;
	}
}

void Game::UpdateGame()
{
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// Delta time is the difference in ticks from last frame
	// (converted to seconds)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

	// Update tick counts (for next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle1/paddle2 position based on direction
	paddle1.UpdatePaddlePos(deltaTime);
	paddle2.UpdatePaddlePos(deltaTime);

	for (auto& thisBall : balls) {
	 	thisBall->BallPos.y += thisBall->BallVel.y * deltaTime;
	    thisBall->BallPos.x += thisBall->BallVel.x * deltaTime;

		// Bounce if needed
		// Did we intersect with paddle 1?
		float diff = paddle1.PaddlePos.y - thisBall->BallPos.y;
		// Take absolute value of difference
		diff = (diff > 0.0f) ? diff : -diff;
		if (
			// Our y-difference is small enough
			diff <= paddleH / 2.0f &&
			// We are in the correct x-position
			thisBall->BallPos.x <= 25.0f && thisBall->BallPos.x >= 20.0f &&
			// The ball is moving to the left
			thisBall->BallVel.x < 0.0f)
		{
			thisBall->BallVel.x *= -1.0f;
		}

		// Did we intersect with paddle 2?
		float diff2 = paddle2.PaddlePos.y - thisBall->BallPos.y;
		diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
		if (
			// Our y-difference is small enough
			diff2 <= paddleH / 2.0f &&
			// We are in the correct x-position
		    thisBall->BallPos.x >= (1024.0f - thickness - edgeBuffer) &&
			// The ball is moving to the right
			thisBall->BallVel.x >= 0.0f)
		{
			thisBall->BallVel.x *= -1.0f;
		}
		// Did the ball go off the screen? (if so, end game)
		if (thisBall->BallPos.x <= 0.0f || thisBall->BallPos.x >= 1024.0f)
		{
			mIsRunning = false;
		}

		// Did the ball collide with the top wall?
		if (thisBall->BallPos.y <= thickness && thisBall->BallVel.y < 0.0f)
		{
			thisBall->BallVel.y *= -1;
		}
		// Did the ball collide with the bottom wall?
		else if (thisBall->BallPos.y >= (768 - thickness) &&
			thisBall->BallVel.y > 0.0f)
		{
			thisBall->BallVel.y *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Set draw color to blue
	SDL_SetRenderDrawColor(
		mRenderer,
		0,		// R
		0,		// G 
		255,	// B
		255		// A
	);
	
	// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
	
	// Draw top wall
	SDL_Rect wall{
		0,			// Top left x
		0,			// Top left y
		1024,		// Width
		thickness	// Height
	};
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);
	
	// Draw paddle1
	SDL_Rect paddleDraw1{
		static_cast<int>(paddle1.PaddlePos.x),
		static_cast<int>(paddle1.PaddlePos.y - paddleH/2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleDraw1);

	// Draw paddle2
	SDL_Rect paddleDraw2{
		static_cast<int>(paddle2.PaddlePos.x),
		static_cast<int>(paddle2.PaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddleDraw2);
	

	for (auto& thisBall : balls) {
		// Draw ball
		SDL_Rect ball{
			static_cast<int>(thisBall->BallPos.x - thickness / 2),
			static_cast<int>(thisBall->BallPos.y - thickness / 2),
			thickness,
			thickness
		};
		SDL_RenderFillRect(mRenderer, &ball);
	}
	
	// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown()
{
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Paddle::UpdatePaddlePos(float deltaTime)
{
	// Update paddle1 position based on direction
	if (PaddleDir != 0)
	{
		PaddlePos.y += PaddleDir * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (PaddlePos.y < (paddleH / 2.0f + thickness))
		{
			PaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (PaddlePos.y > (768.0f - paddleH / 2.0f - thickness))
		{
			PaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}
}
