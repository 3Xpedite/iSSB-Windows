
#include "TextActor.h"
#include "../View.h"
#include "../Application.h"
#include "../Output/Console.h"
#include "../Util/ArrayList.h"

namespace GameEngine
{
	void TextActor::drawString(Graphics2D& graphics, String text, float x1, float y1)
	{
		Graphics2D g(graphics);
		g.scale(Scale, Scale);
		x1 = x1 / Scale;
		y1 = y1 / Scale;

		String currentLine = "";
		ArrayList<String> lines = ArrayList<String>();
			
		for(int i=0; i<text.length(); i++)
		{
			char c = text.charAt(i);
				
			if(c=='\n')
			{
				lines.add(currentLine);
				currentLine = "";
				currentLine.clear();
			}
			else
			{
				currentLine+=c;
			}
		}
		lines.add(currentLine);
			
		float currentY = y1;
			
		for(int i=(lines.size()-1); i>=0; i--)
		{
			String line = lines.get(i);

			sf::Text txt = sf::Text((char*)line,*this->font->getFont());
			txt.setCharacterSize(fontSize);
			txt.setStyle(fontStyle);
			int lineWidth = (int)txt.getLocalBounds().width;
			int offset = 0;
			switch(alignment)
			{
				case ALIGN_BOTTOMLEFT:
				case ALIGN_TOPLEFT:
				offset = 0;
				break;
					
				case ALIGN_CENTER:
				offset = (width - lineWidth)/2;
				break;
				
				case ALIGN_BOTTOMRIGHT:
				case ALIGN_TOPRIGHT:
				offset = width - lineWidth;
				break;
			}
			g.drawString(line, x1 + offset, currentY);
			currentY-= (float)((unsigned int)charHeight+lineSpace);
		}

		lines.clear();
	}

	void TextActor::updateSize()
	{
		int tempheight = fontSize;

		int lines = 1;
		for( int pos = 0; pos < text.length(); pos++)
		{
			char c = text.charAt(pos);
			if( c == '\r' || c== '\n' )
			{
				lines++;
			}
		}
			
		charHeight = tempheight;
			
		height = (tempheight*lines) + (lineSpace*(lines-1)); //got the height!

		ArrayList<int> length = ArrayList<int>();
		ArrayList<String> line = ArrayList<String>();
		line.add("");
		length.add(0);

		for( int pos = 0; pos < text.length(); pos++)
		{
			char c = text.charAt(pos);
			if( c == '\r' || c== '\n' )
			{
				length.add(0);
				line.add("");
			}
			else
			{
				String newS = line.get(line.size()-1);
				newS+=c;
				line.set(line.size()-1, newS);
				int i = length.get(length.size()-1);
				i++;
				length.set(length.size()-1, i);
			}
		}
		int longest = 0;
		if(length.size()>1)
		{
			for(int i=1; i<length.size(); i++)
			{
				int int1 = length.get(i);
				int intlongest = length.get(longest);
				if(int1>intlongest)
				{
					longest = i;
				}
			}
		}
		
		sf::Text txt = sf::Text((char*)line.get(longest), *font->getFont());
		txt.setCharacterSize((unsigned int)fontSize);
		txt.setStyle(font->getStyle());
		
		width = (int)txt.getLocalBounds().width;

		txt.setString("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");
		charWidth = (int)(txt.getLocalBounds().width/52);
		
		line.clear();
		length.clear();

		width = (int)(width*Scale);
		height = (int)(height*Scale);
	}

	float TextActor::getCenterX()
	{
		switch(alignment)
		{
			default:
			case ALIGN_BOTTOMLEFT:
			return this->x + this->width/2;
				
			case ALIGN_BOTTOMRIGHT:
			return this->x - this->width/2;
				
			case ALIGN_CENTER:
			return this->x;
				
			case ALIGN_TOPLEFT:
			return this->x + this->width/2;
				
			case ALIGN_TOPRIGHT:
			return this->x - this->width/2;
		}
	}
		
	float TextActor::getCenterY()
	{
		switch(alignment)
		{
			default:
			case ALIGN_BOTTOMLEFT:
			return this->y - this->height/2;
				
			case ALIGN_BOTTOMRIGHT:
			return this->y - this->height/2;
				
			case ALIGN_CENTER:
			return this->y;
				
			case ALIGN_TOPLEFT:
			return this->y + this->height/2;
				
			case ALIGN_TOPRIGHT:
			return this->y + this->height/2;
		}
	}
		
