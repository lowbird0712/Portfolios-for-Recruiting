using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GridScript : MonoBehaviour {
	[SerializeField] CardScript card;
	[SerializeField] int[]		position = new int[2];

	public int[]				Position => position;

	private void OnMouseDown() {
		SpriteRenderer renderer = GetComponent<SpriteRenderer>();

		if (renderer.enabled) {
			if (card.SelectedGridNum == 1)
				return;
			renderer.enabled = false;
			card.SelectedGridNum += 1;
			card.GetComponent<CardScript>().TargetGridPos = position;
		}
		else {
			renderer.enabled = true;
			card.SelectedGridNum -= 1;
			card.GetComponent<CardScript>().TargetGridPos = position;
		}
	}
}
