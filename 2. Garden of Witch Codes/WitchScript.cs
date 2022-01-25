using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class WitchScript : MonoBehaviour {
    SpriteRenderer mSpriteRenderer = null;
    Animator mAnimator = null;
    Rigidbody2D mRigidbody2D = null;

    public static WitchScript Inst { get; private set; } = null;
    public static Vector3 WitchPos {
        get { return Inst.transform.position; }
        set { Inst.transform.position = value; }
    }
    public static Vector3 WitchVelocity {
        get { return Inst.mRigidbody2D.velocity; }
        set { Inst.mRigidbody2D.velocity = value; }
    }

    int mStamina = 0;
    int mMaxStamina = 0;
    int mStaminaToTry = 0;
    int mMaxHP = 0;
    float mHP = 0;
    public static int Stamina { get { return Inst.mStamina; } }
    public static int StaminaToTry { get { return Inst.mStaminaToTry; } }
    static void ChangeStamina(int _change) {
        Inst.mStamina += _change;
        if (Inst.mStamina > Inst.mMaxStamina)
            Inst.mStamina = Inst.mMaxStamina;
        StaminaScript.ChangeStaminaRate((float)Inst.mStamina / Inst.mMaxStamina);
    }
    static void ChangeHP(float _change) {
        // 하트의 이미지를 전부 null로 설정해준다. 이후 처음부터 수치에 맞게 반쪽, 그리고 완전한 하트의 이미지를 설정해 줄 것이다
        for (int i = 0; i < Inst.mMaxHP; i++) {
            HeartMngScript.Inst.transform.GetChild(i).gameObject.GetComponent<Image>().sprite = null;
            HeartMngScript.Inst.transform.GetChild(i).gameObject.GetComponent<Image>().enabled = false;
        }

        Inst.mHP += _change;

        if (Inst.mHP <= 0) {
            Inst.mHP = 0;
            // 플레이어를 리타이어한다
        }

        int heartNum = (int)Inst.mHP;
        bool isInteger = (Inst.mHP - heartNum == 0);
        for (int i = 0; i < heartNum; i++) {
            HeartMngScript.Inst.transform.GetChild(i).gameObject.GetComponent<Image>().sprite = LogicValue.FullHeart;
            HeartMngScript.Inst.transform.GetChild(i).gameObject.GetComponent<Image>().enabled = true;
        }

        if (!isInteger) {
            HeartMngScript.Inst.transform.GetChild(heartNum).gameObject.GetComponent<Image>().sprite = LogicValue.HalfHeart;
            HeartMngScript.Inst.transform.GetChild(heartNum).gameObject.GetComponent<Image>().enabled = true;
        }
    }
    public static void MaterialTry() { ChangeStamina(-Inst.mStaminaToTry); }
    public static void MaterialFailDamaged() { ChangeHP(-0.5f); }

    bool mUsingLadder = false;
    public static bool UsingLadder {
        get { return Inst.mUsingLadder; }
        set { Inst.mUsingLadder = value; }
    }

    bool mUsingObject = false;
    public static bool UsingObject {
        get { return Inst.mUsingObject; }
        set { Inst.mUsingObject = value; }
    }

    bool mUsingFunction = false;
    public static bool UsingFunction {
        get { return Inst.mUsingFunction; }
        set { Inst.mUsingFunction = value; }
    }

    private void Awake() {
        Inst = this;

        mSpriteRenderer = GetComponent<SpriteRenderer>();
        if (!mSpriteRenderer)
            Debug.LogError("마녀 오브젝트에 SpriteRenderer가 없습니다!");

        mAnimator = GetComponent<Animator>();
        if (!mAnimator)
            Debug.LogError("마녀 오브젝트에 Animator가 없습니다!");

        mRigidbody2D = GetComponent<Rigidbody2D>();
        if (!mRigidbody2D)
            Debug.LogError("마녀 오브젝트에 RigidBody2D가 없습니다!");
    }

    // Start is called before the first frame update
    void Start() {
        // 마녀의 피로도와 체력을 초기화한다
        mMaxStamina = LogicValue.WitchInitStamina;
        mStamina = mMaxStamina;
        mStaminaToTry = LogicValue.WitchInitStaminaToTry;
        mMaxHP = LogicValue.WitchInitHP;
        mHP = mMaxHP;
    }

    private void FixedUpdate() {
        // 오브젝트와 지속적인 상호작용 중이거나 기능을 이용 중이라면 소녀는 이동할 수 없다
        if (mUsingObject || mUsingFunction)
            return;

        // 마녀가 걸어서 이동한다
        if (!mUsingLadder) {
            if (Input.GetKey(KeyCode.LeftArrow) && Input.GetKey(KeyCode.RightArrow))
                mRigidbody2D.velocity = new Vector2(0, mRigidbody2D.velocity.y);
            else if (Input.GetKey(KeyCode.LeftArrow))
                mRigidbody2D.velocity = new Vector2(-LogicValue.WitchSpeed, mRigidbody2D.velocity.y);
            else if (Input.GetKey(KeyCode.RightArrow))
                mRigidbody2D.velocity = new Vector2(LogicValue.WitchSpeed, mRigidbody2D.velocity.y);
            else
                mRigidbody2D.velocity = new Vector2(0, mRigidbody2D.velocity.y);
        }

        // 마녀가 사다리를 타고 이동한다
        if (mUsingLadder) {
            if (Input.GetKey(KeyCode.UpArrow) && Input.GetKey(KeyCode.DownArrow))
                mRigidbody2D.velocity = Vector2.zero;
            else if (Input.GetKey(KeyCode.UpArrow))
                mRigidbody2D.velocity = Vector3.up * LogicValue.WitchLadderSpeed;
            else if (Input.GetKey(KeyCode.DownArrow))
                mRigidbody2D.velocity = Vector3.down * LogicValue.WitchLadderSpeed;
            else
                mRigidbody2D.velocity = Vector2.zero;
        }
    }

    // Update is called once per frame
    void Update() {
        // 오브젝트와 지속적인 상호작용 중이거나 기능을 이용 중이라면 소녀는 이동할 수 없다
        if (mUsingObject || mUsingFunction)
            return;

        Vector3 IconLocalPos = transform.GetChild(0).localPosition;

        // 소녀가 몸의 방향을 바꾼다
        if (Input.GetKeyDown(KeyCode.LeftArrow) && !mUsingLadder) {
            mSpriteRenderer.flipX = true;
            if (Input.GetKey(KeyCode.RightArrow)) {
                mAnimator.SetTrigger("WitchIdle");
                transform.GetChild(0).localPosition = new Vector3(0.0f, IconLocalPos.y, IconLocalPos.z);
                return;
            }
            mAnimator.SetTrigger("WitchWalk");
            transform.GetChild(0).localPosition = new Vector3(-LogicValue.WitchMoveIconShiftX, IconLocalPos.y, IconLocalPos.z);
            return;
        }
        if (Input.GetKeyDown(KeyCode.RightArrow) && !mUsingLadder) {
            mSpriteRenderer.flipX = false;
            transform.GetChild(0).localPosition += Vector3.right * LogicValue.WitchMoveIconShiftX;
            if (Input.GetKey(KeyCode.LeftArrow)) {
                mAnimator.SetTrigger("WitchIdle");
                transform.GetChild(0).localPosition = new Vector3(0.0f, IconLocalPos.y, IconLocalPos.z);
                return;
            }
            mAnimator.SetTrigger("WitchWalk");
            transform.GetChild(0).localPosition = new Vector3(LogicValue.WitchMoveIconShiftX, IconLocalPos.y, IconLocalPos.z);
            return;
        }

        // 마녀가 좌우로 걸어가는 것을 멈춘다

        if (Input.GetKeyUp(KeyCode.LeftArrow) && !mUsingLadder) {
            transform.GetChild(0).localPosition += Vector3.right * LogicValue.WitchMoveIconShiftX;
            if (Input.GetKey(KeyCode.RightArrow)) {
                mSpriteRenderer.flipX = false;
                mAnimator.SetTrigger("WitchWalk");
                transform.GetChild(0).localPosition = new Vector3(LogicValue.WitchMoveIconShiftX, IconLocalPos.y, IconLocalPos.z);
                return;
            }
            mAnimator.SetTrigger("WitchIdle");
            transform.GetChild(0).localPosition = new Vector3(0.0f, IconLocalPos.y, IconLocalPos.z);
            return;
        }
        if (Input.GetKeyUp(KeyCode.RightArrow) && !mUsingLadder) {
            transform.GetChild(0).localPosition += Vector3.left * LogicValue.WitchMoveIconShiftX;
            if (Input.GetKey(KeyCode.LeftArrow)) {
                mSpriteRenderer.flipX = true;
                mAnimator.SetTrigger("WitchWalk");
                transform.GetChild(0).localPosition = new Vector3(-LogicValue.WitchMoveIconShiftX, IconLocalPos.y, IconLocalPos.z);
                return;
            }
            mAnimator.SetTrigger("WitchIdle");
            transform.GetChild(0).localPosition = new Vector3(0.0f, IconLocalPos.y, IconLocalPos.z);
            return;
        }

        // 마녀가 오르내리는 동작을 한다
        if (mUsingLadder) {
            if (Input.GetKeyDown(KeyCode.UpArrow) || Input.GetKeyDown(KeyCode.DownArrow)) {
                if (Input.GetKey(KeyCode.UpArrow) && Input.GetKey(KeyCode.DownArrow))
                    mAnimator.speed = 0;
                else {
                    mAnimator.SetTrigger("WitchLadder");
                    mAnimator.speed = 1;
                }
            }
            if (Input.GetKeyUp(KeyCode.UpArrow) || Input.GetKeyUp(KeyCode.DownArrow)) {
                if (Input.GetKey(KeyCode.UpArrow) || Input.GetKey(KeyCode.DownArrow)) {
                    mAnimator.SetTrigger("WitchLadder");
                    mAnimator.speed = 1;
                }
                else
                    mAnimator.speed = 0;
            }
        }
    }
}
