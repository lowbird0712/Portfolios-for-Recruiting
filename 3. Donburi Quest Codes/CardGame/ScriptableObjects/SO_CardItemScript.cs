using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using Random = UnityEngine.Random;

public enum SO_CardType {
    CannotUse = 0,
    Kitchen = 1,
    Normal = 2,
    Placable = 3,
    Choosable = 4
}

public enum SO_Dotori {
    Default = 0,
    DotoriSide = 1,
    DotoriBackSide = 2
}

public class CardItem {
    public SO_CardType  type = SO_CardType.CannotUse;
    public Sprite       sprite = null;
    public string       explain = null;
    public bool         haveDest = false;
}

[CreateAssetMenu(fileName = "SO_CardItem", menuName = "Scriptable Object/SO_CardItem")]
public class SO_CardItemScript : ScriptableObject {
    [SerializeField] Sprite[]   cardSprites = null;

    string[]                    cardNames = null;
    CardItem[]                  cardItems = null;

    public void Setup(string _deckName) {
        //cardNames[0] = "";
        //cardItems[0] = new CardItem();
        //cardItems[0].type = SO_CardType.CannotUse;
        //cardItems[0].sprite = cardSprites[0];
        //cardItems[0].explain = "";
        //cardItems[0].haveDest = false;

        if (_deckName == "규동 기본") {
            cardNames = new string[16];
            cardItems = new CardItem[16];
            cardItems[0].type = SO_CardType.Kitchen;

            // 주방 전개
            cardNames[0] = "주방 전개";
            cardItems[0] = new CardItem();
            cardItems[0].sprite = cardSprites[0];
            cardItems[0].explain = "지정한 위치에 주방 타일을 전개한다.";
            cardItems[0].type = SO_CardType.Kitchen;
            // 밥솥
            cardNames[1] = "밥솥";
            cardItems[1] = new CardItem();
            cardItems[1].sprite = cardSprites[1];
            cardItems[1].explain = "지정한 위치에 밥솥(조리전)을 놓는다.";
            cardItems[1].type = SO_CardType.Placable;
            // 냄비
            cardNames[2] = "냄비";
            cardItems[2] = new CardItem();
            cardItems[2].sprite = cardSprites[2];
            cardItems[2].explain = "지정한 위치에 냄비(조리전)을 놓는다.";
            cardItems[2].type = SO_CardType.Placable;
            // 불지피기
            cardNames[3] = "불지피기";
            cardItems[3] = new CardItem();
            cardItems[3].sprite = cardSprites[3];
            cardItems[3].explain = "지정한 위치에 불을 지핀다.";
            cardItems[3].type = SO_CardType.Choosable;
            // 쌀
            cardNames[4] = "쌀";
            cardItems[4] = new CardItem();
            cardItems[4].sprite = cardSprites[4];
            cardItems[4].explain = "지정한 위치에 쌀을 놓는다.";
            cardItems[4].type = SO_CardType.Placable;
            // 우삼겹
            cardNames[5] = "우삼겹";
            cardItems[5] = new CardItem();
            cardItems[5].sprite = cardSprites[5];
            cardItems[5].explain = "지정한 위치에 우삼겹을 놓는다.";
            cardItems[5].type = SO_CardType.Placable;
            // 양파
            cardNames[6] = "양파";
            cardItems[6] = new CardItem();
            cardItems[6].sprite = cardSprites[6];
            cardItems[6].explain = "지정한 위치에 양파를 놓는다.";
            cardItems[6].type = SO_CardType.Placable;
            // 조미료
            cardNames[7] = "조미료";
            cardItems[7] = new CardItem();
            cardItems[7].sprite = cardSprites[7];
            cardItems[7].explain = "지정한 위치에 상좌우하 중에서 가장 앞 순서에 있는 타일에 필요한 조미료 중 사용되지 않은 조미료를 랜덤으로 놓는다.";
            cardItems[7].type = SO_CardType.Placable;

            // 주방 위의 도토리
            cardNames[8] = "주방 위의 도토리";
            cardItems[8] = new CardItem();
            cardItems[8].sprite = cardSprites[8];
            cardItems[8].explain = "도토리가 주방 위에 놓여 있다.";
            // 밥솥 안의 도토리
            cardNames[9] = "밥솥 안의 도토리";
            cardItems[9] = new CardItem();
            cardItems[9].sprite = cardSprites[9];
            cardItems[9].explain = "도토리가 밥솥 안에 들어 있다.";
            // 냄비 안의 도토리
            cardNames[10] = "냄비 안의 도토리";
            cardItems[10] = new CardItem();
            cardItems[10].sprite = cardSprites[10];
            cardItems[10].explain = "도토리가 냄비 안에 들어 있다.";
            // 타고 있는 도토리
            cardNames[11] = "타고 있는 도토리";
            cardItems[11] = new CardItem();
            cardItems[11].sprite = cardSprites[11];
            cardItems[11].explain = "도토리가 활활 타고 있다. 고소한 냄새가 난다.";
            // 쌀 포대 안의 도토리
            cardNames[12] = "쌀 포대 안의 도토리";
            cardItems[12] = new CardItem();
            cardItems[12].sprite = cardSprites[12];
            cardItems[12].explain = "도토리가 쌀 포대 안에 들어 있다.";
            // 우삼겹에 말려 있는 도토리
            cardNames[13] = "우삼겹에 말려 있는 도토리";
            cardItems[13] = new CardItem();
            cardItems[13].sprite = cardSprites[13];
            cardItems[13].explain = "도토리가 두툼한 우삼겹에 말려 있다. 이대로 구우면 맛있을 것 같다.";
            // 양파와 도토리
            cardNames[14] = "양파와 도토리";
            cardItems[14] = new CardItem();
            cardItems[14].sprite = cardSprites[14];
            cardItems[14].explain = "도토리가 양파 옆에 놓여 있다.";
            // 향기로운 도토리
            cardNames[15] = "향기로운 도토리";
            cardItems[15] = new CardItem();
            cardItems[15].sprite = cardSprites[15];
            cardItems[15].explain = "도토리가 강렬한 향기를 내뿜고 있다. 식욕을 돋우는 향이다.";
        }
    }

