// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "SpriteComponent.h"
#include <vector>
class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	// Update animation every frame (overriden from component)
	void Update(float deltaTime) override;
	// Set the textures used for animation
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
	// Set/get the animation FPS
	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }
	// set start and end frame in the animation
	void SetAnimRange(int start, int end);
	// set animation plays once
	void SetAnimOneOff(bool animOneOff) { mAnimOneOff = animOneOff; }
	// set to false to reverse animation
	void SetAnimForward(bool animForward) { mAnimForward = animForward; }
	bool FrameInRange(float currFrame);
private:
	// All textures in the animation
	std::vector<SDL_Texture*> mAnimTextures;
	// frame range
	int mFrameStart;
	int mFrameEnd;
	// Current frame displayed
	float mCurrFrame;
	// Animation frame rate
	float mAnimFPS;
	// Animation should not loop
	bool mAnimOneOff;
	// Animation direction
	bool mAnimForward;
};