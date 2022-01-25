using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class GridObjectScript : MonoBehaviour {
    Image                   panel = null;
    Image                   countDownPanel = null;

    [SerializeField] bool   isKitchen = false;
    [SerializeField] int[]  position = new int[2];

    string                  objectName = null;
    bool                    hasObject = false;
    int                     countDown = -1;

    public bool             IsKitchen { get => isKitchen; set { isKitchen = value; } }
    public string           ObjectName { set { objectName = value; } }
    public bool             HasObject { get => hasObject; set { hasObject = value; } }
    public int              CountDown { get => countDown; set { countDown = value; } }
    public int[]            Position { get => position; set { position = value; } }
}
