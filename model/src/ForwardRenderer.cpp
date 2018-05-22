#include "ForwardRenderer.h"
#include "Model.h"

ForwardRenderer::ForwardRenderer() : _forwardShader("forwardShader") {

}

ForwardRenderer::~ForwardRenderer() {

}

void ForwardRenderer::forwardLighting(std::vector<Model*>& modelList, ViewManager* viewManager, ShadowRenderer* shadowRenderer,
    std::vector<Light*>& lights, PointShadowRenderer* pointShadowRenderer) {

    for (auto model : modelList) {
        _forwardShader.runShader(model, viewManager, shadowRenderer, lights, pointShadowRenderer);
    }
}