#include "CameraBase.h"
#include "Renderer.h"
#include "Manager/SceneManager.h"

CameraBase::CameraBase() {
    mViewport = Viewport::New();
    mRenderSettings = q_new<RenderSettings>();

    invalidateFrustum();
}

CameraBase::~CameraBase() {
    gRenderer().notifyCameraRemoved(this);
    gSceneManager().notifyCameraRemoved(this);
}

CameraBase *CameraBase::New() {
    return q_new<CameraBase>();
}

void CameraBase::initialize() {
    if (bInitialized) {
        return;
    }

    bInitialized = true;

    gRenderer().notifyCameraCreated(this);
    gSceneManager().notifyCameraCreated(this);
}

void CameraBase::update(EActorDirtyFlags flags) {
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

Vector3 CameraBase::screenToWorldPoint(const Vector2 &screenPoint, float depth) const {
    return Vector3();
}

Vector2 CameraBase::screenToNdcPoint(const Vector2 &screenPoint) const {
    return Vector2();
}

Vector3 CameraBase::ndcToWorldPoint(const Vector2 &ndcPoint, float depth) const {
    return Vector3();
}

void CameraBase::setTransform(Transform *transform) {
    mTransform = transform;
    bRecalcView = true;
}

void CameraBase::setLayers(const uint64_t &layers) {
    mLayers = layers;
}

void CameraBase::setHorzFov(const Radian &fovy) {
    mHorzFov = fovy;
    invalidateFrustum();
}

void CameraBase::setFarClipDistance(float farDist) {
    mFarDist = farDist;
    invalidateFrustum();
}

void CameraBase::setNearClipDistance(float nearDist) {
    if (nearDist <= 0) {
        LOG(LogRenderer, Error, TEXT("Near must be bigger then 0"));
        return;
    }

    mNearDist = nearDist;
    invalidateFrustum();
}

void CameraBase::setAspectRatio(float ratio) {
    mAspect = ratio;
    invalidateFrustum();
}

void CameraBase::setPriority(int32_t priority) {
    mPriority = priority;
}

void CameraBase::setProjectionType(EProjectionType type) {
    mProjectionType = type;

    invalidateFrustum();
}

void CameraBase::setOrthoWindow(float width, float height) {
    mOrthHeight = height;
    mAspect = width / height;

    invalidateFrustum();
}

void CameraBase::setOrthoWindowHeight(float height) {
    mOrthHeight = height;

    invalidateFrustum();
}

void CameraBase::setOrthoWindowWidth(float width) {
    mOrthHeight = width / mAspect;

    invalidateFrustum();
}

void CameraBase::setDirty(bool dirty) {
    bRecalcFrustum = true;
    bRecalcFrustumPlanes = true;
    bRecalcView = true;
}

void CameraBase::setMain(bool main) {
    bMain = main;
}

const Matrix4 &CameraBase::getProjectionMatrix() const {
    updateFrustum();

    return mProjMatrix;
}

const Matrix4 &CameraBase::getViewMatrix() const {
    updateView();

    return mViewMatrix;
}

void CameraBase::calcProjectionParameters(float &left, float &right, float &bottom, float &top) const {
    if (bFrustumExtentsManuallySet) {
        left = mLeft;
        right = mRight;
        top = mTop;
        bottom = mBottom;
    } if (mProjectionType == EProjectionType::Perspective) {
        Radian thetaX(mHorzFov * 0.5f);
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

void CameraBase::updateFrustum() const {
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

                mProjMatrix = Matrix4::Perspective(mHorzFov, mAspect, mNearDist, farDist);
            } else if (mProjectionType == EProjectionType::Orthograhic) {
                float farDist = 0;
                if (mFarDist == 0) {
                    farDist = InfiniteFarPlaneAdjust - 1;
                } else {
                    farDist = mFarDist;
                }

                mProjMatrix = Matrix4::Orthographic(left, right, bottom, top, mNearDist, farDist);
            }
        }
    }
}

void CameraBase::updateView() const {
    if (!bCustomViewMatrix && bRecalcView) {
        mViewMatrix = mTransform->getWorldMatrix();
        bRecalcView = false;
    }
}

bool CameraBase::isFrustumOutOfDate() const {
    return bRecalcFrustum;
}

void CameraBase::invalidateFrustum() const {
    bRecalcFrustum = true;
    bRecalcFrustumPlanes = true;
}
