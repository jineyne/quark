#include "SceneInfo.h"
#include "Renderer.h"
#include "CameraBase.h"

SceneInfo::SceneInfo() {
    GpuBufferDesc desc{};
    desc.type = EBufferType::Structured;
    desc.format = EGpuBufferFormat::Unknown;
    desc.usage = EBufferUsage::LoadStore;
    desc.elementSize = sizeof(LightData);
    desc.elementCount = STANDARD_FORWARD_MAX_NUM_LIGHTS;

    mData.gLightsBuffer = GpuBuffer::New(desc);
}

SceneInfo::~SceneInfo() {
    q_delete(mData.gLightsBuffer);

    for (auto &entry : mData.renderables) {
        for (auto i = 0; i < entry->elements.length(); i++) {
            q_delete(entry->elements[i].params);
        }

        q_delete(entry);
    }

    for (auto &entry : mData.views) {
        q_delete(entry);
    }
}

void SceneInfo::registerCamera(CameraBase *camera) {
    auto desc = createViewInfoDesc(camera);

    auto view = q_new<ViewInfo>(desc);
    uint32_t viewIdx = static_cast<uint32_t>(mData.views.length());
    mData.views.add(view);
    mData.cameraToView[camera] = viewIdx;
    camera->setRendererId(viewIdx);

    updateCameraRenderTargets(camera);
}

void SceneInfo::updateCamera(CameraBase *camera, uint32_t updateFlags) {
    auto cameraId = camera->getRendererId();
    auto view = mData.views[cameraId];

    if ((updateFlags & static_cast<uint32_t>(ECameraDirtyFlags::Redraw)) != 0) {
        // view->notifyNeedsRedraw();
    }

    uint32_t updateEverythingFlag = (uint32_t) EActorDirtyFlags::Everything | (uint32_t) EActorDirtyFlags::Active | (uint32_t) ECameraDirtyFlags::Viewport;

    if((updateFlags & updateEverythingFlag) != 0) {
        ViewInfoDesc viewDesc = createViewInfoDesc(camera);

        view->setView(viewDesc);
        // view->set(camera->getRenderSettings());

        updateCameraRenderTargets(camera);
        return;
    }

    Transform *transform = camera->getTransform();
    view->setTransform(transform->getPosition(), transform->getRotation().rotate(Vector3(0, 0, -1)), camera->getViewMatrix(), camera->getProjectionMatrix());
}

void SceneInfo::unregisterCamera(CameraBase *camera) {
    auto cameraId = camera->getRendererId();

    auto lastCamera = mData.views.top()->getSceneCamera();
    auto lastCameraId = lastCamera->getRendererId();

    if (cameraId != lastCameraId) {
        std::swap(mData.views[cameraId], mData.views[lastCameraId]);
        lastCamera->setRendererId(cameraId);

        mData.cameraToView[lastCamera] = cameraId;
    }

    auto view = mData.views[mData.views.length() - 1];
    mData.views.remove(view);
    q_delete(view);

    mData.cameraToView.remove(camera);

    updateCameraRenderTargets(camera, true);
}

void SceneInfo::registerRenderable(Renderable *renderable) {
    const auto renderableId = static_cast<uint32_t>(mData.renderables.length());
    renderable->setRendererId(renderableId);

    mData.renderables.add(q_new<RenderableInfo>());
    auto *info = mData.renderables.top();
    info->renderable = renderable;
    info->worldMatrix = renderable->getMatrix();
    info->prevWorldMatrix = info->worldMatrix;
    info->prevFrameDirtyState = EPrevFrameDirtyState::Clean;
    info->updatePerObjectBuffer();

    const auto &mesh = renderable->getMesh();
    if (mesh != nullptr) {
        auto *vertexDecl = mesh->getVertexData();

        info->elements.add(RenderableElement());
        auto &element = info->elements.top();

        element.type = static_cast<uint32_t>(ERenderElementType::Renderable);
        element.mesh = mesh.get();
        element.material = renderable->getMaterial();

        if (element.material != nullptr && element.material->getShader() == nullptr) {
            element.material = nullptr;
        }

        if (element.material == nullptr) {
            // element.material = Material::New(DefaultMaterial::Get()->getShader());
            EXCEPT(FLogRenderer, InternalErrorException, TEXT("Material is null"));
        }

        const auto &shader = element.material->getShader();
        element.params = element.material->createParamsSet(element.defaultTechniqueIdx);
        element.material->updateParamsSet(element.params, 0.0f, true);
    }

    for (auto &element : info->elements) {
        auto *shader = element.material->getShader();
        if (shader == nullptr) {
            LOG(LogRenderer, Warning, TEXT("Material Shader not set!"))
            continue;
        }

        auto *gpuParams = element.params->getGpuParams();
        gpuParams->setParamBlockBuffer(TEXT("PerObject"), info->perObjectParamBuffer);
        gpuParams->setParamBlockBuffer(TEXT("PerCall"), info->perCallParamBuffer);

        gpuParams->getParamInfo()->getBindings(FGpuPipelineParamInfoBase::ParamType::ParamBlock,
                                               TEXT("PerCamera"), element.perCameraBindings);
    }
}

void SceneInfo::updateRenderable(Renderable *renderable, uint32_t updateFlags) {
    const uint32_t renderableId = renderable->getRendererId();

    auto *info = mData.renderables[renderableId];

    if (info->prevFrameDirtyState != EPrevFrameDirtyState::Updated) {
        info->prevWorldMatrix = info->worldMatrix;
    }

    info->worldMatrix = renderable->getMatrix();
    info->prevFrameDirtyState = EPrevFrameDirtyState::Updated;

    mData.renderables[renderableId]->updatePerObjectBuffer();
}

