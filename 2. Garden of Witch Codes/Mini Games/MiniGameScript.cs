using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public enum SUCCEED_FLAG {
    SF_DEFAULT,
    SF_SUCCEED,
    SF_FAILED
};

public class MiniGameScript : MonoBehaviour {
    static MiniGameScript sInst = null;

    protected GameObject mTimeObject = null;
    protected SpriteRenderer mTImeSpriteRenderer = null;
    protected float mTimeWidth = 0.0f;
    protected Color mStartColor;
    protected Color mFinishColor;

    protected SUCCEED_FLAG mSFflag = SUCCEED_FLAG.SF_DEFAULT;
    protected float mTimer = 0.0f;

    protected GameObject mMaterial = null;
    public static GameObject Material { set { sInst.mMaterial = value; } }

    void Awake() {
        sInst = this;
    }

    // Start is called before the first frame update
    protected void Start() {
        mTimeObject = transform.GetChild(1).gameObject;
        Sprite sprite = mTimeObject.GetComponent<SpriteRenderer>().sprite;
        mTimeWidth = sprite.rect.width / sprite.pixelsPerUnit;
        mTimer = LogicValue.MiniGameTime;

        mTImeSpriteRenderer = mTimeObject.GetComponent<SpriteRenderer>();
        mStartColor = LogicValue.StartColor;
        mFinishColor = LogicValue.FinishColor;
    }

    // Update is called once per frame
    protected void Update() {
        switch (mSFflag) {
            // 미니게임 성공 시
            case SUCCEED_FLAG.SF_SUCCEED:
                WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchMatSucceed");
                mMaterial.GetComponent<MaterialScript>().IsSucceed = true;
                Destroy(gameObject);
                break;
            // 미니게임 실패 시
            case SUCCEED_FLAG.SF_FAILED:
                WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchMatFail");
                WitchScript.MaterialFailDamaged();
                Destroy(gameObject);
                break;
        }

        mTimer -= Time.deltaTime;
        mTimeObject.transform.localScale = new Vector3(mTimer / LogicValue.MiniGameTime, 1.0f, 1.0f);
        mTimeObject.transform.localPosition = Vector3.left * mTimeWidth * (1.0f - mTimeObject.transform.localScale.x) / 2.0f;

        mTImeSpriteRenderer.color = mStartColor * mTimer / LogicValue.MiniGameTime + mFinishColor * (1 - mTimer / LogicValue.MiniGameTime);

        if (mTimer <= 0)
            mSFflag = SUCCEED_FLAG.SF_FAILED;
    }
}
