using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using DG.Tweening;

public class DotweenMovingScript : MonoBehaviour {
    public void MoveTransform(PRS _prs) {
        transform.position = _prs.pos;
        transform.rotation = _prs.rot;
        transform.localScale = _prs.scale;
    }

    public void MoveTransform(PRS _prs, float _timeDotween, Ease _ease) {
        transform.DOMove(_prs.pos, _timeDotween).SetEase(_ease);
        transform.DORotateQuaternion(_prs.rot, _timeDotween).SetEase(_ease);
        transform.DOScale(_prs.scale, _timeDotween).SetEase(_ease);
    }
}