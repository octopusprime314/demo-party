#include "Shader.h"
#include "Model.h"

Shader::Shader() {

}

Shader::~Shader() {

}

void Shader::runShader(Model* model) {

    //LOAD IN SHADER
    glUseProgram(_shaderContext); //use context for loaded shader

    //LOAD IN VBO BUFFERS 
    VBO* vbo = model->getVBO();
    //Bind vertex buff context to current buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo->getVertexContext());

    //Say that the vertex data is associated with attribute 0 in the context of a shader program
    //Each vertex contains 3 floats per vertex
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Now enable vertex buffer at location 0
    glEnableVertexAttribArray(0);

    //Bind normal buff context to current buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo->getNormalContext());

    //Say that the normal data is associated with attribute 1 in the context of a shader program
    //Each normal contains 3 floats per normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //Now enable normal buffer at location 1
    glEnableVertexAttribArray(1);

    //Bind texture coordinate buff context to current buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo->getTextureContext());

    //Say that the texture coordinate data is associated with attribute 2 in the context of a shader program
    //Each texture coordinate contains 2 floats per texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    //Now enable texture buffer at location 2
    glEnableVertexAttribArray(2);

    MVP* mvp = model->getMVP();
    //glUniform mat4 combined model and world matrix, GL_TRUE is telling GL we are passing in the matrix as row major
    glUniformMatrix4fv(_modelLocation, 1, GL_TRUE, mvp->getModelBuffer());

    //glUniform mat4 view matrix, GL_TRUE is telling GL we are passing in the matrix as row major
    glUniformMatrix4fv(_viewLocation, 1, GL_TRUE, mvp->getViewBuffer());

    //glUniform mat4 projection matrix, GL_TRUE is telling GL we are passing in the matrix as row major
    glUniformMatrix4fv(_projectionLocation, 1, GL_TRUE, mvp->getProjectionBuffer());

    //glUniform mat4 normal matrix, GL_TRUE is telling GL we are passing in the matrix as row major
    glUniformMatrix4fv(_normalLocation, 1, GL_TRUE, mvp->getNormalBuffer());

    auto textureStrides = model->getTextureStrides();
    unsigned int strideLocation = 0;
    for(auto textureStride : textureStrides) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, model->getTexture(textureStride.first)->getContext()); //grab first texture of model and return context
        //glUniform texture 
        //The second parameter has to be equal to GL_TEXTURE(X) so X must be 0 because we activated texture GL_TEXTURE0 two calls before
        glUniform1iARB(_textureLocation, 0); 

        //Draw triangles using the bound buffer vertices at starting index 0 and number of triangles
        glDrawArraysEXT(GL_TRIANGLES, strideLocation, (GLsizei)textureStride.second);
        strideLocation += textureStride.second;
    }

    glDisableVertexAttribArray(0); //Disable vertex attribute
    glDisableVertexAttribArray(1); //Disable normal attribute
    glDisableVertexAttribArray(2); //Disable texture attribute
    glBindBuffer(GL_ARRAY_BUFFER, 0); //Unbind buffer
    glUseProgram(0);//end using this shader
}

void Shader::build(std::string shaderName) {
    GLhandleARB vertexShaderHandle;
    GLhandleARB fragmentShaderHandle;

    std::string fileNameVert = shaderName;
    fileNameVert.append(".vert");
    std::string fileNameFrag = shaderName;
    fileNameFrag.append(".frag");

    //Compile each shader
    vertexShaderHandle = _compile((char*)fileNameVert.c_str(), GL_VERTEX_SHADER);
    fragmentShaderHandle = _compile((char*)fileNameFrag.c_str(), GL_FRAGMENT_SHADER);

    //Link the two compiled binaries
    _link(vertexShaderHandle, fragmentShaderHandle);

}