	Rectangle TextActor::getRect()
	{
		return Rectangle((int)(getCenterX() - width/2), (int)(getCenterY() - height/2), width, height);
	}

	void TextActor::drawActor(Graphics2D& graphics, long gameTime, bool relativeToScreen)
	{
		if(Scale == 0)
		{
			updateSize();
			return;
		}

		Graphics2D g(graphics);

		unsigned int originalSize = font->getSize();
		int originalStyle = font->getStyle();
		this->font->setSize(fontSize);
		this->font->setStyle(fontStyle);

		//g.setComposite((AlphaComposite.getInstance(AlphaComposite.SRC_OVER, Alpha)));
			
		if(antialiasing)
		{
			//graphics.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);
		}
		
		updateSize();
		
		g.setColor(color);
		g.setAlpha((unsigned char)(255 - (Alpha*255)));
		g.setFont(this->font);
			
		float x1;
		float y1;
		if(relativeToScreen)
		{
			x1 = x - View::x;
			y1 = y - View::y;
		}
		else
		{
			x1 = x;
			y1 = y;
		}
			
		bool onscreen = isOnScreen();

		float width = (float)(this->width);
		float height = (float)(this->height);

		this->width = (int)(this->width/Scale);
		this->height = (int)(this->height/Scale);
		
		if((onscreen && relativeToScreen)||(!relativeToScreen))
		{
			switch(alignment)
			{
				default:
				alignment = ALIGN_BOTTOMLEFT;
					
				case ALIGN_BOTTOMLEFT:
				g.rotate((float)Rotation,x1,y1);
				drawString(g, text, x1, y1);
				break;
					
				case ALIGN_BOTTOMRIGHT:
				g.rotate((float)Rotation,x1-width,y1);
				drawString(g, text, x1-width, y1);
				break;
				
				case ALIGN_CENTER:
				g.rotate((float)Rotation,x1-((float)width/2),y1+((float)height/2));
				drawString(g, text, x1-((float)width/2), y1+((float)height/2));
				break;
					
				case ALIGN_TOPLEFT:
				g.rotate((float)Rotation,x1,y1+height);
				drawString(g, text, x1, y1+height);
				break;
					
				case ALIGN_TOPRIGHT:
				g.rotate((float)Rotation,x1-width,y1+height);
				drawString(g, text, x1-width, y1+height);
				break;
			}
		}
			
		if(showwire && ((onscreen && relativeToScreen)||(!relativeToScreen)))
		{
			g.setColor(wireframeColor);
			switch(alignment)
			{
				default:
				alignment = ALIGN_BOTTOMLEFT;
				
				case ALIGN_BOTTOMLEFT:
				g.drawRect(x1, y1-height, (float)width, (float)height);
				break;
				
				case ALIGN_BOTTOMRIGHT:
				g.drawRect(x1-width, y1-height, (float)width, (float)height);
				break;
				
				case ALIGN_CENTER:
				g.drawRect(x1-((float)width/2), y1-((float)height/2), (float)width, (float)height);
				break;
				
				case ALIGN_TOPLEFT:
				g.drawRect(x1, y1, (float)width, (float)height);
				break;
				
				case ALIGN_TOPRIGHT:
				g.drawRect(x1-width, y1, (float)width, (float)height);
				break;
			}
		}

		font->setSize(originalSize);
		font->setStyle(originalStyle);

		this->width = (int)(width);
		this->height = (int)(height);
	}

	static const unsigned char ALIGN_BOTTOMLEFT = 0;
	static const unsigned char ALIGN_BOTTOMRIGHT = 1;
	static const unsigned char ALIGN_CENTER = 2;
	static const unsigned char ALIGN_TOPLEFT = 3;
	static const unsigned char ALIGN_TOPRIGHT = 4;
		
	TextActor::TextActor(const String&s, Font*f, const Color&c)
	{
		alignment = ALIGN_BOTTOMLEFT;
		text = s;
		font = f;
		color = c;
		x=0;
		y=0;

		fontSize = font->getSize();
		fontStyle = font->getStyle();

		originalText = text;
		actorType = 3;

		wireframeColor = Color::GREEN;
		Rotation = 0;
		Alpha = 0;
		Scale=1;
		charWidth = 0;
		charHeight = 0;
		lineSpace = 0;
		mouseover = false;
		showwire=false;
		clicked = false;
		antialiasing = false;
		relative = true;

		updateSize();
	}
	
