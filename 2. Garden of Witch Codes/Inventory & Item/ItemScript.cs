using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ItemScript : MonoBehaviour {
    string mNameKey = "";
    public string NameKey {
        get { return mNameKey; }
        set { mNameKey = value; }
    }

    bool mUsable = false;
    public bool Usable {
        get { return mUsable; }
        set { mUsable = value; }
    }

    int mNum = 0;
    public int Num {
        get { return mNum; }
        set { mNum = value; }
    }

    public void Copy(ItemScript _script) {
        // _script의 정보를 복사한다
        mNameKey = _script.mNameKey;
        mUsable = _script.mUsable;
        Num = _script.Num;
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }
}
