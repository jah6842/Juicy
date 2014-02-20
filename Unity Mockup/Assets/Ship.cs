using UnityEngine;
using System.Collections;

[System.Serializable]
public class Row
{
	public Transform[] tile;
	public GameObject[] target;
	public int[] invaderNums = new int[4];
};

public class Ship : MonoBehaviour 
{

	public Row[] rows;
	public float cooldownLength = 0.3f;

	private int xPos;
	private int yPos;
	private float cooldownTimer;


	// Use this for initialization
	void Start () 
	{
		xPos = Random.Range (0, 4);
		yPos = Random.Range (0, 4);
		cooldownTimer = cooldownLength;

		this.transform.position = rows[xPos].tile[yPos].position;
	}
	
	// Update is called once per frame
	void Update () 
	{
		if (Input.GetKeyDown (KeyCode.W) && xPos > 0)
		{
			xPos--;
		}
		else if(Input.GetKeyDown (KeyCode.S) && xPos < 3)
		{
			xPos++;
		}
		else if(Input.GetKeyDown (KeyCode.A) && yPos > 0)
		{
			yPos--;
		}
		else if(Input.GetKeyDown (KeyCode.D) && yPos < 3)
		{
			yPos++;
		}

		this.transform.position = rows[xPos].tile[yPos].position;

		cooldownTimer -= Time.deltaTime;

		if (Input.GetKey (KeyCode.Space) && cooldownTimer <= 0)
		{
			Instantiate(Resources.Load("Bullet"), transform.position, new Quaternion());

			cooldownTimer = cooldownLength;
		}

	}
}
