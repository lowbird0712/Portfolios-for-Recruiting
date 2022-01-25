using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class MaterialScript : ObjectScript {
    protected float mCoolTime = 0.0f;
    float mCoolTimeTimer = 0.0f;

    bool mIsCoolTime = false;

    protected bool mIsSucceed = false;
    public bool IsSucceed { set { mIsSucceed = value; } }

    public void AddItem(string _nameKey) {
        ItemScript Script = null;

        // 인벤토리가 빈 상태이면 새로 집어넣어라
        if (InventoryScript.LastItemIndex == -1) {
            Script = InventoryScript.Inst.transform.GetChild(0).gameObject.GetComponent<ItemScript>();
            Script.gameObject.GetComponent<Image>().enabled = true;
            Script.gameObject.GetComponent<Image>().sprite = LogicValue.TestIcon;
            Script.NameKey = _nameKey;
            Script.Usable = true;
            Script.Num = 1;

            InventoryScript.LastItemIndex++;

            return;
        }

        // 인벤토리를 뒤져서 이미 있는 아이템이면 숫자만 증가
        for (int i = 0; i <= InventoryScript.LastItemIndex; i++) {
            Script = InventoryScript.Inst.transform.GetChild(i).gameObject.GetComponent<ItemScript>();
            if (Script.NameKey == _nameKey) {
                Script.Num += 1;
                return;
            }
        }

        // 없는 아이템이면 새로 집어넣어라
        Script = InventoryScript.Inst.transform.GetChild(InventoryScript.LastItemIndex + 1).gameObject.GetComponent<ItemScript>();
        Script.gameObject.GetComponent<Image>().enabled = true;
        Script.gameObject.GetComponent<Image>().sprite = LogicValue.TestIcon;
        Script.NameKey = _nameKey;
        Script.Usable = true;
        Script.Num = 1;

        InventoryScript.LastItemIndex++;
    }

    public override void Action() {
        if (WitchScript.Stamina < WitchScript.StaminaToTry)
            return;

        // 채집 미니게임을 시작한다
        WitchScript.UsingObject = true;
        WitchScript.MaterialTry();
        WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchMatStart");
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().enabled = false;
        MiniGameScript.Material = gameObject;
    }

    // Start is called before the first frame update
    void Start() { }

    protected void Update() {
        base.Update();

        // 쿨타임 상태일 때 쿨타임을 진행시킨다
        if (mIsCoolTime) {
            mCoolTimeTimer += Time.deltaTime;
            if (mCoolTimeTimer > mCoolTime) {
                mIsActive = false;
                mIsCoolTime = false;
                mIsSucceed = false;
                mCoolTimeTimer = 0.0f;
                //WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().enabled = true;
                gameObject.GetComponent<SpriteRenderer>().enabled = true;
                gameObject.GetComponent<BoxCollider2D>().enabled = true;
            }
        }

        // 채집물을 쿨타임 상태로 만든다
        if (mIsSucceed) {
            mIsActive = false;
            mIsCoolTime = true;
            gameObject.GetComponent<SpriteRenderer>().enabled = false;
            gameObject.GetComponent<BoxCollider2D>().enabled = false;
        }
    }

    void OnTriggerEnter2D(Collider2D collision) {
        base.OnTriggerEnter2D(collision);
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().sprite = LogicValue.WitchMatIcon;
    }

    void OnTriggerExit2D(Collider2D collision) {
        base.OnTriggerExit2D(collision);
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().sprite = null;
    }
}
