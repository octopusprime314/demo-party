/*
* SSAO is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
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
*  SSAO class. Screen space ambient occlusion technique using g buffers and compute shader
*  for blurring and smoothing the ssao pass.
*/

#pragma once
#include "Vector4.h"
#include <vector>
#include "SSAOShader.h"
#include "Blur.h"
class MRTFrameBuffer;
class ViewManager;

class SSAO {

	void				 _generateKernelNoise();
	std::vector<Vector4> _ssaoKernel;
	std::vector<Vector4> _ssaoNoise;
	unsigned int		 _noiseTexture;
	unsigned int		 _ssaoFBO;
	unsigned int		 _ssaoColorBuffer;
    SSAOShader		     _ssaoShader;
    Blur                 _blur;
public:
	SSAO();
	~SSAO();
	void computeSSAO(MRTFrameBuffer* mrtBuffer, ViewManager* viewManager);
	unsigned int getNoiseTexture();
    unsigned int getSSAOTexture();
	std::vector<Vector4>& getKernel();
    Blur* getBlur();
};