#pragma once

class Camera; //Cameraクラスへの参照を渡せるようにします

class CameraStrategy
{
public:
    virtual ~CameraStrategy() = default;

    // カメラ挙動が切り替わった瞬間に1度だけ呼ばれる初期化関数
    virtual void Start(Camera* camera) {}

    // 毎フレームのカメラ座標や注視点の計算処理
    virtual void Update(Camera* camera) = 0;
};