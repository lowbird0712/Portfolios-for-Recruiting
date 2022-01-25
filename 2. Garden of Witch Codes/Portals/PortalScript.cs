using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public abstract class PortalScript : MonoBehaviour {
    public abstract bool PortalTest();
    public abstract void UsePortal();

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }

    //private void OnTriggerEnter2D(Collider2D collision) {
    //    WitchScript.UsingPortal = true;
    //}

    private void OnTriggerExit2D(Collider2D collision) {
        // 주로 포탈의 방향에 대한 테스트가 이루어질 예정
        if (!PortalTest()) {
            //WitchScript.UsingPortal = false;
            return;
        }

        UsePortal();
    }
}
