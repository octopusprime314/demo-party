/*
* EffectShader is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
* Copyright (c) 2017 Peter Morley.
*
* ReBoot is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* ReBoot is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

/**
*  EffectShader class. Generic effects class 
*/

#pragma once
#include "Shader.h"
#include "Matrix.h"
#include "MVP.h"
class Light;
class Water;
class Effect;
class ViewManager;

class EffectShader : public Shader {

protected:
    GLint       _timeLocation;
    GLint       _lightPosLocation;
    GLint       _modelViewLocation;
    GLint       _projectionLocation;
    GLint       _inverseViewLocation;
    GLint       _fireTypeLocation;
    GLint       _farPlaneLocation;
    GLint       _noiseTextureLocation;
    GLuint      _vaoContext;
    GLuint      _quadBufferContext;
    GLint       _normalLocation;
    GLint       _fireColorLocation;
public:
    EffectShader(std::string shaderName);
    virtual ~EffectShader();
    virtual void runShader(Effect* effectObject, float seconds);
};