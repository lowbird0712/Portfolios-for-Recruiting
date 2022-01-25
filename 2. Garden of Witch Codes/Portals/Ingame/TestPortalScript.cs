using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class TestPortalScript : PortalScript {
    public override bool PortalTest() {
        if (Input.GetKey(KeyCode.RightArrow))
            return true;
        else
            return false;
    }

    public override void UsePortal() {
        SceneManager.LoadScene("TestScene");
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }
}
