using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

public class CardGameMngScript : MonoBehaviour {
    public static CardGameMngScript                                                         Inst { get; private set; }
    void Awake() => Inst = this;
    
    [SerializeField] [Tooltip("게임의 속도가 매우 빨라집니다")] bool                        fastMode;
    [SerializeField] [Tooltip("시작 카드 개수를 정합니다")] int                             startCardCount;
    [SerializeField] [Tooltip("패에 들고 있을 수 있는 최대 카드 개수를 정합니다")] int      maxCardCount;
    [SerializeField] [Tooltip("첫 턴에 놓을 수 있는 카드의 개수를 정합니다")] int           startPutCardCount;
    [SerializeField] [Tooltip("놓을 수 있는 카드의 최대 개수를 정합니다")] int              maxPutCardCount;

    [SerializeField] TurnStartPanelScript                                                   turnStartPanel;
    [SerializeField] SO_GridObjectScript                                                    SO_GridObject;
    [SerializeField] GridObjectScript[]                                                     gridObjects;

    [Header("게임 시스템 변수")]
    public bool                                                                             isLoading;
                                                                                            
    public static Action                                                                    OnAddCardItem;

    bool                                                                                    myTurn;

    static public int                                                                       MaxCardCount => Inst.maxCardCount;
    static public int                                                                       MaxPutCardCount => Inst.maxPutCardCount;
    static public GridObjectScript[]                                                        GridObjects => Inst.gridObjects;
    static public bool                                                                      MyTurn => Inst.myTurn;

    void Start() {
        StartGame();
    }

    void Update() {
#if UNITY_EDITOR
        CheatKey();
#endif
    }

    private void OnDestroy() {
        if (fastMode) {
            Utils.cardDrawDotweenTime /= Utils.fastModeFloat;
            Utils.cardDrawExtraTime /= Utils.fastModeFloat;
        }
    }

    void CheatKey() {
        if (Input.GetKeyDown(KeyCode.O))
            OnAddCardItem?.Invoke();
        if (Input.GetKeyDown(KeyCode.E))
            EndTurn();
    }

    void GameSetup() {
        if (fastMode) {
            Utils.cardDrawDotweenTime *= Utils.fastModeFloat;
            Utils.cardDrawExtraTime *= Utils.fastModeFloat;
        }

        CardMngScript.InitCardPutArea(startPutCardCount);
    }

    void StartGame() {
        StartCoroutine(StartGameCo());
    }

    void TurnStart() {
        CardMngScript.IncreaseCardPutCount();
        turnStartPanel.Show();
    }

    public void EndTurn() {
        myTurn = false;
        StartCoroutine(ExecuteCardsCo());
        StartCoroutine(ExecuteCountDownCo());
        StartCoroutine(StartTurnCo());
    }

    IEnumerator StartGameCo() {
        GameSetup();

        isLoading = true;

        for (int i = 0; i < startCardCount; i++) {
            OnAddCardItem?.Invoke();
            yield return new WaitForSeconds(Utils.cardDrawDotweenTime + Utils.cardDrawExtraTime);
        }

        StartCoroutine(StartTurnCo());
    }

    IEnumerator StartTurnCo() {
        isLoading = true;

        myTurn = true;
        TurnStart();
        yield return new WaitForSeconds(Utils.turnStartPanelDotweenTime);
        OnAddCardItem?.Invoke();
        yield return new WaitForSeconds(Utils.cardDrawDotweenTime);

        isLoading = false;
    }

    IEnumerator ExecuteCardsCo() {
        List<CardScript>    putCards = CardMngScript.PutCards;
        CardScript          card;

        for (int i = 0; i < putCards.Count; i++) {
            card = putCards[i];
            card.gameObject.SetActive(false);

            if (card.IsDotori && !card.IsFront)
                CardMngScript.CardItemScript.ExecuteCardFunc(card.CardName, SO_Dotori.DotoriBackSide);
            else if (!card.IsFront)
                CardMngScript.CardItemScript.ExecuteCardFunc(card.CardName, SO_Dotori.DotoriSide);
            else
                CardMngScript.CardItemScript.ExecuteCardFunc(card.CardName, SO_Dotori.Default, card.TargetGridPos[0], card.TargetGridPos[1]);

            yield return new WaitForSeconds(Utils.cardExecFuncDotweenTime);
        }

        putCards.Clear();
	}

    IEnumerator ExecuteCountDownCo() {
        GridObjectScript grid = null;

        for (int i = 0; i < 16; i++) {
            grid = gridObjects[i];

            if (grid.CountDown == -1)
                continue;
            else {
                grid.CountDown -= 1;
                if (grid.CountDown == 0) {
                    ////
                }
            }

            yield return new WaitForSeconds(Utils.cardExecCountDownDotweenTime);
        }
	}
}
