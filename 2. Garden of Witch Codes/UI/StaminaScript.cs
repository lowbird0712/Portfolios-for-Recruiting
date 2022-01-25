using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StaminaScript : MonoBehaviour {
    static StaminaScript sInst = null;

    public static void ChangeStaminaRate(float _rate) {
        RectTransform rectTransform = sInst.gameObject.GetComponent<RectTransform>();

        float pastWidth = rectTransform.sizeDelta.x;
        rectTransform.sizeDelta = new Vector2(pastWidth * _rate, rectTransform.sizeDelta.y);
        float shiftX = (pastWidth - rectTransform.sizeDelta.x) / 2;
        rectTransform.position += Vector3.left * shiftX;
    }

    void Awake() {
        sInst = this;
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }
}
