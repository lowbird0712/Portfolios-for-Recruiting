﻿using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;

[Serializable]
public class PRS {
    public Vector3          pos;
    public Quaternion       rot;
    public Vector3          scale;

    public PRS(Vector3 _pos, Quaternion _rot, Vector3 _scale) {
        pos = _pos;
        rot = _rot;
        scale = _scale;
    }
}

public class Utils : MonoBehaviour {
    ///////////////////// 비주얼노벨 적용 변수 ///////////////////////////////////////////////////////////////////////////////
    static public float             storyBlockFlipTime = 0.3f;

    ///////////////////// 카드게임 적용 변수 //////////////////////////////////////////////////////////////////////////////////
    static public Vector3           cardScale = new Vector3(6.0f, 6.0f, 1.0f);
    static public float             cardDragFloat = 1.5f;
    static public float             cardPutUpFloat = 3.0f;
    static public float             turnStartPanelUpDownDotweenTime = 0.3f;
    static public float             turnStartPanelAppendDotweenTIme = 0.9f;
    static public float             turnStartPanelDotweenTime = -1.0f;
    static public float             cardPutUpDotweenTime = 0.3f;
    static public float             cardPutDownDotweenTime = 0.3f;
    static public float             cardSwapDotweenTime = 0.3f;
    static public float             cardPutCoverDotweenTime = 0.3f;
    static public float             cardAlignmentDotweenTime = 0.3f;
    static public float             cardExecFuncDotweenTime = 0.5f;
    static public float             cardExecCountDownDotweenTime = 0.5f;

    ///////////////////// 패스트모드 적용 변수 ///////////////////////////////////////////////////////////////////////////////
    static public float             fastModeFloat = 0.1f;
    static public float             cardDrawDotweenTime = 0.5f;
    static public float             cardDrawExtraTime = 0.1f;
    
    static public Vector3 MousePos {
        get {
            Vector3 pos = Camera.main.ScreenToWorldPoint(Input.mousePosition);
            pos.z += 5;
            return pos;
        }
    }
}
