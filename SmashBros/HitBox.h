
#include "../GameEngine/GameEngine.h"

#pragma once

namespace SmashBros
{
	class HitBox : public WireframeActor
	{
	private:
		byte playerNo;
		friend class Player;

	public:
		HitBox(byte playerNo, float x1, float y1, int w, int h);
		virtual ~HitBox();

		virtual void Draw(Graphics2D&g, long gameTime);
	};
}