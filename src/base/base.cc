/*
   $Id: base.cc,v 1.2 2004/10/18 07:40:22 ksterker Exp $

   Copyright (C) 2004 Kai Sterker <kaisterker@linuxgames.com>
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


/**
 * @file   base/base.cc
 * @author Kai Sterker <kaisterker@linuxgames.com>
 * 
 * @brief  Initialization of the base module.
 */

#include "base/base.h"

// global timer and path objects
namespace base 
{
    base::paths Paths;
    base::timer Timer;
}

// init the base module
bool base::init (const std::string & userdatadir, const std::string & game)
{
    return base::Paths.init (userdatadir, game);
}
