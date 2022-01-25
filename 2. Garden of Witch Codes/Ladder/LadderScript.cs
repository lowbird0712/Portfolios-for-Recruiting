using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LadderScript : MonoBehaviour {
    //bool mIsActive = false;
    //public bool IsActive { set { mIsActive = value; } }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }

    void OnTriggerExit2D(Collider2D collision) {
        // 마녀가 사다리의 이용을 마칠 때 마녀를 원래 상태로 되돌린다
        WitchScript.Inst.GetComponent<BoxCollider2D>().isTrigger = false;
        WitchScript.UsingLadder = false;
        WitchScript.Inst.GetComponent<Rigidbody2D>().gravityScale = 1.0f;
        WitchScript.WitchVelocity = Vector2.zero;

        if (Input.GetKey(KeyCode.LeftArrow) && Input.GetKey(KeyCode.RightArrow))
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchIdle");
        else if (Input.GetKey(KeyCode.LeftArrow)) {
            WitchScript.Inst.GetComponent<SpriteRenderer>().flipX = true;
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchWalk");
        }
        else if (Input.GetKey(KeyCode.RightArrow)) {
            WitchScript.Inst.GetComponent<SpriteRenderer>().flipX = false;
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchWalk");
        }
        else
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchIdle");
    }
}
