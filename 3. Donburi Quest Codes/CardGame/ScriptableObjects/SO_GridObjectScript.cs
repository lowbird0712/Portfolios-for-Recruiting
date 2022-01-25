using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ObjectItem {
    public delegate void FunctionPointer();

    public FunctionPointer[]    effectFunctionNames;
    public FunctionPointer[]    endEffectFunctionNames;
    public Sprite               sprite;
    public string[]             neededObjectNames;
    public string[]             insideObjectNames;
    public string[]             usableSpellNames;
    public string               nextObjectName;
    public int[]                neenedObjectNums;
    public int[]                insideObjectNums;
    public int                  originCountDown = 0;
}

[CreateAssetMenu(fileName = "SO_GridObject", menuName = "Scriptable Object/SO_GridObject")]
public class SO_GridObjectScript : ScriptableObject {
    [SerializeField] Sprite[]   objectSprites = null;
    [SerializeField] Sprite[]   countDownSprites = null;

    string[]                    objectNames;
    ObjectItem[]                objectItems;

    public void SetUp(string _deckName) {
        //objectNames[0] = "";
        //objectItems[0].effectFunctionNames = new ObjectItem.FunctionPointer[1];
        //objectItems[0].endEffectFunctionNames = new ObjectItem.FunctionPointer[1];
        //objectItems[0].sprite = objectSprites[0];
        //objectItems[0].neededObjectNames = new string[1];
        //objectItems[0].insideObjectNames = new string[1];
        //objectItems[0].usableSpellNames = new string[1];
        //objectItems[0].nextObjectName = "";
        //objectItems[0].neenedObjectNums = new int[1];
        //objectItems[0].insideObjectNums = new int[1];
        //objectItems[0].originCountDown = 0;

        if (_deckName == "규동 기본") {
            objectNames = new string[8];
            objectItems = new ObjectItem[8];

            // 밥솥(준비)
            objectNames[0] = "법솥(준비)";
            objectItems[0] = new ObjectItem();
            objectItems[0].sprite = objectSprites[0];
            objectItems[0].neededObjectNames = new string[1] { "쌀" };
            objectItems[0].insideObjectNames = new string[1];
            objectItems[0].usableSpellNames = new string[1] { "불지피기" };
            objectItems[0].nextObjectName = "쌀(조리중)";
            objectItems[0].neenedObjectNums = new int[1] { 1 };
            objectItems[0].insideObjectNums = new int[1];
            // 밥솥(조리중)
        }
    }

    public ObjectItem GetObjectItem(string _objectName) {
        ObjectItem result = new ObjectItem();

        for (int i = 0; i < objectNames.Length; i++) {
            if (objectNames[i] == _objectName) {
                result = objectItems[i];
                return result;
            }
        }

        Debug.LogError("찾으려는 ObjectItem이 Scriptable Object에 없습니다!");
        return result;
    }
}
