/*
* ComputeShader is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
* Copyright (c) 2018 Peter Morley.
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
*  ComputeShader class. Takes in a texture and outputs a texture that is based on the shader
*  implementation.  This class should have more generic member variables.
*/

#pragma once
#include "Shader.h"
class Blur;
class SSAO;

class ComputeShader : public Shader {

	unsigned int _SSAOTextureLocation;
	unsigned int _blurredSSAOTextureLocation;
public:
	ComputeShader();
	~ComputeShader();
	void runShader(Blur* blur, SSAO* ssao);
};