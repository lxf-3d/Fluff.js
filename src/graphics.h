/*
  
  Fluff.js
  Copyright (c) 2010 Ivo Wetzel.
  
  All rights reserved.
  
  Fluff.js is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Fluff.js is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License along with
  Fluff.js. If not, see <http://www.gnu.org/licenses/>.
  
*/

#include <v8.h>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "fluff.h"

using namespace v8;
using namespace std;


// Graphics --------------------------------------------------------------------
// -----------------------------------------------------------------------------
int ToInt32(Handle<Value> i);
const char* ToUTF(Handle<Value> s);

Handle<Value> GraphicsSetMode(const Arguments& args);
Handle<Value> GraphicsSetFPS(const Arguments& args);

