using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class TestMaterialScript : MaterialScript {
    public override void Action() {
        // 미니게임의 종류에 따라 바꿔가면서 테스트할 것
        GameObject miniGame = Instantiate(LogicValue.CmdMiniGame);
        miniGame.GetComponent<CmdGameScript>().InitCommand(4);
        miniGame.transform.parent = WitchScript.Inst.transform;
        miniGame.transform.localPosition = new Vector3(0.0f, LogicValue.MiniGameShiftY, 0.0f);

        base.Action();
    }

    // Start is called before the first frame update
    void Start() {
        mCoolTime = LogicValue.TestCoolTime;
    }

    void Update() {
        base.Update();

        // 미니게임 성공 시 인벤토리에 채집물을 집어넣는다
        // 임시로 감자칩(Potato Chips)이 채집물로써 주어진다
        // 함수화해서 MaterialScript에 집어넣자
        if (mIsSucceed) {
            AddItem("보통 감자칩");
            mIsSucceed = false;
        }
    }
}
