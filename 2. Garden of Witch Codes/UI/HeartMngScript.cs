using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeartMngScript : MonoBehaviour {
    public static HeartMngScript Inst { get; private set; } = null;

    private void Awake() {
        Inst = this;
    }

    // Start is called before the first frame update
    void Start() {
        for (int i = 0; i < LogicValue.WitchInitHP; i++) {
            RectTransform rectTransform = (RectTransform)Instantiate(LogicValue.HeartPrefab).transform;
            float heartWidth = rectTransform.sizeDelta.x;
            rectTransform.parent = transform;
            rectTransform.localPosition = Vector3.right * i * (heartWidth + LogicValue.HeartDist);
        }
    }

    // Update is called once per frame
    void Update() { }
}
