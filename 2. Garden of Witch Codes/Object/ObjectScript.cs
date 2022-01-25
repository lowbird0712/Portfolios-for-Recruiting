using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class ObjectScript : MonoBehaviour {
    protected bool mIsActive = false;

    public abstract void Action();

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    protected void Update() {
        // 플레이어가 상호작용 키를 누를 때 상호작용 가능하다면 작동한다
        if (Input.GetKeyDown(KeyCode.Z)) {
            if (!mIsActive || WitchScript.UsingObject || WitchScript.UsingFunction)
                return;

            Action();
        }
    }

    protected void OnTriggerEnter2D(Collider2D collision) {
        mIsActive = true;
    }

    protected void OnTriggerExit2D(Collider2D collision) {
        mIsActive = false;
    }
}
