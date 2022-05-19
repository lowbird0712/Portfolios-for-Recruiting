using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LoveNode {
	public string	name;
	public int		love;

	public LoveNode(string _name, int _love) {
		name = _name;
		love = _love;
	}
}

public class LoveMngScript : MonoBehaviour {
	static LoveMngScript Inst { get; set; } = null;

	List<LoveNode> loveNodes = new List<LoveNode>();

	private void Awake() => Inst = this;

	private void Start() {
		loveNodes.Add(new LoveNode("고결", 0));
		loveNodes.Add(new LoveNode("우준", 0));
		loveNodes.Add(new LoveNode("현오", 0));
		loveNodes.Add(new LoveNode("유겸", 0));
	}

	static public void AddLove(string _name, int _love) {
		foreach (LoveNode node in Inst.loveNodes) {
			if (node.name == _name) {
				node.love += _love;
				return;
			}
		}

		Debug.LogError("찾고자 하는 캐릭터가 등록되어 있지 않아요!");
	}
}
