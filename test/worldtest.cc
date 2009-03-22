 /*
   $Id: worldtest.cc,v 1.34 2009/03/22 13:53:20 ksterker Exp $

   Copyright (C) 2003/2004 Alexandre Courbot <alexandrecourbot@linuxgames.com>
   Copyright (C) 2007/2008 Kai Sterker <kaisterker@linuxgames.com>
   Part of the Adonthell Project http://adonthell.linuxgames.com

   Adonthell is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   Adonthell is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Adonthell; if not, write to the Free Software 
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

// #define DEBUG_COLLISION 1

#include "base/base.h"
#include "event/date.h"
#include "gfx/sprite.h"
#include "gfx/screen.h"
#include "input/manager.h"
#include "main/adonthell.h"
#include "python/python.h"
#include "world/area.h"
#include "world/character.h"
#include "world/object.h"
#include "world/mapview.h"

class game_client
{
public:
    world::area world;
    world::character * mchar;
    bool letsexit;
    bool draw_grid;
	bool draw_walkable;
	bool draw_bounding_box;
    bool draw_delay;
    bool print_queue;
	bool screenshot;
	bool always_run;
	
    game_client()
    {
        mchar = NULL;
        letsexit = false;
        draw_grid = false;
        draw_walkable = false;
        draw_bounding_box = false;
        draw_delay = false;
        print_queue = false;
        screenshot = false;
        always_run = false;
    }

	// callback for control event listener
    bool control_func(input::control_event * cev)
    {
        if (cev->type() == input::control_event::BUTTON_PUSHED)
        {
        	switch (cev->button())
        	{       		
	            case input::control_event::A_BUTTON:
	            {
	            	if (!always_run) mchar->run ();
	            	else mchar->walk ();
	                break;
	            }            
	            case input::control_event::B_BUTTON:
	            {
	            	mchar->jump();
	                break;
	            }            
	            case input::control_event::LEFT_BUTTON:
	            {
	                mchar->add_direction(mchar->WEST);
	                break;
	            }
	            case input::control_event::RIGHT_BUTTON:
	            {
	                mchar->add_direction(mchar->EAST);
	                break;
	            }
	            case input::control_event::UP_BUTTON:
	            {
	                mchar->add_direction(mchar->NORTH);
	                break;
	            }
	            case input::control_event::DOWN_BUTTON:
	            {
	                mchar->add_direction(mchar->SOUTH);
	                break;
	            }
	            default: break;   
        	}
        }
        else
        {
        	switch (cev->button())
        	{       		
	            case input::control_event::A_BUTTON:
	            {
	            	if (!always_run) mchar->walk();
	            	else mchar->run ();
	                break;
	            }            
	            case input::control_event::LEFT_BUTTON:
	            {
	                mchar->remove_direction(mchar->WEST);
	                break;
	            }
	            case input::control_event::RIGHT_BUTTON:
	            {
	                mchar->remove_direction(mchar->EAST);
	                break;
	            }
	            case input::control_event::UP_BUTTON:
	            {
	                mchar->remove_direction(mchar->NORTH);
	                break;
	            }
	            case input::control_event::DOWN_BUTTON:
	            {
	                mchar->remove_direction(mchar->SOUTH);
	                break;
	            }
	            default: break;   
        	}
        }

        return true;
    }
    
    // callback for key event listener
    bool callback_func(input::keyboard_event * kev)
    {
        if (kev->type() == input::keyboard_event::KEY_PUSHED)
        {
        	if (kev->key() == input::keyboard_event::CAPSLOCK_KEY)
        	{
        		always_run = true;
				mchar->run ();
        	}
        	// quit
            if (kev->key() == input::keyboard_event::ESCAPE_KEY)
            {
                letsexit = true;
            }
            // toggle grid on|off
            if (kev->key() == input::keyboard_event::G_KEY)
            {
                draw_grid = !draw_grid;
            }
            // toggle object borders on|off
            if (kev->key() == input::keyboard_event::B_KEY)
            {
                draw_bounding_box = !draw_bounding_box;
            }
            // toggle collision area on|off
            if (kev->key() == input::keyboard_event::W_KEY)
            {
                draw_walkable = !draw_walkable;
            }
            // toggle collision area on|off
            if (kev->key() == input::keyboard_event::D_KEY)
            {
                draw_delay = true;
            }
            // save snapshot
            if (kev->key() == input::keyboard_event::S_KEY)
            {
            	screenshot = true;
            }
            // print queue
            if (kev->key() == input::keyboard_event::Q_KEY)
            {
            	print_queue = true;
            }
        }
        else
        {
        	if (kev->key() == input::keyboard_event::CAPSLOCK_KEY)
        	{
        		always_run = false;
        		mchar->walk ();
        	}
        }

        return true;
    }
};

class world_test : public adonthell::app
{
public:
	int main () 
	{
        // Initialize the gfx and input systems
    	init_modules (GFX | INPUT | PYTHON);
    
    	// Set video mode
    	gfx::screen::set_video_mode(640, 480);
        
		// Contains map and player controls
	    game_client gc;

		// Add keyboard controls
	    input::listener il;
    	input::manager::add(&il);

		// Listen for control events
    	il.connect_control_function (base::make_functor_ret(gc, &game_client::control_func));

		// Listen for keyboard events
    	il.connect_keyboard_function (base::make_functor_ret(gc, &game_client::callback_func));

		// Create game world
        gc.world.load ("data/test-world.xml");
        
        // set position and speed of player character 
        gc.mchar = (world::character *) (gc.world.get_entity ("Player"));
        gc.mchar->set_speed (1.5);
        gc.mchar->set_position (398, 322);
        gc.mchar->set_z (0);
                
        // we need to update the python search path to find our map view schedule 
        python::add_search_path (base::Paths.user_data_dir() + "/data/");
        
        // we need to load the world module before we can pass the character object to python
        if (python::import_module ("adonthell.world") == NULL) return 1;        

        // arguments to map view schedule
        PyObject *args = PyTuple_New (1);
        PyTuple_SetItem (args, 0, python::pass_instance ("Player"));
        
        // The renderer ...
        world::debug_renderer rndr;
        world::mapview mv (320, 240, &rndr);
        mv.set_map (&gc.world);
        mv.set_schedule ("focus_on_character", args);
        
	    while (!gc.letsexit) 
    	{
        	u_int16 i;
        
        	// FIXME frames_missed is probably not what we want here 
	        // for (int i = 0; i < base::Timer.frames_missed (); i++) 
	        // {
            input::manager::update();
            events::date::update();
            gc.world.update();
            mv.update();
	        //}
            
            // whether to draw bbox or not
            rndr.set_draw_bbox (gc.draw_bounding_box);
            
            // print queue contents
            if (gc.print_queue)
            {
                rndr.print_queue (true);
                gc.print_queue = false;
            }

            // draw with delay
            if (gc.draw_delay)
            {
                rndr.set_delay (150);
                gc.draw_delay = false;
            }            
            
            // render mapview on screen
            mv.draw (160, 120);

            // stop printing queue contents
            rndr.print_queue (false);
            rndr.set_delay (0);
            
            // whether to render grid
	        if (gc.draw_grid)
	        {
	            for (i = 0; i < gfx::screen::length (); i += 40) 
	                gfx::screen::get_surface()->fillrect (i, 0, 1, gfx::screen::height (), 0xFFFF00); 
	            for (i = 0; i < gfx::screen::height (); i += 40) 
	                gfx::screen::get_surface()->fillrect (0, i, gfx::screen::length (), 1, 0xFFFF00); 
	        }
			
            // take a screenshot
			if (gc.screenshot)
			{
				gfx::surface *screen = gfx::screen::get_surface();
                screen->save_png("screenshot.png");
                gc.screenshot = false;
			}
            
#if DEBUG_COLLISION
            gc.mchar->debug_collision(160 + (320 - 160)/2, 120 + (240 - 240)/2);
            // gc.mchar->add_direction(gc.mchar->NORTH);
#endif
            // rectangle that should be filled with the mapview
            gfx::screen::get_surface()->fillrect (160, 120, 320, 1, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (160, 240+120, 320, 1, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (160, 120, 1, 240, 0xFF8888); 
            gfx::screen::get_surface()->fillrect (320+160, 120, 1, 240, 0xFF8888); 
                        
	        base::Timer.update (); 
	        gfx::screen::update ();
	        gfx::screen::clear (); 
	    }
	    
	    return 0; 
	}
};

world_test myApp;
