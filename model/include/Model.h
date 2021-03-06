/*
* Model is part of the ReBoot distribution (https://github.com/octopusprime314/ReBoot.git).
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
*  Model class. Contains vertex and normal data which will get populated in opengl
*  buffers to be rendered to the screen
*/

#pragma once
#include "Matrix.h"
#include "Tex2.h"
#include "StateVector.h"
#include <vector>
#include "UpdateInterface.h"
#include "GLIncludes.h"
#include "StaticShader.h"
#include "DebugShader.h"
#include <iostream>
#include "FbxLoader.h"
#include "MasterClock.h"
#include "TextureBroker.h"
#include "Geometry.h"
#include "VAO.h"
#include "MVP.h"
#include "RenderBuffers.h"
#include "ForwardShader.h"

class SimpleContext;

enum class ModelClass {
    ModelType = 0,
    AnimatedModelType
};

enum class GeometryType {
    Triangle = 0,
    Sphere = 1
};
using TextureMetaData = std::vector<std::pair<std::string, int>>;

class Model : public UpdateInterface {
    
public:

    Model(ViewManagerEvents* eventWrapper, RenderBuffers& renderBuffers, StaticShader* pStaticShader);

    Model(ViewManagerEvents* eventWrapper, ModelClass classId = ModelClass::ModelType)
    : UpdateInterface(eventWrapper),
      _classId(classId)
    {}

    //Default model to type to base class
    Model(std::string name, ViewManagerEvents* eventWrapper, ModelClass classId = ModelClass::ModelType);
    virtual ~Model();
    MVP*                        getMVP();
    VAO*                        getVAO();
    RenderBuffers*              getRenderBuffers();
    StateVector*                getStateVector();
    ModelClass                  getClassType();
    size_t                      getArrayCount();
    void                        addTexture(std::string textureName, int stride);
    void                        addLayeredTexture(std::vector<std::string> textureNames, int stride);
    Texture*                    getTexture(std::string textureName);
    LayeredTexture*             getLayeredTexture(std::string textureName);
    TextureMetaData&            getTextureStrides();
    GeometryType                getGeometryType();
    Geometry*                   getGeometry();
    void                        addGeometryTriangle(Triangle triangle);
    void                        addGeometrySphere(Sphere sphere);
    void                        setPosition(Vector4 position);
    void                        setVelocity(Vector4 velocity);
    void                        setInstances(std::vector<Vector4> offsets); //is this model used for instancing
    bool                        getIsInstancedModel();
    float*                      getInstanceOffsets();

protected:
    StateVector                 _state; //Kinematics
    RenderBuffers               _renderBuffers; //Manages vertex, normal and texture data
    VAO                         _vao; //Vao container
    MVP                         _mvp; //Model view matrix container
    StaticShader*               _shaderProgram; //Container object of the Model's shader
    DebugShader*                _debugShaderProgram; //Container object of the normal line shader
    bool                        _debugMode; //Runs an extra shader with debug information include normals
    FbxLoader*                  _fbxLoader; //Used to load fbx data and parse it into engine format
    ModelClass                  _classId; //Used to identify which class is being used
    MasterClock*                _clock; //Used to coordinate time with the world
    static TextureBroker*       _textureManager; //Static texture manager for texture reuse purposes, all models have access
    std::string                 _textureName; //Keeps track of which texture to grab from static texture manager
    TextureMetaData             _textureStrides; //Keeps track of which set of vertices use a certain texture within the large vertex set
    GeometryType                _geometryType; //Indicates whether the collision geometry is sphere or triangle based
    Geometry                    _geometry; //Geometry object that contains all collision information for a model
    bool                        _isInstanced;
    float                       _offsets[900]; //300 x, y and z offsets
    int                         _instances;

    std::string                 _getModelName(std::string name);
    void                        _updateKeyboard(int key, int x, int y); //Do stuff based on keyboard upate
    void                        _updateReleaseKeyboard(int key, int x, int y);
    void                        _updateMouse(double x, double y); //Do stuff based on mouse update
    void                        _updateDraw(); //Do draw stuff
    void                        _updateView(Matrix view); //Get view matrix updates
    void                        _updateProjection(Matrix projection); //Get projection matrix updates
    void                        _updateKinematics(int milliSeconds);
};