    public CardItem GetCardItem(string _cardName) {
        CardItem result = new CardItem();

        for (int i = 0; i < cardNames.Length; i++) {
            if (cardNames[i] == _cardName) {
                result = cardItems[i];
                return result;
            }
        }

        Debug.LogError("찾으려는 CardItem이 Scriptable Object에 없습니다!");
        return result;
    }

    public List<int[]> GetPlacableGridPos(string _cardName) {
        List<int[]>         list = new List<int[]>();
        GridObjectScript[]  gridObjects = CardGameMngScript.GridObjects;
        SO_CardType         type = GetCardItem(_cardName).type;

        foreach (var grid in gridObjects) {
            switch (type) {
                case SO_CardType.Kitchen:
                    if (grid.IsKitchen)
                        continue;
                    break;
                case SO_CardType.Normal:
                    break;
                case SO_CardType.Placable:
                    if (!grid.IsKitchen || grid.HasObject)
                        continue;
                    break;
                case SO_CardType.Choosable:
                    if (!grid.HasObject)
                        continue;
                    break;
			}

            if (SpecificPlacableTest(_cardName))
				list.Add(grid.Position);
		}

        return list;
    }

    public bool SpecificPlacableTest(string _cardName) {
        if (_cardName == "") {

		}
        else {

		}

        return true; ////
	}

    public void ExecuteCardFunc(string _cardName, SO_Dotori _dotoriFlag, int _x = -1, int _y = -1) {
        // 도토리 기본 기믹
        if (_dotoriFlag == SO_Dotori.DotoriSide)
            ChangeToDotori(_cardName);
        else if (_dotoriFlag == SO_Dotori.DotoriBackSide)
            DotoriReturn(_cardName);

        // 카드 효과 발동
        
	}

    void ChangeToDotori(string _cardName) {
        // 기본
        if (_cardName == "주방 전개")
            CreateCardToHand("주방 위의 도토리");
        else if (_cardName == "불지피기")
            CreateCardToHand("타고 있는 도토리");
        else if (_cardName == "밥솥")
            CreateCardToHand("밥솥 안의 도토리");
        else if (_cardName == "냄비")
            CreateCardToHand("냄비 안의 도토리");
        else if (_cardName == "쌀")
            CreateCardToHand("쌀 포대 안의 도토리");
        else if (_cardName == "조미료")
            CreateCardToHand("향기로운 도토리");

        // 규동 기본
        else if (_cardName == "우삼겹")
            CreateCardToHand("우삼겹에 말려 있는 도토리");
        else if (_cardName == "양파")
            CreateCardToHand("양파와 도토리");

    }

    void DotoriReturn(string _cardName) {
        // 기본
        if (_cardName == "주방 위의 도토리")
            AddCardToDeck("주방 전개");
        else if (_cardName == "타고 있는 도토리")
            AddCardToDeck("불지피기");
        else if (_cardName == "밥솥 안의 도토리")
            AddCardToDeck("밥솥");
        else if (_cardName == "냄비 안의 도토리")
            AddCardToDeck("냄비");
        else if (_cardName == "쌀 포대 안의 도토리")
            AddCardToDeck("쌀");
        else if (_cardName == "향기로운 도토리")
            AddCardToDeck("조미료");

        // 규동 기본
        else if (_cardName == "우삼겹에 말려 있는 도토리")
            AddCardToDeck("우삼겹");
        else if (_cardName == "양파와 도토리")
            AddCardToDeck("양파");

        CardMngScript.AddCardItem();
    }

    // 카드 효과 함수
    void CreateCardToHand(string _cardName, int _num = 1) {
        for (int i = 0; i < _num; i++) {
            CardMngScript.CardBuffer.Insert(0, _cardName);
            CardMngScript.AddCardItem();
		}
    }

    void AddCardToDeck(string _cardName, int _num = 1) {
        int index;
        for (int i = 0; i < _num; i++) {
            index = Random.Range(0, CardMngScript.CardBuffer.Count + 1);
            if (index == CardMngScript.CardBuffer.Count + 1)
                CardMngScript.CardBuffer.Add(_cardName);
            else
                CardMngScript.CardBuffer.Insert(index, _cardName);
		}
	}
}
