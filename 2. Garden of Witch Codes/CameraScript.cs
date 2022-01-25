using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraScript : MonoBehaviour {
    // Start is called before the first frame update
    void Start() { }

    // Update is called once per frame
    void FixedUpdate() {
        if (WitchScript.UsingObject || WitchScript.UsingFunction)
            return;

        // 마녀가 이동 중일 때 조건을 만족하면 카메라가 이동한다
        if (WitchScript.WitchPos.x < transform.position.x - LogicValue.WindowWidth * (0.5f - LogicValue.CameraLeftRate)
            && Input.GetKey(KeyCode.LeftArrow) && !WitchScript.UsingLadder) {

            transform.position += Vector3.left * LogicValue.WitchSpeed * Time.deltaTime;

            if (transform.position.x - LogicValue.WindowWidth / 2 < LogicValue.BgRect.xMin)
                transform.position -= Vector3.left * LogicValue.WitchSpeed * Time.deltaTime;
        }
        if (WitchScript.WitchPos.x > transform.position.x + LogicValue.WindowWidth * (LogicValue.CameraRightRate - 0.5f)
            && Input.GetKey(KeyCode.RightArrow) && !WitchScript.UsingLadder) {

            transform.position += Vector3.right * LogicValue.WitchSpeed * Time.deltaTime;

            if (transform.position.x + LogicValue.WindowWidth / 2 > LogicValue.BgRect.xMax)
                transform.position -= Vector3.right * LogicValue.WitchSpeed * Time.deltaTime;
        }
        if (WitchScript.WitchPos.y > transform.position.y + LogicValue.WindowHeight * (LogicValue.CameraUpRate - 0.5f)
            && Input.GetKey(KeyCode.UpArrow) && WitchScript.UsingLadder) {

            transform.position += Vector3.up * LogicValue.WitchLadderSpeed * Time.deltaTime;

            if (transform.position.y + LogicValue.WindowHeight / 2 > LogicValue.BgRect.yMax)
                transform.position -= Vector3.up * LogicValue.WitchLadderSpeed * Time.deltaTime;
        }
        if (WitchScript.WitchPos.y < transform.position.y - LogicValue.WindowHeight * (0.5f - LogicValue.CameraDownRate)) {
            float witchVelocityY;

            if (WitchScript.WitchVelocity.y < 0)
                witchVelocityY = WitchScript.WitchVelocity.y;
            else if (Input.GetKey(KeyCode.DownArrow))
                witchVelocityY = -LogicValue.WitchLadderSpeed;
            else
                return;

            transform.position += Vector3.up * witchVelocityY * Time.deltaTime;

            if (transform.position.y - LogicValue.WindowHeight / 2 < LogicValue.BgRect.yMin)
                transform.position -= Vector3.up * witchVelocityY * Time.deltaTime;
        }
    }
}
