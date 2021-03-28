// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	//, mCurrFrame(2.0f)
	, mAnimFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
		SpriteComponent::Update(deltaTime);

		if (mAnimTextures.size() > 0)
		{
			if (mAnimOneOff) {
				// run the animation, no cycles
				if (FrameInRange(mCurrFrame)) {
					mCurrFrame += mAnimFPS * deltaTime;
					SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
				}
			}
			else {
				// Update the current frame based on frame rate
				// and delta time
				mCurrFrame += mAnimFPS * deltaTime;
				// Wrap current frame if needed
				// if we have a custom range
				if (mFrameEnd > 0) {
					while (static_cast<int>(mCurrFrame) > mFrameEnd)
					{
						// always rewind at least one frame + diff (end - start)
						mCurrFrame -= 1+(mFrameEnd - mFrameStart);
					}
					SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
				}
				else {
					while (mCurrFrame >= mAnimTextures.size())
					{
						mCurrFrame -= mAnimTextures.size();
					}
					SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);
				}
			}
		}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		if (mFrameStart >= 0.0f) {
			mCurrFrame = mFrameStart;
		} else {
			mCurrFrame = 0.0f;
		}
		SetTexture(mAnimTextures[0]);
	}
}

void AnimSpriteComponent::SetAnimRange(int start, int end) {
	mFrameStart = start;
	mCurrFrame = start;
	mFrameEnd = end;
}

bool AnimSpriteComponent::FrameInRange(float currFrame) {	
	int currFrameInt = static_cast<int>(currFrame);
	return (currFrame < mAnimTextures.size() - 1
		&& currFrameInt >= mFrameStart
		&& currFrameInt <= mFrameEnd);
}
