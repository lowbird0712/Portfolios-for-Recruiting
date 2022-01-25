using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.SceneManagement;
using DG.Tweening;

public class SysKey {
    public string   key = null;
    public string   effectKey = null;
    public int      panelSpriteNum = -1;
    public int      closeUpPanelSpriteNum = -1;
    public int      BGM_Num = -1;
    public int      BGM_SubNum = -1;

    public string   message = null;
    public int      effectNum = -1;
    public int      selectionIndex = -1;
    public int      standingOutNum = -1;
    public bool     fade = false;

    public SysKey() { }

    public SysKey(SysKey _key) {
        key = _key.key;
        effectKey = _key.effectKey;
        panelSpriteNum = _key.panelSpriteNum;
        closeUpPanelSpriteNum = _key.closeUpPanelSpriteNum;
        BGM_Num = _key.BGM_Num;
        BGM_SubNum = _key.BGM_SubNum;

        message = null;
        effectNum = -1;
        selectionIndex = -1;
        standingOutNum = -1;
        fade = _key.fade;
	}
}

public class SysKeySameness {
    public bool sameKey = true;
    public bool sameEffectKey = true;
    public bool samePanelSpriteNum = true;
    public bool sameCloseUpPanelSpriteNum = true;
    public bool same_BGM_Num = true;
    public bool same_BGM_SubNum = true;
}

public class Selection {
    public string   first = null;
    public string   second = null;
    public string   third = null;
    public bool[]   buttonLock = new bool[3] { false, false, false };
    public bool     isLove = false;

    public Selection(string _first, string _second, string _third = null) {
        first = _first;
        second = _second;
        third = _third;
	}

    public Selection ButtonLock(bool _first, bool _second, bool _third = false) {
        buttonLock[0] = _first;
        buttonLock[1] = _second;
        buttonLock[2] = _third;

        return this;
	}

    public Selection ChangeToLove() {
        isLove = true;
        return this;
	}
}

public class StoryNPC {
    public string   name = null;
    public string   eyeTriggerName = "Stop";
    public string   mouthTriggerName = "Stop";
    public int      spriteNum = -1;
    public int      markingNum = -1;

    public StoryNPC() { }

    public StoryNPC(string _name, string _eyeTriggerName, string _mouthTriggerName, int _spriteNum, int _markingNum) {
        name = _name;
        eyeTriggerName = _eyeTriggerName;
        mouthTriggerName = _mouthTriggerName;
        spriteNum = _spriteNum;
        markingNum = _markingNum;
    }

    public StoryNPC(StoryNPC _NPC) {
        name = _NPC.name;
        eyeTriggerName = _NPC.eyeTriggerName;
        mouthTriggerName = _NPC.mouthTriggerName;
        spriteNum = _NPC.spriteNum;
        markingNum = _NPC.markingNum;
    }
}

public class StoryChain {
    public SysKey   sysKey = null;
    public string   text = null;
    public StoryNPC NPC = null;
}

public class StoryBlock {
    StoryChain[] mStoryChains;
    int mLength = 0;

    public StoryChain[] StoryChains {
        get { return mStoryChains; }
        set { mStoryChains = value; }
    }
    public int Length {
        get { return mLength; }
        set { mLength = value; }
    }
}

public class StoryBlockMngScript : MonoBehaviour {
    static public StoryBlockMngScript   Inst { get; set; } = null;

    [SerializeField]
    CanvasGroup                         mCanvas = null;
    [SerializeField]
    CanvasGroup                         mCanvas_NPC = null;
    [SerializeField]
    CanvasGroup                         mTwoSelectCanvas = null;
    [SerializeField]
    Button[]                            mTwoSelectingButtons = null;
    [SerializeField]
    CanvasGroup                         mThreeSelectCanvas = null;
    [SerializeField]
    Button[]                            mThreeSelectingButtons = null;
    [SerializeField]
    CanvasGroup                         mMessageCanvas = null;
    [SerializeField]
    Text                                mMessageText = null;
    [SerializeField]
    Image                               mTextMainPanel = null;
    [SerializeField]
    Text                                mText = null;
    [SerializeField]
    Text                                mNPC_name = null;
    [SerializeField]
    Image                               mNPC_panel = null;
    [SerializeField]
    Animator                            mNPC_eyeAnimator = null;
    [SerializeField]
    Animator                            mNPC_mouthAnimator = null;
    [SerializeField]
    Image                               mSubNPC_panel = null;
    [SerializeField]
    Image                               mPanel = null;
    [SerializeField]
    Image                               mCloseUpPanel = null;
    [SerializeField]
    Image                               mFade = null;

    [SerializeField]
    Sprite[]                            mTextMainPanelSprites = null;
    [SerializeField]
    Sprite[]                            mNPC_sprites = null;
    [SerializeField]
    Sprite[]                            mNPC_fadeSprites = null;
    [SerializeField]
    Sprite[]                            mPanelSprites = null;
    [SerializeField]
    Sprite[]                            mCloseUpPanelSprites = null;

    StoryBlock[]                        mStoryBlocks;
    StoryBlock[]                        mSelectionStoryBlocks;
    Selection[]                         mSelections;
    SysKey                              mPastSysKey = null;
    StoryNPC                            mLastNPC = null;
    StoryNPC                            mLastPanelNPC = null;
    int                                 mChapterIndex = -1;
    int                                 mInitBlockIndex = -1;
    int                                 mBlockIndex = 0;
    int                                 mBlockInnerIndex = 0;
    int                                 mSelectionBlockIndex = 0;
    int                                 mSelectionBlockInnerIndex = 0;
    int                                 mSelectingIndex = -1;
    int                                 mSelectedNum = -1;
    bool                                mActive = false;
    bool                                mFlipping = false;

    static public Button[]              TwoSelectingButtons => Inst.mTwoSelectingButtons;
    static public Button[]              ThreeSelectingButtons => Inst.mThreeSelectingButtons;
    static public Text                  Text => Inst.mText;
    static public Text                  NPC_Name => Inst.mNPC_name;
    static public Image                 NPC_Panel => Inst.mNPC_panel;
    static public Image                 Panel => Inst.mPanel;
    static public Sprite[]              NPC_Sprites => Inst.mNPC_sprites;
    static public Sprite[]              PanelSprites => Inst.mPanelSprites;
    
    static public int BlockIndex {
        set {
            StoryBlock[] blocks;

            if (Inst.mSelectingIndex == -1) {
                Inst.mBlockIndex = value;
                blocks = Inst.mStoryBlocks;
            }
            else {
                Inst.mSelectionBlockIndex = value;
                blocks = Inst.mSelectionStoryBlocks;
			}
            
            Inst.mPastSysKey = blocks[value].StoryChains[0].sysKey;
            Inst.mLastNPC = blocks[value].StoryChains[0].NPC;
            Inst.mLastPanelNPC = blocks[value].StoryChains[0].NPC.markingNum == 0 ? blocks[value].StoryChains[0].NPC : default;
            Inst.mText.text = blocks[value].StoryChains[0].text;
            Inst.mNPC_name.text = blocks[value].StoryChains[0].NPC.name;

            if (blocks[value].StoryChains[0].NPC.spriteNum != -1)
                Inst.mNPC_panel.sprite = Inst.mNPC_fadeSprites[blocks[value].StoryChains[0].NPC.spriteNum];
            else
                Inst.mNPC_panel.sprite = null;
            if (blocks[value].StoryChains[0].sysKey.panelSpriteNum != -1)
                Inst.mPanel.sprite = Inst.mPanelSprites[blocks[value].StoryChains[0].sysKey.panelSpriteNum];
            if (blocks[value].StoryChains[0].sysKey.closeUpPanelSpriteNum != -1)
                Inst.mCloseUpPanel.sprite = Inst.mCloseUpPanelSprites[blocks[value].StoryChains[0].sysKey.closeUpPanelSpriteNum];
        }
    }

    static public bool Active {
        get { return Inst.mActive; }
        set {
            Inst.mActive = value;
            if (value)
                Inst.StartCoroutine(Inst.FlipUp());
            else
                Inst.StartCoroutine(Inst.FlipDown());
        }
    }

    static public void ActiveOnlyPanels() {
        Inst.StartCoroutine(Inst.FlipUp());
    }

    private void Awake() => Inst = this;

    private void Start() {
        mCanvas.DOFade(0, 0);

        LoadStoryBlock(2);
        LoadSelections(2);
        SoundMngScript.LoadSound(2);
    }

