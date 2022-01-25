using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class InventoryScript : MonoBehaviour {
    public static InventoryScript Inst { get; private set; } = null;

    Vector3 mInitPenLocalPos;
    int mSelectingItemIndex = 0;
    int mLastItemIndex = -1;

    public static int LastItemIndex {
        get { return Inst.mLastItemIndex; }
        set { Inst.mLastItemIndex = value; }
    }

    // 아이템들을 앞쪽에 빈 공간이 없게 땡긴다
    public void Arrange() {
        GameObject Item = null;
        GameObject pastItem = null;

        for (int i = mSelectingItemIndex + 1; i < mLastItemIndex; i++) {
            pastItem = transform.GetChild(i - 1).gameObject;
            Item = transform.GetChild(i).gameObject;
            pastItem.GetComponent<Image>().sprite = Item.GetComponent<Image>().sprite;
            pastItem.GetComponent<ItemScript>().Copy(Item.GetComponent<ItemScript>());
        }

        pastItem = transform.GetChild(mLastItemIndex - 1).gameObject;
        Item = transform.GetChild(mLastItemIndex).gameObject;
        pastItem.GetComponent<Image>().sprite = Item.GetComponent<Image>().sprite;
        pastItem.GetComponent<ItemScript>().Copy(Item.GetComponent<ItemScript>());

        Item.GetComponent<Image>().sprite = null;
        Item.GetComponent<Image>().enabled = false;
        mLastItemIndex--;
    }

    // 펜의 위치를 처음 상태로 되돌린다
    public static void PenLocalPosInit() {
        Inst.transform.GetChild(16).transform.localPosition = Inst.mInitPenLocalPos;
        Inst.mSelectingItemIndex = 0;
    }

    private void Awake() {
        Inst = this;
        mInitPenLocalPos = transform.GetChild(16).transform.localPosition;
        gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void Update() {
        if (Input.GetKeyDown(KeyCode.Z)) {
            // 아이템의 개수가 0개이거나 소모가 불가능한 경우에 리턴한다
            GameObject Item = transform.GetChild(mSelectingItemIndex).gameObject;
            ItemScript Script = Item.GetComponent<ItemScript>();

            if (Script.Num < 1 || !Script.Usable)
                return;

            // 아이템을 사용하고 개수를 1 감소시킨다
            UsableItemMngScript.Use(Script.NameKey);
            Script.Num -= 1;
            
            if (Script.Num >= 1)
                return;

            // 사용한 아이템 이외에 다른 아이템을 가지고 있지 않다면 별도로 처리한다
            if (mLastItemIndex == 0) {
                Item = transform.GetChild(0).gameObject;
                Item.GetComponent<Image>().sprite = null;
                Item.GetComponent<Image>().enabled = false;
                mLastItemIndex--;
                return;
            }

            // 사용한 아이템이 마지막인 경우 인벤토리의 빈자리를 없앤다
            Arrange();
        }
        else if (Input.GetKeyDown(KeyCode.X)) {
            // 인벤토리를 닫는다
            gameObject.SetActive(false);
            WitchScript.UsingFunction = false;
            return;
        }
        else if (Input.GetKeyDown(KeyCode.C)) {
            GameObject Item = transform.GetChild(mSelectingItemIndex).gameObject;
            ItemScript Script = Item.GetComponent<ItemScript>();

            // 펜이 위치한 곳에 아이템이 없을 경우 리턴한다
            if (Script.Num < 1)
                return;

            // 아이템을 1개 버린다
            Script.Num -= 1;

            if (Script.Num >= 1)
                return;

            // 사용한 아이템 이외에 다른 아이템을 가지고 있지 않다면 별도로 처리한다
            if (mLastItemIndex == 0) {
                Item = transform.GetChild(0).gameObject;
                Item.GetComponent<Image>().sprite = null;
                Item.GetComponent<Image>().enabled = false;
                mLastItemIndex--;
                return;
            }

            // 사용한 아이템이 마지막인 경우 인벤토리의 빈자리를 없앤다
            Arrange();

            return;
        }

        // 펜을 움직인다
        if (Input.anyKeyDown) {
            if (Input.GetKeyDown(KeyCode.LeftArrow)) {
                if (mSelectingItemIndex % 4 == 0)
                    mSelectingItemIndex += 3;
                else
                    mSelectingItemIndex--;
            }
            else if (Input.GetKeyDown(KeyCode.RightArrow)) {
                if (mSelectingItemIndex % 4 == 3)
                    mSelectingItemIndex -= 3;
                else
                    mSelectingItemIndex++;
            }
            else if (Input.GetKeyDown(KeyCode.UpArrow)) {
                if (mSelectingItemIndex <= 3)
                    mSelectingItemIndex += 12;
                else
                    mSelectingItemIndex -= 4;
            }
            else if (Input.GetKeyDown(KeyCode.DownArrow)) {
                if (mSelectingItemIndex >= 12)
                    mSelectingItemIndex -= 12;
                else
                    mSelectingItemIndex += 4;
            }

            int rightShiftNum = mSelectingItemIndex % 4;
            int downShiftNum = mSelectingItemIndex / 4;
            transform.GetChild(16).transform.localPosition = mInitPenLocalPos + Vector3.right * 98 * rightShiftNum + Vector3.down * 104 * downShiftNum;
        }
    }
}
