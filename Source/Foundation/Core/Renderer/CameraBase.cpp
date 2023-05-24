#include "CameraBase.h"
#include "Renderer.h"
#include "Manager/SceneManager.h"

FCameraBase::FCameraBase() {
    mViewport = FViewport::New();
    mRenderSettings = q_new<FRenderSettings>();

    invalidateFrustum();
}

FCameraBase::~FCameraBase() {
    gRenderer().notifyCameraRemoved(this);
    gSceneManager().notifyCameraRemoved(this);
}

FCameraBase *FCameraBase::New() {
    return q_new<FCameraBase>();
}

void FCameraBase::initialize() {
    if (bInitialized) {
        return;
    }

    bInitialized = true;

    gRenderer().notifyCameraCreated(this);
    gSceneManager().notifyCameraCreated(this);
}

void FCameraBase::update(EActorDirtyFlags flags) {
    EActorDirtyFlags updateEverythingFlag = EActorDirtyFlags::Everything | EActorDirtyFlags::Active;

    if ((flags & updateEverythingFlag) != EActorDirtyFlags::None) {
        if (bIsActiveOld != bIsActive) {
            if (bIsActive)
                gRenderer().notifyCameraCreated(this);
            else
                gRenderer().notifyCameraRemoved(this);
        } else {
            gRenderer().notifyCameraRemoved(this);
            gRenderer().notifyCameraCreated(this);
        }

        bIsActiveOld = bIsActive;
        bRecalcView = false;
    } else if ((flags & EActorDirtyFlags::Mobility) != EActorDirtyFlags::None) {
        gRenderer().notifyCameraRemoved(this);
        gRenderer().notifyCameraCreated(this);
    } else if ((flags & EActorDirtyFlags::Transform) != EActorDirtyFlags::None) {
        if (bIsActive) {
            gRenderer().notifyCameraUpdated(this);
        }
    }
}

FVector3 FCameraBase::screenToWorldPoint(const FVector2 &screenPoint, float depth) const {
    return FVector3();
}

FVector2 FCameraBase::screenToNdcPoint(const FVector2 &screenPoint) const {
    return FVector2();
}

FVector3 FCameraBase::ndcToWorldPoint(const FVector2 &ndcPoint, float depth) const {
    return FVector3();
}

void FCameraBase::setTransform(FTransform *transform) {
    mTransform = transform;
    bRecalcView = true;
}

void FCameraBase::setLayers(const uint64_t &layers) {
    mLayers = layers;
}

void FCameraBase::setHorzFov(const FRadian &fovy) {
    mHorzFov = fovy;
    invalidateFrustum();
}

void FCameraBase::setFarClipDistance(float farDist) {
    mFarDist = farDist;
    invalidateFrustum();
}

void FCameraBase::setNearClipDistance(float nearDist) {
    if (nearDist <= 0) {
        LOG(FLogRenderer, Error, TEXT("Near must be bigger then 0"));
        return;
    }

    mNearDist = nearDist;
    invalidateFrustum();
}

void FCameraBase::setAspectRatio(float ratio) {
    mAspect = ratio;
    invalidateFrustum();
}

void FCameraBase::setPriority(int32_t priority) {
    mPriority = priority;
}

void FCameraBase::setProjectionType(EProjectionType type) {
    mProjectionType = type;

    invalidateFrustum();
}

void FCameraBase::setOrthoWindow(float width, float height) {
    mOrthHeight = height;
    mAspect = width / height;

    invalidateFrustum();
}

void FCameraBase::setOrthoWindowHeight(float height) {
    mOrthHeight = height;

    invalidateFrustum();
}

void FCameraBase::setOrthoWindowWidth(float width) {
    mOrthHeight = width / mAspect;

    invalidateFrustum();
}

void FCameraBase::setDirty(bool dirty) {
    bRecalcFrustum = true;
    bRecalcFrustumPlanes = true;
    bRecalcView = true;
}

void FCameraBase::setMain(bool main) {
    bMain = main;

}

const FMatrix4 &FCameraBase::getProjectionMatrix() const {
    updateFrustum();

    return mProjMatrix;
}

const FMatrix4 &FCameraBase::getViewMatrix() const {
    updateView();

    return mViewMatrix;
}

void FCameraBase::calcProjectionParameters(float &left, float &right, float &bottom, float &top) const {
    if (bFrustumExtentsManuallySet) {
        left = mLeft;
        right = mRight;
        top = mTop;
        bottom = mBottom;
    } if (mProjectionType == EProjectionType::Perspective) {
        FRadian thetaX(mHorzFov * 0.5f);
        float tanThetaX = std::tan(thetaX);
        float tanThetaY = tanThetaX / mAspect;

        float half_w = tanThetaX * mNearDist;
        float half_h = tanThetaY * mNearDist;

        left = -half_w;
        right = half_w;
        bottom = -half_h;
        top = half_h;

        mLeft = left;
        mRight = right;
        mTop = top;
        mBottom = bottom;
    } else if (mProjectionType == EProjectionType::Orthograhic) {
        float half_w = getOrthoWindowWidth() * 0.5f;
        float half_h = getOrthoWindowHeight() * 0.5f;

        left = -half_w;
        right = half_w;
        bottom = -half_h;
        top = half_h;

        mLeft = left;
        mRight = right;
        mTop = top;
        mBottom = bottom;
    }
}

void FCameraBase::updateFrustum() const {
    if (isFrustumOutOfDate()) {
        float left, right, bottom, top;
        calcProjectionParameters(left, right, bottom, top);

        if (!bCustomProjMatrix) {
            if (mProjectionType == EProjectionType::Perspective) {
                float farDist = 0;
                if (mFarDist == 0) {
                    farDist = InfiniteFarPlaneAdjust - 1;
                } else {
                    farDist = mFarDist;
                }

                mProjMatrix = FMatrix4::Perspective(mHorzFov, mAspect, mNearDist, farDist);
            } else if (mProjectionType == EProjectionType::Orthograhic) {
                float farDist = 0;
                if (mFarDist == 0) {
                    farDist = InfiniteFarPlaneAdjust - 1;
                } else {
                    farDist = mFarDist;
                }

                mProjMatrix = FMatrix4::Orthographic(left, right, bottom, top, mNearDist, farDist);
            }
        }
    }
}

void FCameraBase::updateView() const {
    if (!bCustomViewMatrix && bRecalcView) {
        mViewMatrix = mTransform->getWorldMatrix();
        bRecalcView = false;
    }
}

bool FCameraBase::isFrustumOutOfDate() const {
    return bRecalcFrustum;
}

void FCameraBase::invalidateFrustum() const {
    bRecalcFrustum = true;
    bRecalcFrustumPlanes = true;
}