    private void Update() {
        if (!mActive)
            return;

        // 다음 스토리 체인으로 넘어간다. 마지막 체인에 도달하면 비활성화된다
        if (Input.GetButtonDown("Jump") && !mFlipping)
            StartCoroutine(Flip());
    }

    static public void LoadStoryBlock(int _chapterIndex) {
        // 주연 NPC
        StoryNPC    Jian = new StoryNPC("지안", "Stop", "Stop", -1, 1);
        StoryNPC    GyulNormal = new StoryNPC("고결","Gyul Eye Normal", "Gyul Mouth Normal", 0, 0);
        StoryNPC    YugyumNormal = new StoryNPC("유겸", "Gyul Eye Normal", "Gyul Mouth Normal", 0, 0); //// 2
        StoryNPC    NayeonNormal = new StoryNPC("나연", "Gyul Eye Normal", "Gyul Mouth Normal", 0, 0); //// 4

        // 조연 및 기다 NPC
        StoryNPC    UnknownNPC = new StoryNPC("???", "Stop", "Stop", -1, 1);
        StoryNPC    TransparentNPC = new StoryNPC("", "Stop", "Stop", -1, 1);
        StoryNPC    ShipMaster = new StoryNPC("선장", "Stop", "Stop", 5, 2);
        StoryNPC    FemaleVillager = new StoryNPC("여자 주민 1", "Stop", "Stop", 6, 2);
        StoryNPC    VillageMaster = new StoryNPC("이장", "Stop", "Stop", 5, 2);

        // Panel Sprite
        // 0. 창틈으로 들어오는 달
        // 1. 구두 신은 남자의 발
        // 2. 꽃을 쥔 어린 아이 손
        // 3. 짖밟힌 들꽃
        // 4. 흐릿하게 나무가 우거진 숲
        // 5. 바다
        // 6. 해록도 전경
        // 7. 해록도 거리(낮)
        // 8. 1-1 고결
        // 9. 마을 회관
        // 10. 나연의 집 외경
        // 11. 1-1 나연
        // 12. 나연의 방
        // 13. 어두운 배경
        // 14. 선착장
        // 15. 등대(낮)
        // 16. 숲속(낮)
        // 17. 숲속(저녁)
        // 18. 밝은 배경

        // Close Up Panel Sprite
        // 0. 신호가 안 잡히는 핸드폰

        // BGM
        // 0. 타이틀 BG
        // 2. 1장 메인 BG
        // 서브 BGM
        // 1. 캐리어 끄는 소리
        // 3. 파도 소리, 갈매기 소리

        // 이펙트 사운드
        // 0. 사진 찍는 소리
        // 1. 문 두드리는 소리
        // 2. 놀라는 연출 소리
        // 3. 문 열리는 소리
        // 4. 이불 걷히는 소리
        // 5. 부스럭 거리는 소리

        SysKey      defaultSysKey = new SysKey();
        SysKey      sysKey = new SysKey();

        switch (_chapterIndex) {
			// 1-0
			#region
			case 0:
                Inst.mStoryBlocks = new StoryBlock[1];
                Inst.MakeStoryBlock(0, 44);
				// 창틈으로 들어오는 달
				sysKey.key = "No Text";
				sysKey.panelSpriteNum = 0;
				sysKey.fade = true;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				// 구두 신은 남자의 발
				sysKey = new SysKey(sysKey);
				sysKey.panelSpriteNum = 1;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				// 꽃을 쥔 어린 아이 손
				sysKey = new SysKey(sysKey);
				sysKey.effectKey = "Double Fade";
				sysKey.panelSpriteNum = 2;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				// 짖밟힌 들꽃
				sysKey = new SysKey(sysKey);
				sysKey.effectKey = "";
				sysKey.panelSpriteNum = 3;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				// 흐릿하게 나무가 우거진 숲
				sysKey = new SysKey(sysKey);
				sysKey.effectKey = "Shake";
				sysKey.panelSpriteNum = 4;
				sysKey.BGM_SubNum = 1;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				// 어린 우준의 독백
				sysKey = new SysKey(defaultSysKey);
				sysKey.fade = true;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "괜찮아..", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "우린, 꼭 다시 만날 거야..");
				// 여주와 선장의 대화
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 5;
				sysKey.BGM_Num = 2;
				sysKey.BGM_SubNum = 3;
				sysKey.fade = true;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "우욱! 우웨엑!", Jian);
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아가씨 괜찮아요? 멀미약은 먹은 거야?", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "우웩! 아니요... 제가 배를 처음 타 봐서 멀미가 있는 줄 몰랐어요...", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "(으으... 머리가 깨질 것 같아..! 나한테 뱃멀미가 있었나?)");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "(출항한 지 5분도 안 돼서 속은 울렁거리지 머리는 아프지...)");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "웅크리고 앉아 비닐봉지에 얼굴을 묻었다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "배에 토하면 큰일 나! 아가씨가 다 치우고 내려야 돼!", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "네, 걱정 마시.. 욱...!", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아이고, 이거라도 먹어, 멀미약이야. 도착하려면 1시간은 더 걸리니까 지금이라도 먹는 게 낫겠어.", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "네 감사합니다.", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				sysKey.fade = true;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "약을 먹고 의자 깊숙히 몸을 늘어뜨렸다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "(20분쯤 지났나, 이제 좀 살 것 같네.)", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "선장님, 섬이 정말 머네요? 한번 들어가면 나오는 것도 일이겠어요.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그래서 해록도 사람들은 육지로 잘 안가.", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나야 일주일에 한번 씩 벌써 5년 째 다니고 있으니까 익숙한데 어업도 금지고 파도도 험해서 물자선 말고는 섬에 오가는 배가 없어.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그럼 해록도 주민들은 어떻게 육지로 가는데요?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "거기야 뭐 주민들 사용하는 배가 있기는 한데 이장님이랑 젊은이 몇 명 빼고는 육지로 잘 안 나가.", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "오래전에 불법어업 하는 사람들이 해록도에서 소동을 많이 일으켰나봐, 그 뒤로 주민들이 외부인 들어오는 걸 싫어해.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "모르는 배가 정박하려고만 하면 벌떼 같이 몰려와서 쫒아낸다니까?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "되게 폐쇄적이네요.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "말도 마, 10년 째 보는 나랑도 대화를 잘 안 해. 그래서 그런가 주민들끼리는 사이가 돈독해 보이더구만.", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그럼 주민이 아니면 해록도에 오래 머물긴 힘들겠어요.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그렇지, 아니 그러고 보니까 신기한 일이네?", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "외부인한테 출입허가를 잘 안 해주는데 아가씨는 무슨 일로 허가를 해줬대.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도 주민 중에 가족이 있어?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "글쎄요, 저도 있는지 없는지 확인해보러 가는 거예요.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "응? 그게 무슨 말이야?", ShipMaster);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "헤헤 아무것도 아니에요.", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "저희 삼촌이 오랫동안 해록도를 관리하는 담당 공무원이셨거든요.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "10년 넘게 해록도에 방문하셔서 주민들과 친하셨을 거예요.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "제가 출입허가서에 삼촌 조카인 걸 적었는데, 아마 그래서 허가 해주신 게 아닐까요?");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 주민 조사하러 가는 공무원은 적어도 일주일은 해록도에 머무니까 말이야", ShipMaster);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어쩌면 다른 외부인들이랑 달리 주민들이랑 친했을 수도 있겠네.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도에 대해 선장님과 이런저런 얘기를 나누다 보니 어느새 저 멀리 삐죽 솟은 외딴 섬 하나가 보였다.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.panelSpriteNum = 6;
                sysKey.BGM_Num = -2;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "우와 진짜 예뻐요! 푸른 바다, 푸른 하늘 그리고 녹색 섬이라니 그림 같은데요?", Jian);
                // 해록도 도착
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "곧바로 핸드폰을 꺼내 섬을 촬영했다");
                sysKey = new SysKey(defaultSysKey);
                sysKey.effectNum = 0;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "드디어 도착했다, 내가 태어난 곳");

                StartStoryBlock(0);
                break;
			#endregion
			// 1-1
			#region
			case 1:
                Inst.mStoryBlocks = new StoryBlock[1];
                Inst.MakeStoryBlock(0, 133);
				// 섬에 도착한 지안
				sysKey.panelSpriteNum = 7;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아니~ 무슨 무인도도 아니고 어떻게 개미 한 마리도 안 보이냐고오~!", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "20분 째 무거운 캐리어를 끌고 섬을 돌아다녔지만 주민들의 머리카락 조차 보이지 않는다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아무리 작은 섬이라지만 어쩜 이렇게 휑할 수 있지?");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어? 집! 집이다!", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.effectNum = 1;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "계세요? 말씀 좀 여쭤보려고요!");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "안 쪽에선 아무 소리도 들리지 않는다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자급자족 하는 섬이라고 들었는데 다들 일하러 갔나? 하긴 농사는 새벽부터 하니까.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아이참, 그래도 이건 너무 심한 거 아니야?", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "지도라도 찾아보기 위해 휴대폰을 켰지만 인터넷이 먹통이었다.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.closeUpPanelSpriteNum = 0;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "으악, 심지어 핸드폰도 안 터지잖아!", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.closeUpPanelSpriteNum = -2;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "진짜 큰일이네 누가 있어야 뭘 물어보든 말든 할 거 아니야… 다리도 아프고 짐도 무겁고...");
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 0;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				// 고결과의 첫 만남
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "좋아, 이제 남은 방법은 하나뿐이야.", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "하압-! 숨을 최대한 깊게 들이마셨다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "저기요오오!!! 아무도오오!!! 없어요오오!!!", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "도와주세요오오오!!!!!!");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "귀청 떨어지겠네.", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				sysKey.effectKey = "Shake";
				sysKey.effectNum = 2;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "으악!", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "갑자기 들려오는 목소리에 너무 놀라 자리에 주저앉아 버렸다");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뭐야 불러놓고 왜 놀라?", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "소리가 들리는 쪽으로 고개를 들었지만 올려보고 올려봐도 얼굴이 보이지 않았다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "무슨 키가 이렇게 커?");
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 8;
				sysKey.standingOutNum = 0;
				TransparentNPC.name = "고결";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누구...세요?", Jian);
				StoryNPC GyulUnknown = new StoryNPC(GyulNormal);
				GyulUnknown.name = "???";
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 7;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그쪽은 누군데 그렇게 해록도 떠나가라 소리를 지르고 있어?", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "제가 먼저 물어봤잖아요", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자리에서 일어나 눈을 흘겨 쳐다봤다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어라? 잘생겼잖아?");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "잘생긴거 아니까 너무 그렇게 뚫어져라 보지는 말고.", GyulUnknown);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "재수도 없고…");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "재수 없다고 생각하는 중이면 그것도 맞으니까 그만 쳐다보지 그래?", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "하하.. 혹시 여기 주민이세요? 제가 방금 해록도에 들어왔는데 여쭤볼게 있어서요.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "주민은 아니고 주민 조사하러 나온 공무원이야.", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "여기 외지인은 안 받는다는데 그쪽이야 말로 누구?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그냥 휴양하러 온 관광객인데요.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "특이하네 무슨 이런 오지에 관광을 와?", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 아무튼 며칠 묵을 곳을 찾고 있어요. 여기 여관 같은 곳이 있을까요?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "딱 봐도 없어보이지 않아? 뭐, 다 쓰러져가는 마을회관이 있긴 하지만.", GyulUnknown);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "마을회관이라니, 부탁만 잘하면 머물게 해줄 수도 있겠다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "근데 거기까진 또 어떻게 가지?");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "저 혹시… 마을회관이 어느 쪽에 있나요?", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "남자는 턱 짓으로 앞을 가리키며 말했다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "쭉 직진하다보면 길 끝에 건물 하나 있어.", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "감사합니다.", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "잘 모르겠지말 일단 가다보면 나오겠지.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "인사를 하고 몸을 돌려 남자가 알려준 방향으로 걷기 시작했다.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.standingOutNum = 0;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "...");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "근데…");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뭐지, 자꾸 뒤에서 들려오는 발 소리는…?");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "왜… 왜 자꾸 따라오세요?", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나는 걸음을 멈추고 남자를 향해 물었다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "너 안 따라가는데?", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그럼 어디가시는 데요?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "마을회관.", GyulUnknown);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "으, 따라오시는 거 맞잖아요!", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "내가 널 왜 따라가, 나도 마을회관에 머물고 있거든?", GyulUnknown);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "남자는 어이없다는 듯 웃으며 말했다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뭐야 진작 말하던가.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "다시 뒤돌아 회관으로 가려다 잠시 캐리어를 끌던 손을 주물렀다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "비포장 된 길에서 한참을 돌아다녀 그런지 손목이 시큰했다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "가지가지 하네 정말.", GyulUnknown);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "남자는 나를 흘겨보니 내 캐리어를 낚아채더니 앞장 서 걸었다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "감사합니다! 제가 들어도 되는데….", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "감사하면 빨리 빨리 따라와.", GyulUnknown);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뭐지, 이 친절한 듯 불친절한 언사는?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 거의 뛰다시피 걸으며 그와 속도를 맞췄다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그….  성함이 어떻게 되세요?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "고결, 얼굴이랑 잘 어울리는 이름이지?", GyulNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 순간 할 말을 찾지 못해 입을 꾹 닫고 말을 골랐다.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 1;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "근데 정말 여긴 왜 왔어? 보통 여자 혼자 이런 아무 것도 없는 섬엔 안오지않나?", GyulNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "관광왔다니까요 저 대학 막 졸업했거든요. 졸업여행 삼아 온거에요.", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "고결은 별로 내 말을 믿는 눈치가 아니었다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "시시콜콜한 대화를 하며 10분 정도 걸었을까.");
				// 마을 회관 도착
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "No Text";
				sysKey.message = "<system : 장소를 발견했습니다! '마을회관'>";
				sysKey.panelSpriteNum = 9;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "다 왔어.", GyulNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "네 감사합니다.", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "곧 주민들이 올테니까 아무나 붙잡고 도와달라고 해.", GyulNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "얼마나 머물 생각이야?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "한 달 정도 있을 것 같아요.", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그는 거만하게 웃으며 말했다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아는 지 모르겠지만 해록도 사람들은 외지인에게 친절한 편이 아니야. 아마 너한테 제일 친절한 사람은 나일걸?", GyulNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아하... 네...", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "감사하지?", GyulNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 2;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "윽, 진짜…! 참자 참아!", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "결의 말에 대꾸하는 대신 눈으로 그를 흘겨봤다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아! 그나저나 여기가 마을의 중심인 것 같은데 여기라면 핸드폰이 터지지 않을까?!");
				sysKey = new SysKey(sysKey);
				sysKey.closeUpPanelSpriteNum = 0;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "서둘러 핸드폰을 켜봤지만 기대완 달리 여전히 핸드폰은 먹통이었다.");
				sysKey = new SysKey(sysKey);
				sysKey.closeUpPanelSpriteNum = -2;
				sysKey.effectNum = 3;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그때, 누군가 회관문을 열고 들어왔다");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "에그머니!", FemaleVillager);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "날 보자마자 소스라치게 놀라며 뒷걸음질 친 아주머니는 황급히 문을 닫고 회관을 나갔다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "봐봐, 여기 사람들은 외지인만 보면 꼭 귀신이라도 본 것처럼 저런다니까.", GyulNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 처음에 날 보고 감탄해서 그런 줄 알았어.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "대답할 가치가 없는 말 같으니 아무 말도 하지말자.");
				sysKey = new SysKey(sysKey);
                sysKey.effectNum = 3;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "곧 다시 문이 열리고 방금 나간 아주머니와 또 다른 아저씨 한 명이 함께 들어왔다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아저씨는 나를 보며 어색한 미소로 말을 건냈다.");
                sysKey = new SysKey(defaultSysKey);
                StoryNPC VillageMasterUnknown = new StoryNPC(VillageMaster);
                VillageMasterUnknown.name = "아저씨";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "혹시 해록도에 관광오기로 한 아가씨인가?", VillageMasterUnknown);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 네 안녕하세요. 저 출입허가서도 받았어요! 이상한 애 아니에요!", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "주민들 눈에 경계심이 어려있어 다급히 입꼬리를 최대한 끌어올려 빠르게 말했다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아~ 그 아가씨구나, 관광 온다는 얘기 들었어요~ 나는 또 처음보는 사람이라 놀랐지 뭐야.", FemaleVillager);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도가 원채 오가는 사람도 없고 주민들도 적어서 누가 들어온다고 하면 우리끼리 다 알거든.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "경계가 풀렸는지 아주머니는 인자하게 웃으며 다가왔다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "여기가 다 흙길이라서 가방 끌고 오기 힘들었을 텐데 선착장에 조금만 있지 그랬어.", VillageMasterUnknown);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아가씨 오늘 온다는 얘기 듣고 지금 다른 주민이 선착장에 마중나갔는데.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "정말요? 제가 배에서 내리자마자 돌아다녀서 길이 엇갈렸나봐요. 마중까지 나와주시고 정말 감사합니다.", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뒤에서 시선이 느껴져 돌아보니 결이 묘한 눈으로 주민들을 빤히 쳐다보고 있었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "왜 저렇게 쳐다보지??");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "여기 얼마나 있을 거에요? 해록도가 그 뭐냐, 무슨 보호? 생태보호? 그걸로 지정 돼 있는 건 알죠?", FemaleVillager);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나무들이 건강해서 공기도 좋고 해안가 경치도 정말 좋아요.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "오래오래 있으면 좋겠다, 오랜만에 외지인 보니까 정말 반갑네~");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그러게 말이야 오랜만에 온 귀한 손님이네 귀한 손님.", VillageMasterUnknown);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "머물 곳은 우리가 다 마련해 놨으니까 따라와요.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아저씨를 따라 회관을 나서려다 뒤를 돌아 고결에게 말했다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "데려다 주셔서 감사합니다 또 뵈어요!", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "결은 씨익 웃으며 대답을 대신했다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어? 그런데 이상하다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도 주민들은 외지인을 경계한다고 하지 않았나?");
                // 나연의 집 도착
                sysKey = new SysKey(sysKey);
                sysKey.panelSpriteNum = 10;
                sysKey.standingOutNum = 0;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아저씨는 나를 데려다주는 길에 해록도에 관한 몇 가지 이야기와 본인이 해록도의 이장이라는 것을 알려줬다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "회관을 나와 15분 쯤 걸었을까, 아저씨와 함께 도착한 곳은 소담스런 2층짜리 주택이었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "지나오면서 본 해록도의 집 중에 제일 큰 것 같은데, 어딘지 모르게 쓸쓸해 보이는 집이었다.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.effectNum = 1;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나연 자매 있어? 손님 왔어!", VillageMaster);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "잠시 뒤 문이 열리며 앳되보이는 여자아이가 고개를 빼꼼 내밀었다.");
                sysKey = new SysKey(sysKey);
                sysKey.panelSpriteNum = 11;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "여자애는 인사도 없이 무표정한 얼굴로 가만히 나를 관찰하듯 쳐다볼 뿐이었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아니 여긴 왜 나를 자꾸 빤히 쳐다보는 사람 밖에 없지? 외지인이 드물다더니, 내가 신기한가?");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "안녕하세요, 김지안이라고 해요!", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도에서 지내는 동안 이 집에 머물면 돼요. 나연 자매 혼자 사는 집이라서 불편하진 않을 거야.", VillageMaster);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나연은 따라오라며 고개짓을 한 뒤 성큼 성큼 집 안으로 들어갔다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나는 나연의 뒤를 쫓으며 이장님을 향해 고개를 꾸벅였다.");
                sysKey = new SysKey(defaultSysKey);
                sysKey.panelSpriteNum = 12;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "여기 쓰면 돼.", NayeonNormal);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나연은 2층에 있는 방까지 날 안내한 뒤 무심히 말한채 대답도 듣지 않고 방 밖으로 사라졌다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "깔끔하게 정돈된 방이었다. 본래는 숙박용이 아니라 개인이 쓰던 방인 듯했다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "벽지는 과거에 있었던 가구 모양에 맞추어 바래있었고, 나무를 깎아 만든 십자가가 벽에 걸려 있었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그걸 보자 급격히 피곤이 몰려와 침대에 누웠다. 생각했던 것보다 푹신해서 몸이 나른해졌다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 이대로 잠들어버릴지도 모르겠어…");

                StartStoryBlock(0);
                break;
            #endregion
            // 1-2
            #region
            case 2:
                Inst.mStoryBlocks = new StoryBlock[1];
                Inst.MakeStoryBlock(0, 128);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "....어나...", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누가 나를 부르고 있다. 누구지? 알람인가....");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "일어나고 싶은데 몸이 무거워 움직일 수가 없다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "...나라고!", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 12;
				sysKey.effectNum = 4;
				sysKey.fade = true;
				StoryNPC InvisibleNayeon = new StoryNPC("???", "Stop", "Stop", -1, 1);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "진짜 귀찮게 한다, 너.", InvisibleNayeon);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "익숙한 목소리에 고개를 돌리니...");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "벌써 해가 중천인데 아직도 자고 있으면 어떡해? 여기 요양왔어?", NayeonNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "놀라서 시계를 바라보자, 오후 12시가 넘어가고 있었다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "헉! 거의 12시간을 잤네… 깨워줘서 고마워!", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "깨워줘서 고마워? 참 나. 그 쪽이 예뻐서 깨운 거 아니거든?", NayeonNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "밖에 손님왔어! 신경쓰여 죽겠으니까 빨리 나가.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "응? 내 손님?", Jian);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누구지. 어제 만난 그 남자인가, 아니면 이장님?");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나는 긴가민가해하며 자리에서 일어났다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "야.", NayeonNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "응?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나 너 좋아서 우리집에 머물게 한 거 아니고 어쩔 수 없이 떠맡은 거야.", NayeonNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그러니까 거슬리는 행동하지 말고 최대한 조용히 있다가 떠나, 나한테나 다른 주민들한테나.");
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나연은 차갑게 쏘아 붙인채 부셔져라 문을 닫고 나갔다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아니, 나도 꼭 여기 머물고 싶어서 온 건 아니거든? 정말 어이없어....");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "일단, 씻고 얼른 나가보자.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아침부터 찾아온 손님이 대체 누굴까? 해록도에는 나를 찾는 사람이 없을텐데...");
				sysKey = new SysKey(defaultSysKey);
				sysKey.fade = true;
				sysKey.standingOutNum = 0; //// 나연
				sysKey.panelSpriteNum = 13;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "지안아. 이제부터 좋은 것만 보고, 좋은 것만 듣고, 좋은 것만 기억하면서 사는 거야.", UnknownNPC);
				sysKey = new SysKey(defaultSysKey);
				sysKey.fade = false;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "기억이란 생각보다 단순해서 내 입맛대로 보관할 수가 있거든.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그야말로 신이 내린 선물이지!");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "뭐? 거짓말 말라고? 넌 쪼끄만 게 벌써부터 어른을 의심하니.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "No Text";
				sysKey.panelSpriteNum = 18;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "문을 열자마자 쏟아지는 햇살에 눈이 부시다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그리고 그 사이로 보이는 건....");
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 7;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "기다렸어요, 누나. 잘 잤어요?", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "설마 그쪽이… 제 손님이세요...?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "하핫. 반가워요! 육지에서 예쁜 누나가 왔다길래 얼른 보러 왔지요!", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아무리 작은 섬이라도 그렇지, 여기 온 지 하루만에 나에 대한 소문이 다 퍼진걸까?");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "근데... 역시 소문은 믿을 게 못 되네요?", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그는 웃음기를 싹 뺀, 진지한 얼굴로 말했다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "소문보다 더더더, 훨씬 더 예쁘잖아요?!", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누나 이름이 뭐예요?");
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 0;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "...", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "하하하. 지금 정색한 거에요? 누나 반응 진짜 재밌네요.", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 1;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "전 홍유겸! 이예요. 스무살이니까 말 편하게 해요. 누나!", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자기가 스무 살인건 그렇다쳐도 내 나이도 모르면서 왜 누나라고 확신하는 거지, 이 자식...");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "조금 별나긴 하지만… 나쁜 사람은 아닌 것 같아.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이건 절대 겉모습만 보고 판단한 게 아니다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "웃는 게 조금 귀엽다고 방심한 게 아니라는 말이다. 진짜로.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그...래, 그나저나 나는 왜 찾아온 거야?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도에 처음 왔으니, 섬 구경을 해야하지 않겠어요?", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "사실 제가 해록도의 비공식 마스코트를 맡고 있거든요.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도하면 유겸이, 유겸이하면 귀여워... 귀여우면 홍유겸...");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그.래.서?", Jian);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "멍하니 듣고 있다가는 해가 질 것 같다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그러니까! 이 유겸이가 지안 누나만의 특별 가이드를 해주려 왔다는 거죠.", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "가이드?", Jian);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "도시에 비해선 작지만, 해록도의 지형이 은근 헷갈리거든요!", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "주민이 아니면 길을 잃기 쉬워요.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "섬을 안내 해주겠다는 그의 말에 나는 잠시 고민했다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "안 그래도 오늘부터 해록도를 탐방하려고 했다.");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "물론 나는 태생부터 길치라 조금 막막했지만….");
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어떡할까?");
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 2;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				sysKey.panelSpriteNum = 7;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누나가 묵고 있는 곳에서 조금만 더 나오면 바로 큰 길가에요.", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸은 새하얀 손가락으로 이곳 저곳을 가리켰다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이쪽으로 쭉 내려가면 마을 회관이 나오고, 반대편으로 가면 이장님 댁이에요.", YugyumNormal);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "다들 마을 회관에 자주 모이니 필요한게 생기거나 궁금한게 있을 땐 그쪽으로 가면 돼요.");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 그가 가리키는 곳들을 열심히 메모했다.");
				sysKey = new SysKey(defaultSysKey);
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "흠, 이제 어디 먼저 가볼래요?", YugyumNormal);
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 3;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
				sysKey.key = "Naration";
				sysKey.panelSpriteNum = 6;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자 다음은 어디로 가볼까?");
				sysKey = new SysKey(defaultSysKey);
				sysKey.selectionIndex = 4;
				Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
				sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자 다음은 어디로 가볼까?");
                sysKey = new SysKey(defaultSysKey);
                sysKey.selectionIndex = 5;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "으흠. 공기 좋다.", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "섬에 이런 숲 길이 있었구나. 낮에는 산책하기에도 딱 좋겠어!");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "누나... 보기보다 액티비티를 좋아하나봐요. 이런 산에서 산책이라니... 전 여기 오르내릴때마다 힘들어 죽겠던데.", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸이 넌 보기보다 연약하구나. 길도 고르고 별로 경사도 없는데, 엄살은...", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "내가 혀를 끌끌 차자 유겸은 못 들은 척 고개를 돌린다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "근데, 이 위로 쭉 올라가면 뭐가 있는데?", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "선락원이요, 그냥 교회인데 저희 마을에선 선락원이라고 부르고 있어요.", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "무슨 교회가 산 중턱에 있어?", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그곳이 해록도의 중심이거든요.", YugyumNormal);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그때, 맞은 편에서 이장님과 주민 한 분이 내려오는 게 보였다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어어, 유겸이~", VillageMaster);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이쪽은 그 해록도 관광왔다는 아가씨야? 세상에 얼굴 하얀 것 봐. 너무 곱다.", FemaleVillager);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 안녕하세요.", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "반가워요. 근데 어디가는 길이야?", FemaleVillager);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "제가 또 한 친절 하잖아요? 육지에서 온 귀한 손님에게 섬을 안내해주고 있었지요.", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아가씨가 잘 만났네! 우리 유겸이가 얼마나 해록도에 빠삭한 지.", VillageMaster);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이보다 더 유능한 가이드는 없을거야.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "해록도만 잘 알게? 요리면 요리, 바느질이면 바느질. 못하는 게 없다니까.", FemaleVillager);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아주 해록도 주민들에게 사랑과 관심을 듬뿍 받고 있나보네?");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "에헤이. 지금 외부인 앞이라고 저 띄워주시는 거죠? 사양말고 더 띄워줘도 돼요. 엣헴.", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아, 요리하니까 생각났는데.", FemaleVillager);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸이가 알려준 레시피대로 빵을 구웠는데도 자꾸 실패해!");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "왜 그럴까, 으응? 우리 집에 와서 한번만 더 알려줘~");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이러다 남은 재료들마저 다 버리겠어.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "에이 정말! 제가 하란대로 안 한거 아니에요? 진짜 쉬운데...", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "말도 마. 계속 타고, 터지고 난리도 아니더라니까.", FemaleVillager);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "주민의 하소연에 유겸은 제 머리를 긁적이며 내 눈치를 봤다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "흐음... 오늘은 지안 누나를 위해 하루를 비우기로 했으니까, 그럼 내일...", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 괜찮아! 웬만한 곳은 다 둘러봤고.", Jian);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이왕 여기까지 온 거 산책 좀 하다가 돌아가려고.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "흐음… 알겠어요!", YugyumNormal);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸은 아쉬운 듯했지만, 이내 주민과 자연스럽게 팔짱을 꼈다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "자자. 우리 누님은 유겸이의 베이킹 교실로 가보자고요.", YugyumNormal);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "그럼 지안씨도 조금만 둘러보다 해 지기 전에 어서 들어가요.", VillageMaster);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "네네. 다음에 뵈어요.", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                sysKey.standingOutNum = 4;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "모두가 내려가자 순식간에 고요해졌다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "생태보호구역이라더니 정말 관리가 잘 되어 있네.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "휴대폰만 잘 터졌다면 더 좋았을 걸.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "난 여전히 먹통인 휴대폰의 화면을 두드리며 걸었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸이 이쪽으로 쭉 올라가면 선락원이랬는데.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "잠깐, 이 방향이 맞나?");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "몸을 틀어보았다. 숲이라서 동서남북의 풍경이 다 비슷비슷했다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "다시 말하지만 나는 길치다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나는 올라왔던 방향을 되새기며, 다시 내려가기로 했다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "내려가는 건 쉽지. 그냥 아래로만 쭉 가면 되는 거잖아?");
                sysKey = new SysKey(defaultSysKey);
                sysKey.panelSpriteNum = 17;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어째 점점 더 깊숙한 곳으로 들어가는 것 같은데, 기분 탓인가.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "아...피곤해. 배도 고프고. 산만 타다가 날 새겠네.", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                sysKey.effectNum = 5;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어? 이게 무슨 소리지?");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나는 걸음을 멈추고 주위를 둘러봤다.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "유겸이?", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "저 멀리 그토록 찾아 헤매던 교회의 십자가가 보였으나,");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이젠 그게 문제가 아니었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "등 뒤로 알 수 없는 오한이 들었다.");
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "섬뜩한 기운이 느껴지는 곳을 향해 천천히 고개를 돌리는 순간.");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "......?", Jian);
                sysKey = new SysKey(defaultSysKey);
                sysKey.key = "Naration";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "나뭇잎 더미 사이로 새빨간 무언가가 비쳤다.");

                StartStoryBlock(0);
                break;
            #endregion
                // 밝아지는 효과 테스트
            case -1:
                Inst.mStoryBlocks = new StoryBlock[1];
                Inst.MakeStoryBlock(0, 3);
                sysKey = new SysKey(defaultSysKey);
                sysKey.panelSpriteNum = 12;
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "이번에는 밝아지는 효과를 테스트 할거야.", GyulNormal);
                sysKey = new SysKey(defaultSysKey);
                sysKey.effectKey = "Brightness";
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "화아악..!");
                sysKey = new SysKey(defaultSysKey);
                Inst.MakeChain(Inst.mStoryBlocks[0], sysKey, "어때. 고칠 부분이 있나? 있으면 말해줘.");

                StartStoryBlock(0);
                break;
		}

		Inst.mChapterIndex = _chapterIndex;
    }

    static public void LoadSelections(int _chapterIndex) {
        switch (_chapterIndex) {
            case 1:
                Inst.mSelections = new Selection[3];
                Inst.mSelections[0] = new Selection(
                    "사람이 올 때까지 가만히 기다린다.",
                    "일단 계속 돌아다녀본다."
                    );
                Inst.mSelections[1] = new Selection(
                    "네에… 정말 어울리네요…",
                    "자기애가 되게 강하신가봐요…?"
                    );
                Inst.mSelections[2] = new Selection(
                    "….네에… 감사합니다.",
                    "근데 왜 자꾸 반말을 하세요?"
                    );
                break;
            case 2:
                Inst.mSelections = new Selection[6];
                Inst.mSelections[0] = new Selection(
                    "얼버무린다.",
                    "알려준다."
                    );
                Inst.mSelections[1] = new Selection(
                    "대체 누구세요?",
                    "저한테 왜 이러세요?"
                    );
                Inst.mSelections[2] = new Selection(
                    "어색한 건 딱 질색. 혼자 다닌다.",
                    "의외로 좋은 기회일지도. 같이 다닌다."
                    );
                Inst.mSelections[3] = new Selection(
                    "선착장",
                    "등대",
                    "아니 됐어, 그냥 산책이나 하자."
                    ).ButtonLock(false, true, true);
                Inst.mSelections[4] = new Selection(
                    "선착장",
                    "등대",
                    "아니 됐어, 그냥 산책이나 하자."
                    ).ButtonLock(true, false, true);
                Inst.mSelections[5] = new Selection(
                    "선착장",
                    "등대",
                    "아니 됐어, 그냥 산책이나 하자."
                    ).ButtonLock(true, true, false);
                break;
		}
	}

    static public void LoadSelectedStoryBlock(int _chapterIndex, int _selectingIndex, int _selectedNum) {
        // 주연 NPC
        StoryNPC    Jian = new StoryNPC("지안","Stop", "Stop", -1, 1);
        StoryNPC    GyulNormal = new StoryNPC("고결", "Gyul Eye Normal", "Gyul Mouth Normal", 0, 0);
        StoryNPC    YugyumNormal = new StoryNPC("유겸", "Gyul Eye Normal", "Gyul Mouth Normal", 0, 0); //// 2

        // 조연 및 기다 NPC
        StoryNPC    UnknownNPC = new StoryNPC("???", "Stop", "Stop", -1, 1);
        StoryNPC    ShipMaster = new StoryNPC("선장", "Stop", "Stop", 4, 2);

        SysKey      defaultSysKey = new SysKey();
        SysKey      sysKey = new SysKey();

        switch (_chapterIndex) {
            case 1:
                switch (_selectingIndex) {
                    case 0:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 6);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "일단 누구든 올 때까지 기다려봐야지.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "시계를 보니 벌써 20분이나 흘렀다.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "안되겠다 계속 이대로 있을 순 없어!");
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 6);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "일단 계속 돌아 다녀봐야지.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "하염없이 직진만 한지 20분 째...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "길가엔 사람 그림자도 보이지 않는다.");
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 1:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 5);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "네에… 정말 어울리네요…", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "표정은 아닌데? 재수 없다고 생각했지 너.", GyulNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "아, 아닌데요?", Jian);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "뭐야 어떻게 알았지?!");
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 7);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "자기애가 되게… 강하신가봐요…?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "꽤 들어 그런 말.", GyulNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "뭐지 이 은근하게 느껴지는 재수없음은?");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "재수 없다고 생각했지 너.", GyulNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "오, 들켰다.", Jian);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "장난스레 대답했지만 사실 그의 말마따나 얼굴과 꽤나 잘 어울리는 이름이었다.");
                                Inst.mSelectionBlockIndex = 0;
                                break;
                        }
                        break;
                    case 2:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 3);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "... 네에... 감사합니다.", Jian);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "엎드려 절받는 걸 좋아하는 편인가 보네.");
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 3);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "근데 왜 자꾸 반말을 하세요?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "기분 나쁘면 너도 까던가.", GyulNormal);
                                Inst.mSelectionBlockIndex = 0;
                                break;
                        }
                        break;
				}
                break;
            case 2:
                switch (_selectingIndex) {
                    case 0:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 6);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "....#$*^@...인데요.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "아. 김지안이라고요?", YugyumNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "와. 소름.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "일부러 발음을 잔뜩 뭉갰는데 뭔 놈의 귀가 이렇게 좋아?");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "김지안이라… 진짜 말도 안 돼! 이름까지 잘 어울리면 반칙 아니에요?!", YugyumNormal);
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 3);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "....김지안인데요.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "와... 진짜 말도 안 돼, 이름까지 잘 어울리면 반칙 아니에요?!", YugyumNormal);
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 1:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 2);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그쪽은 대체 누구신데요?", Jian);
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 4);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "저한테 왜 이러세요? 외부인이라고 신고식이라도 하는 건가요?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "하핫. 신고식을 원하면 해줄 수도 있는데. 원하는 컨셉 있어요?", YugyumNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "뭐든지 사양할게요! 대체 누구세요?", Jian);
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 2:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 9);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "역시 처음 보는 사람과 단둘이 다니는 건, 생각만 해도 어색해.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "아니, 괜찮아. 나는 혼자 다니는 게 편해서....", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "앗... 저는 불편해요...?", YugyumNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그가 갑자기 내 소매 끝을 잡는다.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그리고 푹 숙인 고개를 천천히 드는데...");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "아... 저 커다랗고 맑은 눈동자.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, ".....알겠어, 같이 다니자. 길 헤매는 것보단 훨씬 낫겠지.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "탁월한 선택이에요!", YugyumNormal);
                                Inst.mSelectionBlockIndex = 0;
                                break;
                            case 1:
                                LoveMngScript.AddLove("유겸", 20);
                                Inst.StartCoroutine(Inst.FlipUpMessage("유겸의 호감도가 20 상승하였습니다!"));

                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 5);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그래. 도움 받을 수 있는 건 다 받아보자. 어딘가 나사는 빠져있어도 친절한 것 같으니.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "고마워, 시간 괜찮아?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "당연하죠! 오늘 하루는 누나를 위해 비워뒀는걸요?", YugyumNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "저 동글동글한 눈빛을 보고도 차갑게 거절할 수 있는 사람이 어딨겠어?");
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 3:
                        switch (_selectedNum) {
                            case 0:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 14);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "No Text";
                                sysKey.panelSpriteNum = 14;
                                sysKey.message = "<system: 장소를 발견했습니다! '선착장'>";
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "누나도 알다시피 해록도로 오는 배가 별로 없어요.", YugyumNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "일주일에 한 번 물자선이 오는 정도?");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그럼 해록도 사람들은 육지로 잘 안 나가?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "굳이 특별한 일이 없으면 안 나가죠.", YugyumNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "근데 그래도 괜찮아요.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "주민들 중에 육지에서 온 사람들도 있는데 모두 해록도가 훨~씬 살기 좋다고 하더라고요!");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그리고,");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "보다시피 해록도가 너무너무 아름답잖아요?");
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "유겸은 눈을 내리깐 채 조용히 웃으며 말을 이었다.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "한 번 오면... 다신 떠나고 싶지 않을 만큼.", YugyumNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "뭐지. 방금 분위기가 갑자기, 묘하게 어른스러워졌는데?");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "맞아 아름답긴 하더라. 누구든 해록도를 보면 여기 머물고 싶다고 생각할 거야.", Jian);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "나는 어제 배 위에서 처음 본 해록도의 전경을 다시 되뇌었다.");
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 4:
                        switch (_selectedNum) {
                            case 1:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 12);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                sysKey.key = "No Text";
                                sysKey.panelSpriteNum = 15;
                                sysKey.message = "<system: 장소를 발견했습니다! '등대'>";
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "등대 근처에 창고로 보이는 건물이 하나 있지만, 굳게 닫혀 있는 듯 하다.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "여긴 진짜 별게 없어요!", YugyumNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "안에 들어가봤자 계단 뿐이죠.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "예전에는 여기로 불법 어선들이 자주 나타났대요.");
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그래서 주민들이 돌아가면서 등대지기를 하고 있어요.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그렇구나, 너도 해본 적 있어?", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "엑...! 저는 이런 어둡고 음침한 곳과 안 어울리죠....! 그리고 제가 잠도 많은 편이라...", YugyumNormal);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "무서워서 빠졌구나.", Jian);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "당연....이 아니고! 아, 누나!", YugyumNormal);
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "Naration";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "그가 꽥 소리를 지르는 바람에 귀가 멀 뻔 했다.");
                                sysKey = new SysKey(defaultSysKey);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "아무튼 누나도 여기 혼자 오지말아요! 새빨간 데다가 엄청 낡아서 뭔가.... 으스스 하다구요!", YugyumNormal);
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
                    case 5:
                        switch (_selectedNum) {
                            case 2:
                                Inst.mSelectionStoryBlocks = new StoryBlock[1];
                                Inst.MakeStoryBlock(0, 2);
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                sysKey = new SysKey(defaultSysKey);
                                sysKey.key = "No Text";
                                sysKey.panelSpriteNum = 16;
                                sysKey.message = "<system: 장소를 발견했습니다! '숲속'>";
                                Inst.MakeChain(Inst.mSelectionStoryBlocks[0], sysKey, "");
                                Inst.mSelectionBlockIndex = 0;
                                break;
						}
                        break;
				}
                break;
		}

        Inst.StartCoroutine(Inst.Flip());
    }

    static public void StartStoryBlock(int _index) {
        BlockIndex = _index;
        Active = true;
    }

    public void Select(int _selectedNum) {
        mSelectedNum = _selectedNum;
        LoadSelectedStoryBlock(mChapterIndex, mSelectingIndex, _selectedNum);
        StartCoroutine(FlipDownSelection());
    }

    void MakeStoryBlock(int _blockIndex, int _length) {
        if (mSelectingIndex == -1) {
            Inst.mStoryBlocks[_blockIndex] = new StoryBlock();
            Inst.mStoryBlocks[_blockIndex].Length = _length;
            Inst.mStoryBlocks[_blockIndex].StoryChains = new StoryChain[_length];
        }
        else {
            Inst.mSelectionStoryBlocks[_blockIndex] = new StoryBlock();
            Inst.mSelectionStoryBlocks[_blockIndex].Length = _length;
            Inst.mSelectionStoryBlocks[_blockIndex].StoryChains = new StoryChain[_length];
        }

        Inst.mInitBlockIndex = 0;
    }

    void MakeChain(StoryBlock _block, SysKey _sysKey, string _string, StoryNPC _NPC = default) {
        StoryChain chain = new StoryChain();
        chain.sysKey = _sysKey;
        chain.text = _string;
        chain.NPC = _NPC != null ? _NPC : new StoryNPC();
        _block.StoryChains[Inst.mInitBlockIndex] = chain;
        Inst.mInitBlockIndex++;
    }

    IEnumerator FlipUp() {
        mFlipping = true;

        mCanvas.gameObject.SetActive(true);

        if (mStoryBlocks[mBlockIndex].StoryChains[0].sysKey.key != "No Text")
            mCanvas.DOFade(1, Utils.storyBlockFlipTime);
        if (mPanel.sprite)
            mPanel.DOFade(1, Utils.storyBlockFadeTime);
        if (mCloseUpPanel.sprite)
            mCloseUpPanel.DOFade(1, Utils.storyBlockFadeTime);
        if (!mNPC_panel.sprite)
            mCanvas_NPC.DOFade(0, 0);

		yield return new WaitForSeconds(Utils.storyBlockFadeTime);

        StoryBlock[]    blocks = mSelectingIndex == -1 ? mStoryBlocks : mSelectionStoryBlocks;
        int             value = mSelectingIndex == -1 ? mBlockIndex : mSelectionBlockIndex;

        if (blocks[value].StoryChains[0].NPC.spriteNum != -1)
            mNPC_panel.sprite = mNPC_sprites[blocks[value].StoryChains[0].NPC.spriteNum];
        if (mNPC_name.text != null) {
            mNPC_eyeAnimator.SetTrigger(blocks[value].StoryChains[0].NPC.eyeTriggerName);
            mNPC_mouthAnimator.SetTrigger(blocks[value].StoryChains[0].NPC.mouthTriggerName);
        }

        mFlipping = false;
    }

    IEnumerator FlipDown() {
        mFlipping = true;
        
        mCanvas.DOFade(0, Utils.storyBlockFlipTime);

        yield return new WaitForSeconds(Utils.storyBlockFlipTime);

        DOTween.KillAll();
        mCanvas.gameObject.SetActive(false);

        mFlipping = false;
    }

    IEnumerator Flip() {
        SysKey      nextKey;
        string      nextText;
        StoryNPC    nextNPC;

        if (mSelectingIndex == -1) {
            mBlockInnerIndex++;

            if (mBlockInnerIndex == mStoryBlocks[mBlockIndex].Length) {
                if (mLastNPC.markingNum == 0) {
                    mNPC_panel.sprite = mNPC_fadeSprites[mLastNPC.spriteNum];
                    mNPC_eyeAnimator.SetTrigger("Stop");
                    mNPC_mouthAnimator.SetTrigger("Stop");
                }

                mBlockInnerIndex = 0;
                Active = false;
                yield break;
            }

            nextKey = mStoryBlocks[mBlockIndex].StoryChains[mBlockInnerIndex].sysKey;
            nextText = mStoryBlocks[mBlockIndex].StoryChains[mBlockInnerIndex].text;
            nextNPC = mStoryBlocks[mBlockIndex].StoryChains[mBlockInnerIndex].NPC;
        }
        else if (mSelectedNum != -1) {
            mSelectionBlockInnerIndex++;

            if (mSelectionBlockInnerIndex == mSelectionStoryBlocks[mSelectionBlockIndex].Length) {
                mSelectionBlockInnerIndex = 0;
                mSelectingIndex = -1;
                mSelectedNum = -1;
                StartCoroutine(Flip());
                yield break;
            }

            nextKey = mSelectionStoryBlocks[mSelectionBlockIndex].StoryChains[mSelectionBlockInnerIndex].sysKey;
            nextText = mSelectionStoryBlocks[mSelectionBlockIndex].StoryChains[mSelectionBlockInnerIndex].text;
            nextNPC = mSelectionStoryBlocks[mSelectionBlockIndex].StoryChains[mSelectionBlockInnerIndex].NPC;
        }
        else
            yield break;

        if (nextKey.selectionIndex != -1) {
            StartCoroutine(FlipUpSelection(nextKey.selectionIndex));
            yield break;
		}

        if (nextKey.message != null)
            StartCoroutine(FlipUpMessage(nextKey.message));
        else if (mMessageCanvas.alpha != 0)
            StartCoroutine(FlipDownMessage());

        SysKeySameness sameness = new SysKeySameness();
        if (mPastSysKey.key != nextKey.key)
            sameness.sameKey = false;
        if (mPastSysKey.effectKey != nextKey.effectKey)
            sameness.sameEffectKey = false;
        if (mPastSysKey.panelSpriteNum != nextKey.panelSpriteNum)
            sameness.samePanelSpriteNum = false;
        if (mPastSysKey.closeUpPanelSpriteNum != nextKey.closeUpPanelSpriteNum)
            sameness.sameCloseUpPanelSpriteNum = false;
        if (mPastSysKey.BGM_Num != nextKey.BGM_Num)
            sameness.same_BGM_Num = false;
        if (mPastSysKey.BGM_SubNum != nextKey.BGM_SubNum)
            sameness.same_BGM_SubNum = false;

        mFlipping = true;

        mText.DOFade(0, Utils.storyBlockFlipTime);

        if (nextKey.key == "Naration" && !sameness.sameKey) {
            mTextMainPanel.DOFade(0, Utils.storyBlockFlipTime);
            mNPC_name.DOFade(0, Utils.storyBlockFlipTime);
            mSubNPC_panel.DOFade(0, Utils.storyBlockFlipTime);
        }
        else if (nextKey.key == "Face Change") {
            if (nextNPC.eyeTriggerName != null)
                mNPC_eyeAnimator.SetTrigger(nextNPC.eyeTriggerName);
            if (nextNPC.mouthTriggerName != null)
                mNPC_mouthAnimator.SetTrigger(nextNPC.mouthTriggerName);
            if (nextNPC.spriteNum != -1)
                mNPC_panel.sprite = mNPC_sprites[nextNPC.spriteNum];
        }
        else if (nextKey.key == "No Text" && !sameness.sameKey) {
            if (mLastPanelNPC != null && mLastPanelNPC.markingNum == 0) {
                mNPC_panel.sprite = mNPC_fadeSprites[mLastPanelNPC.spriteNum];
                mNPC_eyeAnimator.SetTrigger("Stop");
                mNPC_mouthAnimator.SetTrigger("Stop");
            }
            mCanvas.DOFade(0, Utils.storyBlockFlipTime);
        }
        else if (nextNPC.name != null) {
            if (nextNPC.markingNum == 0 && mLastPanelNPC != null && mLastPanelNPC.spriteNum != nextNPC.spriteNum) {
                mNPC_panel.sprite = mNPC_fadeSprites[mLastPanelNPC.spriteNum];
                mNPC_eyeAnimator.SetTrigger("Stop");
                mNPC_mouthAnimator.SetTrigger("Stop");
                mCanvas_NPC.DOFade(0, Utils.storyBlockFlipTime);
            }
            else if (mPastSysKey.key == "Naration")
                mTextMainPanel.DOFade(0, Utils.storyBlockFlipTime);

            mNPC_name.DOFade(0, Utils.storyBlockFlipTime);
            mSubNPC_panel.DOFade(0, Utils.storyBlockFlipTime);
        }
        if (nextKey.standingOutNum != -1) {
            mNPC_panel.sprite = mNPC_fadeSprites[nextKey.standingOutNum];
            mNPC_eyeAnimator.SetTrigger("Stop");
            mNPC_mouthAnimator.SetTrigger("Stop");
            mCanvas_NPC.DOFade(0, Utils.storyBlockFlipTime);
            mLastPanelNPC = null;
        }

        if (!sameness.samePanelSpriteNum && nextKey.panelSpriteNum != -1 && mPastSysKey.panelSpriteNum != -2)
            mPanel.DOFade(0, Utils.storyBlockFlipTime);
        if (!sameness.sameCloseUpPanelSpriteNum && nextKey.closeUpPanelSpriteNum != -1 && mPastSysKey.closeUpPanelSpriteNum != -2)
            mCloseUpPanel.DOFade(0, Utils.storyBlockFlipTime);
        if (nextKey.fade)
            mFade.DOFade(1, Utils.storyBlockFadeTime);

        yield return new WaitForSeconds(Utils.storyBlockFlipTime);

        mText.text = nextText;
        mText.DOFade(1, Utils.storyBlockFlipTime);

        if (nextKey.key == "Naration" && !sameness.sameKey) {
            mText.color = Color.white;
            mTextMainPanel.sprite = mTextMainPanelSprites[1];
            mTextMainPanel.DOFade(1, Utils.storyBlockFlipTime);
        }

        if (nextNPC.name != null) {
            mNPC_name.text = nextNPC.name;
            mNPC_name.DOFade(1, Utils.storyBlockFlipTime);

            if (nextNPC.markingNum != 2) {
                if(nextNPC.markingNum == 0 && ((mLastPanelNPC != null && mLastPanelNPC.spriteNum != nextNPC.spriteNum) || mLastPanelNPC == null)) {
                    mNPC_panel.sprite = mNPC_fadeSprites[nextNPC.spriteNum];
                    mCanvas_NPC.DOFade(1, Utils.storyBlockFlipTime);
                }
            }
            else {
                mSubNPC_panel.sprite = mNPC_sprites[nextNPC.spriteNum];
                mSubNPC_panel.DOFade(1, Utils.storyBlockFlipTime);
            }

            if (mPastSysKey.key == "Naration" || mPastSysKey.key == "No Text" || mPastSysKey.key == null) {
                mText.color = Color.black;
                mTextMainPanel.sprite = mTextMainPanelSprites[0];
                mTextMainPanel.DOFade(1, Utils.storyBlockFlipTime);
            }
        }

        StartCoroutine(EffectKey(nextKey, sameness));

        if (nextKey.key != "No Text" && !sameness.sameKey && nextNPC.markingNum != 2)
            mCanvas.DOFade(1, Utils.storyBlockFlipTime);
        if (!sameness.samePanelSpriteNum && nextKey.panelSpriteNum != -1 && nextKey.panelSpriteNum != -2) {
            mPanel.sprite = mPanelSprites[nextKey.panelSpriteNum];
            mPanel.DOFade(1, Utils.storyBlockFlipTime);
        }
        if (!sameness.sameCloseUpPanelSpriteNum && nextKey.closeUpPanelSpriteNum != -1 && nextKey.closeUpPanelSpriteNum != -2) {
            mCloseUpPanel.sprite = mCloseUpPanelSprites[nextKey.closeUpPanelSpriteNum];
            mCloseUpPanel.DOFade(1, Utils.storyBlockFlipTime);
        }
        if (nextKey.BGM_Num == -2 && !sameness.same_BGM_Num)
            SoundMngScript.PlaySound("Stop All BGM");
        else if (nextKey.BGM_SubNum == -2 && !sameness.same_BGM_SubNum)
            SoundMngScript.PlaySound("Stop Sub BGM");
        else if (!sameness.same_BGM_Num && nextKey.BGM_Num != -1)
            SoundMngScript.PlaySound("BGM", nextKey.BGM_Num, nextKey.BGM_SubNum);
        else if (!sameness.same_BGM_SubNum && nextKey.BGM_SubNum != -1)
            SoundMngScript.PlaySound("Sub BGM", -1, nextKey.BGM_SubNum);
        if (nextKey.effectNum != -1)
            SoundMngScript.PlaySound("Effect", nextKey.effectNum);
        if (nextKey.fade)
            mFade.DOFade(0, Utils.storyBlockFadeTime);

        yield return new WaitForSeconds(Utils.storyBlockFlipTime);

        if (nextNPC.markingNum == 0 && mLastNPC.name != nextNPC.name) {
            mNPC_panel.sprite = mNPC_sprites[nextNPC.spriteNum];
            mNPC_eyeAnimator.SetTrigger(nextNPC.eyeTriggerName);
            mNPC_mouthAnimator.SetTrigger(nextNPC.mouthTriggerName);
        }

        mFlipping = false;

        StartCoroutine(EffectKey(nextKey, sameness));

        mPastSysKey = nextKey;
        if (nextNPC.markingNum != -1)
            mLastNPC = nextNPC;
        if (nextNPC.markingNum == 0)
            mLastPanelNPC = nextNPC;
    }

    IEnumerator FlipUpSelection(int _selectingIndex) {
        mFlipping = true;

        mSelectingIndex = _selectingIndex;

        if (mSelections[_selectingIndex].isLove)
            StartCoroutine(FlipUpMessage("여기에서의 선택에 따라 호감도가 크게 변합니다!"));

        if (mSelections[_selectingIndex].third == null) {
            for (int i = 0; i < 2; i++)
                mTwoSelectingButtons[i].interactable = !mSelections[_selectingIndex].buttonLock[i];
            mTwoSelectCanvas.DOFade(1, Utils.storyBlockFadeTime);
            mTwoSelectingButtons[0].GetComponentInChildren<Text>().text = mSelections[_selectingIndex].first;
            mTwoSelectingButtons[1].GetComponentInChildren<Text>().text = mSelections[_selectingIndex].second;
            mTwoSelectCanvas.gameObject.SetActive(true);
        }
        else {
            for (int i = 0; i < 3; i++)
                mThreeSelectingButtons[i].interactable = !mSelections[_selectingIndex].buttonLock[i];
            mThreeSelectCanvas.DOFade(1, Utils.storyBlockFadeTime);
            mThreeSelectingButtons[0].GetComponentInChildren<Text>().text = mSelections[_selectingIndex].first;
            mThreeSelectingButtons[1].GetComponentInChildren<Text>().text = mSelections[_selectingIndex].second;
            mThreeSelectingButtons[2].GetComponentInChildren<Text>().text = mSelections[_selectingIndex].third;
            mThreeSelectCanvas.gameObject.SetActive(true);
        }

        yield return new WaitForSeconds(Utils.storyBlockFadeTime);

        mFlipping = false;
	}

    IEnumerator FlipDownSelection() {
        mFlipping = true;

        if (mSelections[mSelectingIndex].third == null) {
            mTwoSelectCanvas.DOFade(0, Utils.storyBlockFadeTime);
            mTwoSelectCanvas.gameObject.SetActive(false);
        }
        else {
            mThreeSelectCanvas.DOFade(0, Utils.storyBlockFadeTime);
            mThreeSelectCanvas.gameObject.SetActive(false);
        }

        yield return new WaitForSeconds(Utils.storyBlockFadeTime);

        mFlipping = false;
    }

    IEnumerator FlipUpMessage(string _message) {
        mMessageText.text = _message;
        mMessageCanvas.DOFade(1, Utils.storyBlockFadeTime);

        yield return new WaitForSeconds(Utils.storyBlockFadeTime);
	}

    IEnumerator FlipDownMessage() {
        mMessageCanvas.DOFade(0, Utils.storyBlockFadeTime);

        yield return new WaitForSeconds(Utils.storyBlockFadeTime);
    }

    IEnumerator EffectKey(SysKey _key, SysKeySameness _sameness) {
        if (mFlipping) {
            if (_key.effectKey == "Shake" && !_sameness.sameEffectKey)
                mPanel.rectTransform.DOShakeAnchorPos(100.0f);
            else {
                mPanel.rectTransform.DOKill();
                mPanel.rectTransform.anchoredPosition = Vector2.zero;
            }
        }
        else {
            if (_key.effectKey == "Double Fade") {
                if (!_key.fade)
                    Debug.LogError("Double Fade 를 fade 옵션 없이 사용하면 안 돼요!");

                mFlipping = true;
                mFade.DOFade(1, Utils.storyBlockFadeTime);
                yield return new WaitForSeconds(Utils.storyBlockFlipTime);
                mFade.DOFade(0, Utils.storyBlockFadeTime);
                yield return new WaitForSeconds(Utils.storyBlockFlipTime);
                mFlipping = false;
            }
        }
    }
}
