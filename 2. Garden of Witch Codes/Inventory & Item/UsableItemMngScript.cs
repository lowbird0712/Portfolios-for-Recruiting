using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UsableItemMngScript : MonoBehaviour {
    public static void Use(string _nameKey) {
        // 각 아이템의 nameKey에 따라 호출되는 기능 함수가 달라진다
        switch (_nameKey) {
            case "보통 감자칩":
                PotatoChips();
                break;
        }
    }

    static void PotatoChips() {
        Debug.Log("감자칩을 먹으니 맛이 있군요!");
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() { }
}
