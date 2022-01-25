using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TopLadderScript : MonoBehaviour {
    bool mIsActive = false;

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() {
        // 마녀가 범위 내에서 아래쪽 화살표를 누르면 사다리를 오르기 시작한다
        if (Input.GetKeyDown(KeyCode.DownArrow)) {
            if (!mIsActive || WitchScript.UsingLadder)
                return;

            Vector3 witchPos = WitchScript.WitchPos;

            WitchScript.Inst.GetComponent<BoxCollider2D>().isTrigger = true;

            WitchScript.UsingLadder = true;
            WitchScript.WitchPos = new Vector3(transform.position.x, witchPos.y, witchPos.z);
            WitchScript.Inst.GetComponent<Rigidbody2D>().gravityScale = 0.0f;
            WitchScript.Inst.GetComponent<Animator>().SetTrigger("WitchLadder");
        }
    }

    void OnTriggerEnter2D(Collider2D collision) {
        mIsActive = true;
    }

    void OnTriggerExit2D(Collider2D collision) {
        mIsActive = false;
    }
}