void Shader::_link(GLhandleARB vertexShaderHandle, GLhandleARB fragmentShaderHandle) {
    _shaderContext = glCreateProgramObjectARB();

    glAttachObjectARB(_shaderContext, vertexShaderHandle);
    glAttachObjectARB(_shaderContext, fragmentShaderHandle);

    glLinkProgramARB(_shaderContext);

    GLint      successfully_linked = 0;
    glGetProgramiv(_shaderContext, GL_LINK_STATUS, &successfully_linked);

    // Exit if the program couldn't be linked correctly
    if (!successfully_linked) {
        GLint errorLoglength;
        GLint actualErrorLogLength;
        //Attempt to get the length of our error log.
        glGetProgramiv(_shaderContext, GL_INFO_LOG_LENGTH, &errorLoglength);

        std::cout << errorLoglength << std::endl;

        //Create a buffer to read compilation error message
        char* errorLogText = (char*)malloc(sizeof(char) * errorLoglength);

        //Used to get the final length of the log.
        glGetProgramInfoLog(_shaderContext, errorLoglength, &actualErrorLogLength, errorLogText);

        std::cout << actualErrorLogLength << std::endl;

        // Display errors.
        std::cout << errorLogText << std::endl;

        // Free the buffer malloced earlier
        free(errorLogText);

        std::cout << "Program was not linked correctly!" << std::endl;
    }
    else { //Program successful grab locations in shader for uniforms and attributes

        //glUniform mat4 combined model and world matrix
        _modelLocation = glGetUniformLocation(_shaderContext, "model");

        //glUniform mat4 view matrix
        _viewLocation = glGetUniformLocation(_shaderContext, "view");

        //glUniform mat4 projection matrix
        _projectionLocation = glGetUniformLocation(_shaderContext, "projection");

        //glUniform mat4 normal matrix
        _normalLocation = glGetUniformLocation(_shaderContext, "normal");

        //glUniform texture map sampler location
        _textureLocation = glGetUniformLocation(_shaderContext, "textureMap");
    }
}

// Loading shader function
GLhandleARB Shader::_compile(char* filename, unsigned int type)
{
    FILE *pfile;
    GLhandleARB handle;
    const GLcharARB* files[1];

    // shader Compilation variable
    GLint result;				// Compilation code result
    GLint errorLoglength;
    char* errorLogText;
    GLsizei actualErrorLogLength;

    char buffer[400000];
    memset(buffer, 0, 400000);

    errno_t err = fopen_s(&pfile, filename, "rb");
    if (err != 0)
    {
        printf("Sorry, can't open file: '%s'.\n", filename);
        return 0;
    }

    fread(buffer, sizeof(char), 400000, pfile);

    fclose(pfile);

    handle = glCreateShaderObjectARB(type);
    if (!handle) {
        //We have failed creating the vertex shader object.
        printf("Failed creating vertex shader object from file: %s.", filename);
        return 0;
    }

    files[0] = (const GLcharARB*)buffer;
    glShaderSourceARB(
        handle, //The handle to our shader
        1, //The number of files.
        files, //An array of const char * data, which represents the source code of theshaders
        nullptr);

    glCompileShaderARB(handle);

    //Compilation checking.
    glGetObjectParameterivARB(handle, GL_OBJECT_COMPILE_STATUS_ARB, &result);

    // If an error was detected.
    if (!result) {
        //We failed to compile.
        printf("Shader '%s' failed compilation.\n", filename);

        //Attempt to get the length of our error log.
        glGetObjectParameterivARB(handle, GL_OBJECT_INFO_LOG_LENGTH_ARB, &errorLoglength);

        //Create a buffer to read compilation error message
        errorLogText = (char*)malloc(sizeof(char) * errorLoglength);

        //Used to get the final length of the log.
        glGetInfoLogARB(handle, errorLoglength, &actualErrorLogLength, errorLogText);

        // Display errors.
        printf("%s\n", errorLogText);

        // Free the buffer malloced earlier
        free(errorLogText);
    }

    return handle;
}