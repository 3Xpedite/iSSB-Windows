
#include "../GameEngine/GameEngine.h"

#pragma once

namespace SmashBros
{
	class HUD
	{
	private:
		class CharacterPanel
		{
		private:
			int percent;
			byte playerNo;
			friend class HUD;
			Actor*percentText;
			Actor*symbol;
			Actor*stockIcon;
			TextActor*stockText;
			TextActor*playerName;
			TextActor*scoreText;
			long scoreTime;
			boolean scoreShowing;

		public:
			float x,y;

			CharacterPanel(HUD*hud, byte playerNo);
			virtual ~CharacterPanel();

			virtual void Update(long gameTime);
			virtual void Draw(Graphics2D&g, long gameTime);

		};

		class OptionPanel : public Actor
		{
		private:
			int min;
			int max;
			int value;

			static const unsigned int textSize = 12;
			static const int arrowXOffset = 48;
			static const int labelYOffset = 8;

			Actor*left;
			Actor*right;
			TextActor*label;

		public:
			OptionPanel(float x1, float y1, String anim);
			virtual ~OptionPanel();

			virtual void Update(long gameTime);
			virtual void Draw(Graphics2D&g, long gameTime);

			void setMin(int min);
			void setMax(int max);
			void setValue(int value);
			void setLabel(String label);
			int getValue();
		};

		class TrainingMenu
		{
		private:
			Actor*bg;
			Actor*tab;
			ArrayList<OptionPanel*> options;

			Actor*apply;
			Actor*finish;

			boolean opened;
			
			void applyLabels(long gameTime);
			void applyChanges();
			
			void onOpen();
			void onClose();

		public:
			static const int OPTION_ITEMS = 0;
			static const int OPTION_CPU = 1;
			static const int OPTION_CAMERA = 2;

			TrainingMenu(HUD*hud);
			virtual ~TrainingMenu();

			virtual void Update(long gameTime);
			virtual void Draw(Graphics2D&g, long gameTime);
		};

		class ReadyGoActor : public Actor
		{
		private:
			HUD*hud;
		public:
			ReadyGoActor(float x1, float y1, HUD*hud);
			virtual ~ReadyGoActor();

			virtual void onAnimationFinish(const String&n);
		};

		long startGameTime;
		boolean showReadyGo;
		boolean showFinishGame;
		friend class Global;
		long finishGameTime;
		friend class Global;
		ReadyGoActor*ReadyGo;
		Actor*FinishGame;
		TextActor*currentTime;
		long prevWorldTime;
		TrainingMenu*trainingMenu;

		boolean firstUpdate;
		ArrayList<CharacterPanel*> charPanels;
		
		ArrayList<String> files;

		CharacterPanel*getCharPanel(byte pNum);

	public:
		HUD();
		virtual ~HUD();

		void Load();
		void Unload();

		virtual void Update(long gameTime);
		virtual void Draw(Graphics2D&g, long gameTime);

		void showScore(byte pNum, int amount);
	};
}