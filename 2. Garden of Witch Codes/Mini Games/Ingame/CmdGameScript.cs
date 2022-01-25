using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CmdGameScript : MiniGameScript {
    int mCommandNum = 0;
    int mCorrectNum = 0;
    int[] mCommands = new int[4];

    public void InitCommand(int _commandNum) {
        // 화살표 커맨드의 개수를 설정한다
        // 이 때 _commandNum개의 화살표의 방향이 랜덤으로 설정되고 이 정보가 스크립트 내에 저장된다
        mCommandNum = _commandNum;

        int dirNum = 0;
        GameObject arrow = null;
        for (int i = 0; i < mCommandNum; i++) {
            arrow = new GameObject("Arrow" + i);
            arrow.AddComponent<SpriteRenderer>();
            arrow.transform.parent = transform;

            dirNum = Random.Range(0, 4);
            switch (dirNum) {
                case 0:
                    arrow.GetComponent<SpriteRenderer>().sprite = LogicValue.CmdLeftArrowSprite;
                    mCommands[i] = 0;
                    break;
                case 1:
                    arrow.GetComponent<SpriteRenderer>().sprite = LogicValue.CmdRightArrowSprite;
                    mCommands[i] = 1;
                    break;
                case 2:
                    arrow.GetComponent<SpriteRenderer>().sprite = LogicValue.CmdUpArrowSprite;
                    mCommands[i] = 2;
                    break;
                case 3:
                    arrow.GetComponent<SpriteRenderer>().sprite = LogicValue.CmdDownArrowSprite;
                    mCommands[i] = 3;
                    break;
            }

            arrow.transform.localPosition = Vector3.zero + Vector3.left * (mCommandNum / 2.0f - 0.5f - i) + Vector3.up * LogicValue.CmdArrowShiftY;
        }
    }

    // Start is called before the first frame update
    void Start() {
        base.Start();
    }

    // Update is called once per frame
    void Update() {
        if (Input.GetKeyDown(KeyCode.LeftArrow) || Input.GetKeyDown(KeyCode.RightArrow) || Input.GetKeyDown(KeyCode.UpArrow) || Input.GetKeyDown(KeyCode.DownArrow)) {
            if (Input.GetKeyDown(KeyCode.LeftArrow) && mCommands[mCorrectNum] == 0) {
                transform.GetChild(mCorrectNum + 2).GetComponent<SpriteRenderer>().enabled = false;
                mCorrectNum++;
            }
            else if (Input.GetKeyDown(KeyCode.RightArrow) && mCommands[mCorrectNum] == 1) {
                transform.GetChild(mCorrectNum + 2).GetComponent<SpriteRenderer>().enabled = false;
                mCorrectNum++;
            }
            else if (Input.GetKeyDown(KeyCode.UpArrow) && mCommands[mCorrectNum] == 2) {
                transform.GetChild(mCorrectNum + 2).GetComponent<SpriteRenderer>().enabled = false;
                mCorrectNum++;
            }
            else if (Input.GetKeyDown(KeyCode.DownArrow) && mCommands[mCorrectNum] == 3) {
                transform.GetChild(mCorrectNum + 2).GetComponent<SpriteRenderer>().enabled = false;
                mCorrectNum++;
            }
            else
                mSFflag = SUCCEED_FLAG.SF_FAILED;

            if (mCorrectNum == mCommandNum)
                mSFflag = SUCCEED_FLAG.SF_SUCCEED;
        }

        base.Update();
    }
}
