#ifndef GUI_OPTION_H
#define GUI_OPTION_H

#include "button.h"

namespace gui
{
	class option: public button
	{
	private:
		bool state;
	public:
		option(const char* upimg, const char* downimg, clickevent c, void* a=NULL, int o=5):button(upimg, downimg, c,a,o),state(false) {}


		virtual bool keyup(input::keyboard_event& k);
		virtual bool keydown(input::keyboard_event&k);
//waiting for mouse support
/*
		virtual bool mouseup(SDL_MouseButtonEvent & m);
		virtual bool mousedown(SDL_MouseButtonEvent & m);
// */
		virtual void unfocus(){}

		virtual void activate();
		void setstate(bool s) {clicked = s;}
	};
};


#endif//GUI_LABEL_H
