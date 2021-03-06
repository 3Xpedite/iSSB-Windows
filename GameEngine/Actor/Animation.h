
#include "../Global.h"
#include "../Util/String.h"
#include "../Util/ArrayList.h"
#include "../Graphics/Graphics2D.h"
#include "../Util/Vector2.h"

#pragma once

namespace GameEngine
{
	const unsigned char NO_CHANGE = 0;
	const unsigned char FORWARD = 1;
	const unsigned char BACKWARD = 2;
	const unsigned char STOPPED = 3;

	class Animation
	{
	private:
		int frames;
		ArrayList<String> names;

		int currentFrame;
		int recentFrame;

		bool mirrored;
		bool mirroredVertical;
		int rows;
		int cols;

		int*sequence;

		sf::Vector2i imageSize;

		int animType;

		int width;
		friend class Actor;
		int height;
		friend class Actor;

		unsigned char direction;
		friend class Actor;
	public:
		static const int TYPE_MULTIFILE = 0;
		static const int TYPE_SINGLEFILE = 1;
		static const int TYPE_SPECFRAME_SINGLEFILE = 2;

		int fps;
		String name;

		Animation(const Animation& anim);
		Animation(const String&name,int speed);
		Animation(const String&name,int speed,const String&frame);
		Animation(const String&name,int speed,int rows,int cols);
		Animation(const String&name,int speed,int rows,int cols,const ArrayList<int>&seq);
		virtual ~Animation();

		void mirror(bool toggle);
		void mirrorVertical(bool toggle);
		bool isMirrored();
		bool isMirroredVertical();

		int getWidth();
		int getHeight();
		Vector2i getSize();
		
		void addFrame(const String&fImage);
		void setFrame(int frameNo, const String&fImage);
		const String& getFrame(int frameNo);
		int getTotalFrames();

		BufferedImage*getCurrentImage();
		int getCols();
		int getRows();
		int getType();
		
		void drawFrame(Actor*a, Graphics2D& g, float x1, float y1, double scale);
		void drawFrame(Actor*a, Graphics2D& g, int fNum, float x1, float y1, double scale);
		void drawFrame(Actor*a, Graphics2D& g, int fNum, float x1, float y1, double scale, bool relativeToScreen);
		void drawFrame(Actor*a, Graphics2D& g, int fNum, float x1, float y1, float x2, float y2);
		void nextFrame(Actor*a, Graphics2D& g, long gameTime, float x1, float y1, double scale, bool relativeToScreen=true);

		void setCurrentFrame(int fNum);
		int getLastFrame();
		int getCurrentFrame();
		int getSequenceFrame(int seqNum);
		
		ArrayList<String> getAllFilenames();

		Animation& operator=(const Animation& anim);
	};
}