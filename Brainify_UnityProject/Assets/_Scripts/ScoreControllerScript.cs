using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class ScoreControllerScript : MonoBehaviour
{
    public EventsContainer events;

    public Text scoreText;

    void Start()
    {
        scoreText.text = "Your score: " + events.currentScore + " points";
    }
}
