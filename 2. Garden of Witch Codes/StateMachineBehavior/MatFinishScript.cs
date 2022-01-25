using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MatFinishScript : StateMachineBehaviour {
    public override void OnStateExit(Animator animator, AnimatorStateInfo stateInfo, int layerIndex) {
        // 채집을 완료하는 애니메이션의 재생 종료 직후 소녀는 다른 오브젝트와 상호작용 및 이동이 가능해지며 Idle 애니메이션으로 돌아간다
        WitchScript.UsingObject = false;
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().enabled = true;

        if (Input.GetKey(KeyCode.LeftArrow)) {
            WitchScript.Inst.GetComponent<SpriteRenderer>().flipX = true;
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchWalk");
        }
        else if (Input.GetKey(KeyCode.RightArrow)) {
            WitchScript.Inst.GetComponent<SpriteRenderer>().flipX = false;
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchWalk");
        }
    }
}
