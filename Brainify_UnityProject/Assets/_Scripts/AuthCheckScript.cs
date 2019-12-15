using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AuthCheckScript : MonoBehaviour
{
    // Start is called before the first frame update

    // Update is called once per frame
    public Text errorMessage;
    void Update()
    {
        if(IPCManager.instance.result == 2 && PlayerPrefs.GetInt("is_logged_in") == 1 ){
            print("THE RESULT IS " + IPCManager.instance.result);
            IPCManager.signin(PlayerPrefs.GetString("username"), PlayerPrefs.GetString("password"));
            ScenesManager.instance.SwitchScene("StartMenu");
        }
        // if(IPCManager.instance.result != 2){
        //     errorMessage.text = "Waiting for the connection...";
        // }else{
        //     errorMessage.text = "";
        //     IPCManager.instance.result = 1;
        // }
    }
}
