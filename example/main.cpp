//
// Created by Shenyao Ke on 2019-02-16.
//

#include <Core/ProgressBar.h>
#include <Core/Sampler.h>
#include "Core/Scene.h"
#include <Camera/ProjectiveCamera.h>
#include "IO/SceneLoader.h"
#include <Integrator/WhittedIntegrator.h>

using namespace Kaguya;

int main(int /*argc*/, char */*argv*/[])
{
    std::unique_ptr<Scene> mScene(SceneLoader::load("../../scene/unitest_scene.json"));
    mScene->commitScene();
    std::cout << "Prim count: " << mScene->getPrimitiveCount() << "\n";

    clock_t startT, endT;
    startT = clock();

    auto view_cam = std::static_pointer_cast<ProjectiveCamera>(mScene->getCamera());
    view_cam->getFilm().setResolution(640, 480);
    //ConsoleProgressBar progBar;
    CameraSample camsmp;
    Transform w2o;

    uint32_t maxDepth = 3;
    Bounds2i pixelRange{Point2i(0, 0), Point2i(view_cam->getFilm().height, view_cam->getFilm().width)};
    Sampler sampler;
    WhittedIntegrator integrator(maxDepth, pixelRange, sampler);
    integrator.render(*mScene);
    //progBar.complete();
    endT = clock();
    std::cout << "Rendering Time:\t" << (Float)(endT - startT) / CLOCKS_PER_SEC << "s" << std::endl;//Timer

    return 0;
}