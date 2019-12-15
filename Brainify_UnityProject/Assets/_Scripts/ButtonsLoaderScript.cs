using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;


public class ButtonsLoaderScript : MonoBehaviour
{
    // Start is called before the first frame update
    public Button[] Buttons;
    void Start()
    {
        int i = 0;
        foreach(KeyValuePair<string, string> game in IPCManager.instance.gamesMap){
            Buttons[i].gameObject.GetComponentInChildren<Text>().text = game.Key;
            int index = i;
            Buttons[i].onClick.AddListener(() => start_game(index));
            i++;
        }
        while(i < Buttons.Length){
            Buttons[i].gameObject.SetActive(false);
            i++;
        }

    }

    // Update is called once per frame
    void Update()
    {
        
    }
    
    public void start_game(int index){
        print("You started " + IPCManager.instance.gamesMap[Buttons[index].gameObject.GetComponentInChildren<Text>().text]);
        IPCManager.instance.question = IPCManager.get_questions(IPCManager.instance.gamesMap[Buttons[index].gameObject.GetComponentInChildren<Text>().text]);
        print(IPCManager.instance.question);
    }
}
