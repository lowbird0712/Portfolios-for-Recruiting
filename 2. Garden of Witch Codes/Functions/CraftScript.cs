using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CraftRecipe {
    public struct InnerRecipe {
        public string itemKey;
        public int itemNum;

        public InnerRecipe(string _itemKey, int _itemNum) {
            itemKey = _itemKey;
            itemNum = _itemNum;
        }
    }

    bool mIsNeed = false;
    LinkedList<InnerRecipe> mNeedItems = new LinkedList<InnerRecipe>();

    public bool IsNeed { get; set; }
    public LinkedList<InnerRecipe> NeedItems { get; }

    // 실수로 mIsNeed를 추가하지 않는 경우에서 발생하는 오류를 예방하기 위해 만든 생성자
    public CraftRecipe(bool _isNeed) {
        mIsNeed = _isNeed;
    }

    public void AddNeedItem(string _itemKey, int _itemNum) {
        mNeedItems.AddFirst(new InnerRecipe(_itemKey, _itemNum));
    }
}

public class CraftScript : MonoBehaviour {
    public static CraftScript Inst { get; private set; } = null;

    Dictionary<string, CraftRecipe> mRecipes = new Dictionary<string, CraftRecipe>();
    LinkedList<string> mRecipeKeys = new LinkedList<string>();
    Vector3 mInitPenLocalPos;
    int mSelectingRecipeIndex = 0;
    int mLastRecipeIndex = -1;

    static public int LastRecipeIndex {
        get { return Inst.mLastRecipeIndex; }
        set { Inst.mLastRecipeIndex = value; }
    }

    static public void AddRecipe(string _recipeKey) {
        Inst.mRecipeKeys.AddFirst(_recipeKey);
        Inst.mRecipes[_recipeKey] = new CraftRecipe(false);
        Inst.mLastRecipeIndex++;
    }

    static public void MakeRecipeNeed(string _itemKey, bool _isNeed) {
        if (!Inst.mRecipes.ContainsKey(_itemKey))
            return;

        Inst.mRecipes[_itemKey].IsNeed = _isNeed;
    }

    // 펜의 위치를 처음 상태로 되돌린다
    public static void PenLocalPosInit() {
        Inst.transform.GetChild(1).transform.localPosition = Inst.mInitPenLocalPos;
        Inst.mSelectingRecipeIndex = 0;
    }

    private void Awake() {
        Inst = this;
        mInitPenLocalPos = transform.GetChild(1).transform.localPosition;
        gameObject.SetActive(false);
    }

    // Start is called before the first frame update
    void Start() {// 모든 제작법을 추가한다, 추가 방법은 AddRecipe()을 통해 일단 레시피를 만들고 그 뒤에 필요한 아이템의 종류와 개수를 각각 추가해준다.
        AddRecipe("더 맛있는 감자칩");
        mRecipes["더 맛있는 감자칩"].AddNeedItem("보통 감자칩", 2);
        AddRecipe("초 맛있는 감자칩");
        mRecipes["초 맛있는 감자칩"].AddNeedItem("보통 감자칩", 5);

        // 제작법 정보를 좌측 페이지에 표시한다

    }

    // Update is called once per frame
    void Update() {
        if (Input.GetKeyDown(KeyCode.Q)) {
            // 퀘스트에 필요한 아이템만 표시한다
        }
        else if (Input.GetKeyDown(KeyCode.A)) {
            // 아이템을 제작한다
        }
        else if (Input.GetKeyDown(KeyCode.X)) {
            // 제작 UI를 닫는다
            gameObject.SetActive(false);
            WitchScript.UsingFunction = false;
            return;
        }

        // 펜을 움직인다
        if (Input.anyKeyDown) {
            if (Input.GetKeyDown(KeyCode.UpArrow)) {
                if (mSelectingRecipeIndex == 0)
                    mSelectingRecipeIndex = 12;
                else
                    mSelectingRecipeIndex--;
            }
            else if (Input.GetKeyDown(KeyCode.DownArrow)) {
                if (mSelectingRecipeIndex == 12)
                    mSelectingRecipeIndex = 0;
                else
                    mSelectingRecipeIndex++;
            }

            transform.GetChild(1).transform.localPosition = mInitPenLocalPos + Vector3.down * 35 * mSelectingRecipeIndex;
        }
    }
}