void SceneInfo::unregisterRenderable(Renderable *renderable) {
    const auto renderableId = renderable->getRendererId();
    auto *lastRenderable = mData.renderables.top();
    auto lastRenderableId = lastRenderable->renderable->getRendererId();

    auto current = mData.renderables[renderableId];

    for (auto &element : current->elements) {
        q_delete(element.params);
    }

    if (renderableId != lastRenderableId) {
        std::swap(mData.renderables[renderableId], mData.renderables[lastRenderableId]);
        lastRenderable->renderable->setRendererId(renderableId);
    }

    mData.renderables.remove(mData.renderables.top());
    q_delete(current);
}

void SceneInfo::registerLight(LightBase *light) {
    const auto lightId = static_cast<uint32_t>(mData.lights.length());
    light->setRendererId(lightId);

    mData.lights.add(q_new<RenderableLight>(light));
    // auto &element = mData.lights.top();
}

void SceneInfo::updateLight(LightBase *light, uint32_t updateFlags) {
    auto lightId = light->getRendererId();
    auto current = mData.lights[lightId];
}

void SceneInfo::unregisterLight(LightBase *light) {
    auto lightId = light->getRendererId();
    auto current = mData.lights[lightId];

    auto last = mData.lights.top();
    auto lastLight = last->mInternal;
    auto lastLightId = lastLight->getRendererId();

    if (lightId != lastLightId) {
        std::swap(mData.views[lightId], mData.views[lastLightId]);
        lastLight->setRendererId(lightId);
    }

    mData.lights.remove(mData.lights.top());
    q_delete(current);
}

ViewInfoDesc SceneInfo::createViewInfoDesc(CameraBase *camera) const {
    auto *viewport = camera->getViewport();
    auto clearFlags = viewport->getClearFlags();
    ViewInfoDesc desc{};

    desc.target.clearFlags = 0;
    if ((clearFlags & EClearFlags::Color) == EClearFlags::Color) {
        desc.target.clearFlags |= static_cast<uint32_t>(EFrameBufferType::Color);
    }

    if ((clearFlags & EClearFlags::Depth) == EClearFlags::Depth) {
        desc.target.clearFlags |= static_cast<uint32_t>(EFrameBufferType::Depth);
    }

    if ((clearFlags & EClearFlags::Stencil) == EClearFlags::Stencil) {
        desc.target.clearFlags |= static_cast<uint32_t>(EFrameBufferType::Stencil);
    }

    desc.target.clearColor = viewport->getClearColorValue();

    desc.target.target = viewport->getTarget();
    desc.target.viewRect = viewport->getPixelArea();

    if (desc.target.target != nullptr) {
        desc.target.targetWidth = desc.target.target->getWidth();
        desc.target.targetHeight = desc.target.target->getHeight();
    } else {
        desc.target.targetWidth = 0;
        desc.target.targetHeight = 0;
    }

    desc.camera = camera;

    Transform *transform = camera->getTransform();
    if (transform == nullptr) {
        desc.viewTransform = Matrix4(1.0f);
    } else {
        desc.viewTransform = camera->getViewMatrix(); // Matrix4::Transform(transform->getPosition(), transform->getRotation(), transform->getScale());
    }

    desc.projTransform = camera->getProjectionMatrix();
    desc.projType = camera->getProjectionType();

    return desc;
}

void SceneInfo::updateCameraRenderTargets(CameraBase *camera, bool remove) {
    auto *renderTarget = camera->getViewport()->getTarget();

    int rtChanged = 0; // 0 - No RT, 1 - RT found, 2 - RT changed
    for (auto targetIt = mData.renderTargetInfos.begin(); targetIt != mData.renderTargetInfos.end(); ++targetIt) {
        auto target = *targetIt;
        for (auto camIt = target.cameras.begin(); camIt != target.cameras.end(); ++camIt) {
            if (camera == *camIt) {
                if (remove) {
                    target.cameras.remove(*camIt);
                    rtChanged = 1;
                } else {
                    if (renderTarget != target.target) {
                        target.cameras.remove(*camIt);
                        rtChanged = 2;
                    } else {
                        rtChanged = 1;
                    }
                }

                break;
            }
        }

        RenderTargetInfo t = *targetIt;

        if (target.cameras.empty()) {
            mData.renderTargetInfos.removeIt(targetIt);
            break;
        }
    }

    if (renderTarget != nullptr && !remove && (rtChanged == 0 || rtChanged == 2)) {
        auto it = std::find_if(mData.renderTargetInfos.begin(), mData.renderTargetInfos.end(),
                               [&](const RenderTargetInfo &x) {
                                   return x.target == renderTarget;
                               });

        if (it != mData.renderTargetInfos.end()) {
            it->cameras.add(camera);
        } else {
            mData.renderTargetInfos.add(RenderTargetInfo());
            auto &info = mData.renderTargetInfos.top();

            info.target = renderTarget;
            info.cameras.add(camera);
        }

        auto cameraComparer = [&](const CameraBase *a, const CameraBase *b) {
            return a->getPriority() > b->getPriority();
        };

        auto renderTargetInfoComparer = [&](const RenderTargetInfo &a, const RenderTargetInfo &b) {
            return a.target->getPriority() > b.target->getPriority();
        };

        std::sort(std::begin(mData.renderTargetInfos), std::end(mData.renderTargetInfos), renderTargetInfoComparer);

        for (auto &info : mData.renderTargetInfos) {
            auto &cameras = info.cameras;
            std::sort(std::begin(cameras), std::end(cameras), cameraComparer);
        }
    }
}
