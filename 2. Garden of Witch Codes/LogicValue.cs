using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[Serializable]
public class CameraValue {
    [SerializeField]
    public float mLeftMoveRate = 0.3f;
    [SerializeField]
    public float mRightMoveRate = 0.7f;
    [SerializeField]
    public float mUpMoveRate = 0.7f;
    [SerializeField]
    public float mDownMoveRate = 0.3f;
}

[Serializable]
public class WitchValue {
    [SerializeField]
    public float mSpeed = 8.0f;
    [SerializeField]
    public float mLadderSpeed = 5.0f;
    [SerializeField]
    public int mInitStamina = 100;
    [SerializeField]
    public int mInitStaminaToTry = 10;
    [SerializeField]
    public int mInitHP = 5;

    [SerializeField]
    public float mIconMoveShiftX = 0.5f;
    [SerializeField]
    public Sprite mMatIcon = null;
    [SerializeField]
    public Sprite mTalkIcon = null;
    [SerializeField]
    public Sprite mMakeIcon = null;
    [SerializeField]
    public Sprite mRestIcon = null;
}

[Serializable]
public class MiniGameValue {
    [SerializeField]
    public float mMiniGameShiftY = 0.5f;
    [SerializeField]
    public float mMiniGameTime = 10.0f;
    [SerializeField]
    public Color mStartColor = new Color(1.0f, 1.0f, 1.0f);
    [SerializeField]
    public Color mFinishColor = new Color(1.0f, 0.0f, 0.0f);

    [SerializeField]
    public GameObject mCmdGamePrefab = null;
    [SerializeField]
    public float mCmdArrowShiftY = 1.0f;
    [SerializeField]
    public Sprite mCmdLeftArrowSprite = null;
    [SerializeField]
    public Sprite mCmdRightArrowSprite = null;
    [SerializeField]
    public Sprite mCmdUpArrowSprite = null;
    [SerializeField]
    public Sprite mCmdDownArrowSprite = null;

    [SerializeField]
    public GameObject mSbarHitGamePrefab = null;
    [SerializeField]
    public GameObject mSbarTimingGamePrefab = null;
}

[Serializable]
public class MaterialValue {
    [SerializeField]
    public Sprite mTestIcon = null;
    [SerializeField]
    public float mTestCoolTime = 5.0f;
}

[Serializable]
public class UIValue {
    [SerializeField]
    public Color mStaminaColor;

    [SerializeField]
    public Sprite mFullHeart = null;
    [SerializeField]
    public Sprite mHalfHeart = null;
    [SerializeField]
    public float mHeartDist = 10.0f;
    [SerializeField]
    public GameObject mHeartPrefab = null;
}

public class LogicValue : MonoBehaviour {
    static LogicValue sInst = null;

    float deltaTime = 0.0f;

    [SerializeField]
    CameraValue mCameraValue = new CameraValue();
    [SerializeField]
    WitchValue mWitchValue = new WitchValue();
    [SerializeField]
    MiniGameValue mMiniGameValue = new MiniGameValue();
    [SerializeField]
    MaterialValue mMaterialValue = new MaterialValue();
    [SerializeField]
    UIValue mUIValue = new UIValue();

    public static float CameraLeftRate { get { return sInst.mCameraValue.mLeftMoveRate; } }
    public static float CameraRightRate { get { return sInst.mCameraValue.mRightMoveRate; } }
    public static float CameraUpRate { get { return sInst.mCameraValue.mUpMoveRate; } }
    public static float CameraDownRate { get { return sInst.mCameraValue.mDownMoveRate; } }

    public static float WitchSpeed { get { return sInst.mWitchValue.mSpeed; } }
    public static float WitchLadderSpeed { get { return sInst.mWitchValue.mLadderSpeed; } }
    public static int WitchInitStamina { get { return sInst.mWitchValue.mInitStamina; } }
    public static int WitchInitStaminaToTry { get { return sInst.mWitchValue.mInitStaminaToTry; } }
    public static int WitchInitHP { get { return sInst.mWitchValue.mInitHP; } }