	TextActor::TextActor(float x1, float y1, const String&s, Font*f, const Color&c)
	{
		alignment = ALIGN_BOTTOMLEFT;
		text = s;
		font = f;
		color = c;
		x=x1;
		y=y1;

		fontSize = font->getSize();
		fontStyle = font->getStyle();

		originalText = text;
		actorType = 3;

		wireframeColor = Color::GREEN;
		Rotation = 0;
		Alpha = 0;
		Scale=1;
		charWidth = 0;
		charHeight = 0;
		lineSpace = 0;
		mouseover = false;
		showwire=false;
		clicked = false;
		antialiasing = false;
		relative = true;

		updateSize();
	}

	TextActor::~TextActor()
	{
		//
	}
		
	const String& TextActor::toString()
	{
		return originalText;
	}
		
	void TextActor::formatToBox(int width, int height)
	{
		String newText = "";
		
		int currentWidth = 0;
		float currentHeight = 0;
			
		bool spaceInLine = false;
			
		String currentWord = "";
			
		for(int i=0; i<originalText.length(); i++)
		{
			char c = originalText.charAt(i);
			if(c=='\n')
			{
				newText+=currentWord + '\n';
				currentWidth = 0;
				currentHeight += sf::Text((const char*)currentWord,*font->getFont(),(unsigned int)fontSize).getLocalBounds().height + (float)lineSpace;
				currentWord = "";
				spaceInLine = false;
				if(currentHeight>((float)height))
				{
					i = originalText.length();
				}
					
			}
			else
			{
				if(c==' ')
				{
					newText+=currentWord + ' ';
					currentWidth+=charWidth;
					currentWord = "";
					spaceInLine = true;
						
					if(currentWidth>=((float)width))
					{
						newText+='\n';
						currentWidth = 0;
						currentHeight += sf::Text((const char*)currentWord,*font->getFont(),(unsigned int)fontSize).getLocalBounds().height + (float)lineSpace;
						spaceInLine = false;
						if(currentHeight > ((float)height))
						{
							i = originalText.length();
						}
							
					}
				}
				else
				{
					currentWord+=c;
					currentWidth+=charWidth;
						
					if(currentWidth>=((float)width))
					{
						if(spaceInLine)
						{
							newText+='\n';
							currentHeight += sf::Text((const char*)currentWord,*font->getFont(),(unsigned int)fontSize).getLocalBounds().height + (float)lineSpace;
							currentWidth = 0;//currentWord.length()*charWidth;
							spaceInLine = false;
								
							if(currentHeight > ((float)height))
							{
								i = originalText.length();
							}
						}
					}
				}
			}
		}

		newText+=currentWord;
		
		text = newText;
		updateSize();
	}
		
	void TextActor::Update(long gameTime)
	{
		prevclicked = clicked;

		if(mouseOver())
		{
			if(!mouseover)
			{
				mouseover=true;
				onMouseEnter();
			}
			if(Application::MouseState(Mouse::LEFTCLICK) && !Application::PrevMouseState(Mouse::LEFTCLICK))
			{
				clicked = true;
				onClick();
			}
		}
		else if(mouseover)
		{
			mouseover=false;
			onMouseLeave();
		}
		if(clicked && !Application::MouseState(Mouse::LEFTCLICK))
		{
			clicked = false;
			onRelease();
		}
	}
		
	void TextActor::Draw(Graphics2D& g, long gameTime)
	{
		drawActor(g,gameTime,relative);
	}
		
	bool TextActor::isOnScreen()
	{
		int left1, left2;
		int right1, right2;
		int top1, top2;
		int bottom1, bottom2;
			
		Rectangle rect = getRect();
	
		left1 = (int)rect.x;
		left2 = (int)View::x;
		right1 = (int)rect.x+rect.width;
		right2 = (int)View::x+(View::ScaleWidth());
		top1 = (int)rect.y;
		top2 = (int)View::y;
		bottom1 = (int)rect.y+rect.height;
		bottom2 = (int)View::y+(View::ScaleHeight());
			
		if (bottom1 < top2)
		{
			return false;
		}
		if (top1 > bottom2)
		{
			return false;
		}
	
		if (right1 < left2)
		{
			return false;
		}
		if (left1 > right2)
		{
			return false;
		}
	
		return true;
	}
		
