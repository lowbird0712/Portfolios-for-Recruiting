using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class FunctionMngScript : MonoBehaviour {
    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() {
        if (WitchScript.UsingFunction)
            return;

        if (Input.GetKeyDown(KeyCode.I)) {
            InventoryScript.Inst.gameObject.SetActive(true);
            InventoryScript.PenLocalPosInit();
            WitchScript.UsingFunction = true;
        }
    }
}