    public static float WitchMoveIconShiftX { get { return sInst.mWitchValue.mIconMoveShiftX; } }
    public static Sprite WitchMatIcon { get { return sInst.mWitchValue.mMatIcon; } }
    public static Sprite WitchTalkIcon { get { return sInst.mWitchValue.mTalkIcon; } }
    public static Sprite WitchMakeIcon { get { return sInst.mWitchValue.mMakeIcon; } }
    public static Sprite WitchRestIcon { get { return sInst.mWitchValue.mRestIcon; } }

    public static float MiniGameShiftY { get { return sInst.mMiniGameValue.mMiniGameShiftY; } }
    public static float MiniGameTime { get { return sInst.mMiniGameValue.mMiniGameTime; } }
    public static Color StartColor { get { return sInst.mMiniGameValue.mStartColor; } }
    public static Color FinishColor { get { return sInst.mMiniGameValue.mFinishColor; } }
    public static GameObject CmdMiniGame { get { return sInst.mMiniGameValue.mCmdGamePrefab; } }
    public static float CmdArrowShiftY { get { return sInst.mMiniGameValue.mCmdArrowShiftY; } }
    public static Sprite CmdLeftArrowSprite { get { return sInst.mMiniGameValue.mCmdLeftArrowSprite; } }
    public static Sprite CmdRightArrowSprite { get { return sInst.mMiniGameValue.mCmdRightArrowSprite; } }
    public static Sprite CmdUpArrowSprite { get { return sInst.mMiniGameValue.mCmdUpArrowSprite; } }
    public static Sprite CmdDownArrowSprite { get { return sInst.mMiniGameValue.mCmdDownArrowSprite; } }
    public static GameObject SbarHitMiniGame { get { return sInst.mMiniGameValue.mSbarHitGamePrefab; } }
    public static GameObject SbarTimingMiniGame { get { return sInst.mMiniGameValue.mSbarTimingGamePrefab; } }

    public static Sprite TestIcon { get { return sInst.mMaterialValue.mTestIcon; } }
    public static float TestCoolTime { get { return sInst.mMaterialValue.mTestCoolTime; } }

    public static Color StaminaColor { get { return sInst.mUIValue.mStaminaColor; } }
    public static Sprite FullHeart { get { return sInst.mUIValue.mFullHeart; } }
    public static Sprite HalfHeart { get { return sInst.mUIValue.mHalfHeart; } }
    public static float HeartDist { get { return sInst.mUIValue.mHeartDist; } }
    public static GameObject HeartPrefab { get { return sInst.mUIValue.mHeartPrefab; } }

    float mWindowWidth;
    public static float WindowWidth { get { return sInst.mWindowWidth; } }
    float mWindowHeight;
    public static float WindowHeight { get { return sInst.mWindowHeight; } }

    Rect mBgRect;
    public static Rect BgRect { get { return sInst.mBgRect; } }

    void Awake() {
        sInst = this;
    }

    // Start is called before the first frame update
    void Start() {
        mWindowHeight = Camera.main.orthographicSize * 2;
        mWindowWidth = mWindowHeight * Camera.main.aspect;

        // 배경의 Rect를 얻어온다
        Sprite BgSprite = GameObject.Find("Field Bg").GetComponent<SpriteRenderer>().sprite;
        Rect BgPixelRect = BgSprite.rect;
        mBgRect = new Rect(-BgPixelRect.width / 2 / BgSprite.pixelsPerUnit, -BgPixelRect.height / 2 / BgSprite.pixelsPerUnit,
            BgPixelRect.width / BgSprite.pixelsPerUnit, BgPixelRect.height / BgSprite.pixelsPerUnit);
    }

    // Update is called once per frame
    void Update() {
        deltaTime += (Time.unscaledDeltaTime - deltaTime) * 0.1f;
        //Debug.LogFormat("FPS : {0}", 1 / 1.0f / deltaTime);
    }
}
