using UnityEngine;
using System.Collections;

public class Invader : MonoBehaviour {

	public float speed = -3.0f;
	public float moveCooldown = 0.8f;

	private float moveTimer;
	private int row;
	private int column;
	private InvaderSpawner spawner;

	// Use this for initialization
	void Start () {
		moveTimer = moveCooldown;
	}
	
	// Update is called once per frame
	void Update () {
		moveTimer -= Time.deltaTime;

		if(moveTimer <= 0)
		{
			transform.Translate (0, speed, 0, Space.World);

			moveTimer = moveCooldown;
		}
	}

	void OnTriggerEnter(Collider collider)
	{
		spawner.rows[row].invaderNums[column]--;
		
		if (spawner.rows[row].invaderNums[column] == 0)
		{
			spawner.rows[row].target[column].SetActive (false);
		}

		Instantiate(Resources.Load("Explosion"), transform.position, new Quaternion());
		Destroy (gameObject);
	}

	public void init(int rowPos, int colPos, InvaderSpawner spawnerObject)
	{
		row = rowPos;
		column = colPos;
		spawner = spawnerObject;
	}
}
