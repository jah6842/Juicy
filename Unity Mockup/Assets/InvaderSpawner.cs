using UnityEngine;
using System.Collections;

public class InvaderSpawner : MonoBehaviour {

	public Row[] rows;
	public float spawnCooldown=2.5f;
	public int initInvaders=4;

	private float spawnTimer;

	// Use this for initialization
	void Start () {
		spawnTimer = 0.0f;

		for (int i = 0; i < initInvaders; i++)
		{
			SpawnInvader (i, Random.Range (0, 4));
		}
	}
	
	// Update is called once per frame
	void Update () {

		spawnTimer -= Time.deltaTime;

		if (spawnTimer <= 0)
		{
			SpawnInvader ();
		}

		if (Input.GetKeyDown (KeyCode.Escape))
		{
			Application.Quit ();
		}
	}

	void SpawnInvader()
	{
		int row = Random.Range (0, 4);
		int column = Random.Range (0, 4);
		
		GameObject invader = Instantiate(Resources.Load("Space Invader"), rows[row].tile[column].position, new Quaternion()) as GameObject;
		Invader invaderScript = invader.GetComponent<Invader>();
		
		invaderScript.init (row, column, this);
		
		rows[row].invaderNums[column]++;
		
		if (rows[row].invaderNums[column] == 1)
		{
			rows[row].target[column].SetActive (true);
		}
		
		spawnTimer = spawnCooldown;
	}

	void SpawnInvader(int row, int column)
	{
		GameObject invader = Instantiate(Resources.Load("Space Invader"), rows[row].tile[column].position, new Quaternion()) as GameObject;
		Invader invaderScript = invader.GetComponent<Invader>();
		
		invaderScript.init (row, column, this);
		
		rows[row].invaderNums[column]++;
		
		if (rows[row].invaderNums[column] == 1)
		{
			rows[row].target[column].SetActive (true);
		}
		
		spawnTimer = spawnCooldown;
	}
}
