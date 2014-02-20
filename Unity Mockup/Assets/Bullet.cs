using UnityEngine;
using System.Collections;

public class Bullet : MonoBehaviour {

	// Use this for initialization
	void Start () {
	
	}
	
	// Update is called once per frame
	void Update () {
		transform.Translate (0.0f, 1.0f, 0.0f);

		if(transform.position.y > 140)
		{
			DestroyObject(this.gameObject);
		}
	}

	void OnTriggerEnter(Collider collider)
	{
		if(collider.gameObject.tag == "Invader")
		{
			Destroy (gameObject);
		}
	}
}
