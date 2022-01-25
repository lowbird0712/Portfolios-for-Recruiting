using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PotScript : ObjectScript {
    public override void Action() {
        // 제작 UI를 연다
        WitchScript.UsingFunction = true;
        CraftScript.Inst.gameObject.SetActive(true);
        CraftScript.PenLocalPosInit();
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() {
        base.Update();
    }

    void OnTriggerEnter2D(Collider2D collision) {
        base.OnTriggerEnter2D(collision);
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().sprite = LogicValue.WitchMakeIcon;
    }

    void OnTriggerExit2D(Collider2D collision) {
        base.OnTriggerExit2D(collision);
        WitchScript.Inst.transform.GetChild(0).GetComponent<SpriteRenderer>().sprite = null;
    }
}
