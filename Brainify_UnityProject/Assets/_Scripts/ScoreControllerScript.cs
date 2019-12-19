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
        if(IPCManager.instance.receiverThread.IsAlive){
            print("It is still alive ffs: " + IPCManager.instance.receiverThread.IsAlive);
            IPCManager.instance.receiverThread.Abort();
            print("After killing: " + IPCManager.instance.receiverThread.IsAlive);
        }else{
            print("It was dead");
        }
    }
}
