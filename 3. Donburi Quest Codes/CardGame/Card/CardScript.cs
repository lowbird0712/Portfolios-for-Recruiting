using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using DG.Tweening;

public class CardScript : DotweenMovingScript {
    [SerializeField] GameObject     grids;
    [SerializeField] SpriteRenderer cardRenderer;
    [SerializeField] SpriteRenderer spriteRenderer;
    [SerializeField] TMP_Text       nameTMP;
    [SerializeField] Sprite         cardFrontSprite;
    [SerializeField] Sprite         cardBackSprite;
    [SerializeField] bool           isEmpty;

    string                          cardName;
    int[]                           targetGridPos = new int[2]{ -1, -1};
    int                             selectedGridNum;
    bool                            isFront = true;
    bool                            isDotori = false;
    bool                            isPut;
    PRS                             originPRS;

    public GameObject               Grids => grids;
    public string                   CardName => cardName;
    public int[]                    TargetGridPos { get => targetGridPos; set { targetGridPos = value; } }
    public int                      SelectedGridNum { get => selectedGridNum; set { selectedGridNum = value; } }
    public bool                     IsFront => isFront;
    public bool                     IsDotori => isDotori;
    public bool                     IsPut { get => isPut; set { isPut = value; } }
    public PRS                      OriginPRS { get => originPRS; set => originPRS = value; }

    #region Card & Mouse
    private void OnMouseDown() => CardMngScript.CardMouseDown(this);

    private void OnMouseUpAsButton() => CardMngScript.CardMouseUp();
    #endregion

    void GridSetUp() {
        int a;
        if (cardName == "불지피기")
            a = 1;

        List<int[]> placableList = CardMngScript.CardItemScript.GetPlacableGridPos(cardName);
        GridScript  grid;
        int         index = 0;
        int[]       pos1 = {-1, -1};
        int[]       pos2 = {-1, -1};

        foreach (Transform transform in grids.GetComponentsInChildren<Transform>()) {
            if (!(grid = transform.GetComponent<GridScript>()) || placableList.Count == 0)
                continue;

            pos1 = grid.Position;
            pos2 = placableList[index];
            if (pos1[0] == pos2[0] && pos1[1] == pos2[1]) {
                grid.GetComponent<BoxCollider2D>().enabled = true;
                index++;
                if (index == placableList.Count)
                    return;
			}
        }
	}

    public void Setup(string _cardName, bool _isFront) {
        cardName = _cardName;
        isFront = _isFront;

        if (_isFront) {
            var cardItem = CardMngScript.GetCardItem(cardName);
            cardRenderer.sprite = cardFrontSprite;
            spriteRenderer.sprite = cardItem.sprite;
            nameTMP.text = cardName;
        }
        else {
            cardRenderer.sprite = cardBackSprite;
            spriteRenderer.sprite = null;
            nameTMP.text = "";
        }

        GridSetUp();
    }

    public void Swap() {
        if (isFront) {
            cardRenderer.sprite = cardBackSprite;
            spriteRenderer.sprite = null;
            nameTMP.text = "";
            isFront = false;
        }
        else {
            var cardItem = CardMngScript.GetCardItem(cardName);
            cardRenderer.sprite = cardFrontSprite;
            spriteRenderer.sprite = cardItem.sprite;
            nameTMP.text = cardName;
            isFront = true;
        }
    }
}
