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
#include <SFML/Window.hpp>
#include <string>
#include <math.h>
#include "graphics.h"
#include "util.h"
#include "game.h"
#include "fluff.h"

using namespace v8;
using namespace std;


// Graphics --------------------------------------------------------------------
// -----------------------------------------------------------------------------
const float PI = 3.14159;

Handle<Value> GraphicsSetMode(const Arguments& args) {
    bool created = false;
    
    // Width, Height, Fullscreen, VSync, FSAA
    if (args.Length() == 5 && args[0]->IsNumber() && args[1]->IsNumber()
        && args[2]->IsBoolean() && args[3]->IsBoolean() && args[4]->IsNumber()) {
        
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), args[2]->IsTrue(),
                   args[3]->IsTrue(), ToInt32(args[4]));
        
        created = true;
    
    // Width, Height, Fullscreen, VSync, FSAA=4
    } else if (args.Length() == 4 && args[0]->IsNumber() && args[1]->IsNumber()
        && args[2]->IsBoolean() && args[3]->IsBoolean()) {
        
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), args[2]->IsTrue(),
                   args[3]->IsTrue(), 4);
        
        created = true;
    
    // Width, Height, Fullscreen, VSync=true, FSAA=4
    } else if (args.Length() == 3 && args[0]->IsNumber() && args[1]->IsNumber()
        && args[2]->IsBoolean()) {
        
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), ToInt32(args[2]), true, 4);
        created = true;
    
    // Width, Height, Fullscreen=false, VSync=true, FSAA=4
    } else if (args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber()) {
        GameCreate(ToInt32(args[0]), ToInt32(args[1]), false, true, 4);
        created = true;
    }    
    return Boolean::New(created);
}

//Handle<Value> GraphicsSetSize(const Arguments& args) {
//    if (args.Length() == 2 && args[0]->IsNumber() && args[1]->IsNumber()) {
//        GameSetSize(ToInt32(args[0]), ToInt32(args[1]));
//    }
//    return Undefined();
//}

Handle<Value> GraphicsSetFPS(const Arguments& args) {
    if (args.Length() == 1) {
        gameFPS = ToInt32(args[0]);
        gameWindow.SetFramerateLimit(gameFPS);
    }
    return Undefined();
}

Handle<Value> GraphicsGetFPS(const Arguments& args) {
    return Number::New(gameFPS);
}

Handle<Value> GraphicsSetPosition(const Arguments& args) {
    if (args.Length() == 2) {
         gameWindow.SetPosition(ToInt32(args[0]), ToInt32(args[1]));
    }
    return Undefined();
}

Handle<Value> GraphicsSetMouse(const Arguments& args) {
    if (args.Length() == 1) {
        gameCursor = args[0]->IsTrue();
        gameWindow.ShowMouseCursor(gameCursor);
    }
    return Undefined();
}

Handle<Value> GraphicsGetMouse(const Arguments& args) {
    return Boolean::New(gameCursor);
}

Handle<Value> GraphicsGetWidth(const Arguments& args) {
    return Number::New(gameWindow.GetWidth());
}

Handle<Value> GraphicsGetHeight(const Arguments& args) {
    return Number::New(gameWindow.GetHeight());
}

Handle<Value> GraphicsGetScreenWidth(const Arguments& args) {
    sf::VideoMode dmode = sf::VideoMode::GetDesktopMode();
    return Number::New(dmode.Width);
}

Handle<Value> GraphicsGetScreenHeight(const Arguments& args) {
    sf::VideoMode dmode = sf::VideoMode::GetDesktopMode();
    return Number::New(dmode.Height);
}


// Settings --------------------------------------------------------------------
Handle<Value> GraphicsSetBackgroundColor(const Arguments& args) {
    gameBackColorR = ToFloat(args[0]) / 255.f;
    gameBackColorG = ToFloat(args[1]) / 255.f;
    gameBackColorB = ToFloat(args[2]) / 255.f;
    glClearColor(gameBackColorR, gameBackColorG, gameBackColorB, gameBackColorA);
    return Undefined();
}

Handle<Value> GraphicsSetBackgroundAlpha(const Arguments& args) {
    gameColorA = ToFloat(args[0]);
    glColor4f(gameBackColorR, gameBackColorG, gameBackColorB, gameBackColorA);
    return Undefined();
}

Handle<Value> GraphicsClear(const Arguments& args) {
    glClear(GL_COLOR_BUFFER_BIT);
    return Undefined();
}

Handle<Value> GraphicsSetBlendMode(const Arguments& args) {
    String::Utf8Value mode(args[0]->ToString());
    string cmode = *mode;
    
    if (cmode.compare("lighter") == 0) {
        gameBlendMode = 1;
    
    } else if (cmode.compare("normal") == 0) {
        gameBlendMode = 0;
    }
    GameSetBlendMode();
    return Undefined();
}

Handle<Value> GraphicsSetLineWidth(const Arguments& args) {
    glLineWidth(ToFloat(args[0]));
    return Undefined();
}