	void TextActor::showWireframe(bool toggle)
	{
		showwire = toggle;
	}
		
	void TextActor::setWireframeColor(const Color&c)
	{
		wireframeColor = c;
	}
		
	void TextActor::setText(const String&s)
	{
		text = s;
		originalText = text;
		updateSize();
	}
		
	const String& TextActor::getText()
	{
		return text;
	}
		
	void TextActor::setFont(Font*f)
	{
		font = f;
		updateSize();
	}
		
	Font*TextActor::getFont()
	{
		return font;
	}
		
	void TextActor::setSize(unsigned int size)
	{
		fontSize = size;
		updateSize();
	}
		
	int TextActor::getSize()
	{
		return fontSize;
	}
	
	void TextActor::setStyle(int style)
	{
		fontStyle = style;
		updateSize();
	}

	int TextActor::getStyle()
	{
		return fontStyle;
	}
	
	void TextActor::setColor(const Color&c)
	{
		color = c;
	}
		
	const Color& TextActor::getColor()
	{
		return color;
	}
		
	void TextActor::setAlignment(unsigned char align)
	{
		if(align>=0 && align<=4)
		{
			alignment = align;
		}
		else
		{
			Console::WriteLine((String)"Error: Invalid argument " + align + (String)" for method setAlignment(unsigned char)");
		}
	}
	
	unsigned char TextActor::getAlignment()
	{
		return alignment;
	}

	void TextActor::setLineSpacing(unsigned int space)
	{
		lineSpace = space;
		updateSize();
	}
	
	void TextActor::setRelativeToView(bool toggle)
	{
		relative = toggle;
	}

	bool TextActor::isRelativeToView()
	{
		return relative;
	}

	void TextActor::onMouseEnter() //When mouse enters Actor
	{
		//Open for implementation
	}
		
	void TextActor::onMouseLeave() //When mouse enters Actor
	{
		//Open for implementation
	}
		
	void TextActor::onClick() //When Actor is clicked
	{
		//Open for implementation
	}
		
	void TextActor::onRelease() //When Actor's click is released
	{
		//Open for implementation
	}
		
	bool TextActor::mouseOver()
	{
		int mousex = 0;
		int mousey = 0;
		if(relative)
		{
			mousex = (int)(Application::MouseX() + View::x);
			mousey = (int)(Application::MouseY() + View::y);
		}
		else
		{
			mousex = Application::MouseX();
			mousey = Application::MouseY();
		}
		switch(alignment)
		{
			case ALIGN_BOTTOMLEFT:
			if (mousex<(x+width) && mousex>x && mousey<y && mousey>(y-height))
			{
				return true;
			}
			break;
				
			case ALIGN_BOTTOMRIGHT:
			if (mousex<(x-width) && mousex>x && mousey<y && mousey>(y-height))
			{
				return true;
			}
			break;
				
			case ALIGN_CENTER:
			if (mousex<(x+(width/2)) && mousex>(x-(width/2)) && mousey<(y+(height/2)) && mousey>(y-(height/2)))
			{
				return true;
			}
			break;
				
			case ALIGN_TOPLEFT:
			if (mousex<(x+width) && mousex>x && mousey<y && mousey>(y+height))
			{
				return true;
			}
			break;
					
			case ALIGN_TOPRIGHT:
			if (mousex<(x-width) && mousex>x && mousey<y && mousey>(y+height))
			{
				return true;
			}
			break;
		}
		return false;
	}
		
	bool TextActor::isClicked()
	{
		return clicked;
	}

	bool TextActor::wasClicked()
	{
		return prevclicked;
	}
	
	void TextActor::setAntiAliasing(bool toggle)
	{
		antialiasing = toggle;
	}

	bool TextActor::getAntiAliasing()
	{
		return antialiasing;
	}
		
	void TextActor::setRotation(double degrees) //sets rotation of Actor in degrees
	{
		Rotation = degrees;
	}
		
	double TextActor::getRotation() //returns current rotation of Actor in degrees
	{
		return Rotation;
	}
		
	void TextActor::setAlpha(float alpha)
	{
		if(alpha<=1)
		{
			Alpha = alpha;
		}
	}
		
	float TextActor::getAlpha()
	{
		return Alpha;
	}

	void TextActor::setScale(float scale)
	{
		Scale = scale;
		updateSize();
	}

	float TextActor::getScale()
	{
		return Scale;
	}
}