Handle<Value> GraphicsSetColor(const Arguments& args) {
    gameColorR = ToFloat(args[0]) / 255.f;
    gameColorG = ToFloat(args[1]) / 255.f;
    gameColorB = ToFloat(args[2]) / 255.f;
    glColor4f(gameColorR, gameColorG, gameColorB, gameColorA);
    return Undefined();
}

Handle<Value> GraphicsSetAlpha(const Arguments& args) {
    gameColorA = ToFloat(args[0]);
    glColor4f(gameColorR, gameColorG, gameColorB, gameColorA);
    return Undefined();
}


// Shapes ----------------------------------------------------------------------
Handle<Value> GraphicsLine(const Arguments& args) {
    glBegin(GL_LINES);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsTriangle(const Arguments& args) {
    glBegin(GL_LINE_LOOP);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glVertex2f(ToFloat(args[4]), ToFloat(args[5]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsTriangleFilled(const Arguments& args) {
    glBegin(GL_TRIANGLES);
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glVertex2f(ToFloat(args[2]), ToFloat(args[3]));
    glVertex2f(ToFloat(args[4]), ToFloat(args[5]));
    glVertex2f(ToFloat(args[0]), ToFloat(args[1]));
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsCircle(const Arguments& args) {
    glBegin(GL_LINE_LOOP);
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float r = ToFloat(args[2]);
    
    const float pi = PI / (1.5f * r);
    for(int i = 0; i <= 3 * r; i++) {
        const float rad = pi * (float)i;
        glVertex2f(x + sin(rad) * r, y + cos(rad) * r);
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsCircleFilled(const Arguments& args) {
    glBegin(GL_TRIANGLE_FAN);
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float r = ToFloat(args[2]);
    
    glVertex2f(x, y) ;
    const float pi = PI / (1.5f * r);
    for(int i = 0; i <= 3 * r; i++) {
        const float rad = pi * (float)i;
        glVertex2f(x + sin(rad) * r, y + cos(rad) * r);
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsRectangle(const Arguments& args) {
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float w = ToFloat(args[2]);
    const float h = ToFloat(args[3]);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsRectangleFilled(const Arguments& args) {
    const float x = ToFloat(args[0]);
    const float y = ToFloat(args[1]);
    const float w = ToFloat(args[2]);
    const float h = ToFloat(args[3]);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsPolygon(const Arguments& args) {
    HandleScope scope;
    Handle<Array> points = Handle<Array>::Cast(args[0]);
    glBegin(GL_LINE_LOOP);
    for(unsigned int i = 0; i < points->Length(); i += 2) {
        glVertex2f(ToFloat(points->Get(Number::New(i))),
                   ToFloat(points->Get(Number::New(i + 1))));
    }
    glEnd();
    return Undefined();
}

Handle<Value> GraphicsPolygonFilled(const Arguments& args) {
    HandleScope scope;
    Handle<Array> points = Handle<Array>::Cast(args[0]);
    glBegin(GL_POLYGON);
    for(unsigned int i = 0; i < points->Length(); i += 2) {
        glVertex2f(ToFloat(points->Get(Number::New(i))),
                   ToFloat(points->Get(Number::New(i + 1))));
    }
    glEnd();
    return Undefined();
}


// Text ------------------------------------------------------------------------
Handle<Value> GraphicsSetFont(const Arguments& args) {
    if (args.Length() == 2 && args[0]->IsString() && args[1]->IsNumber()){
        String::Utf8Value name(args[0]->ToString());
        string fontName(*name);
        return Boolean::New(GameLoadFont(fontName, ToInt32(args[1])));
    
    } else {
        return Boolean::New(false);
    }
}

Handle<Value> GraphicsFontRotate(const Arguments& args) {
    gameFontRotation = ToFloat(args[0]) * 180 / PI;
    return Undefined();
}

Handle<Value> GraphicsFontScale(const Arguments& args) {
    gameFontScaleX = ToFloat(args[0]);
    gameFontScaleY = ToFloat(args[1]);
    return Undefined();
}

Handle<Value> GraphicsDrawText(const Arguments& args) {
    String::Utf8Value text(args[0]->ToString());
    return Boolean::New(GameDrawText(*text, gameFont, ToInt32(args[1]), ToInt32(args[2]), ToInt32(args[3])));
}


// Others ----------------------------------------------------------------------
Handle<Value> GraphicsRotate(const Arguments& args) {
    glRotatef(ToFloat(args[0]), 0.0, 0.0, 1.0);
    return Undefined();
}

Handle<Value> GraphicsTranslate(const Arguments& args) {
    glTranslatef(ToFloat(args[0]), ToFloat(args[1]), 0);
    return Undefined();
}

Handle<Value> GraphicsScale(const Arguments& args) {
    glScalef(ToFloat(args[0]), ToFloat(args[1]), 1);
    return Undefined();
}

Handle<Value> GraphicsPush(const Arguments& args) {
    glPushMatrix();
    gameStacks++;
    return Undefined();
}

Handle<Value> GraphicsPop(const Arguments& args) {
    if (gameStacks > 0) {
        glPopMatrix();
        gameStacks--;
    }
    return Undefined();
}

Handle<Value> GraphicsReset(const Arguments& args) {
    glLoadIdentity();
    return Undefined();
}

