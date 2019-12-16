using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class AuthCheckScript : MonoBehaviour
{
    // Start is called before the first frame update

    // Update is called once per frame
    int x = 0;
    public GameObject signin_window;
    void Start(){
        // signin_window.SetActive(false);
    }
    void Update()
    {
        // if(IPCManager.instance.result == 1 && PlayerPrefs.GetInt("is_logged_in") == 1 && x == 0){
        //     int x = IPCManager.signin(PlayerPrefs.GetString("username"), PlayerPrefs.GetString("password"));
        //     if(x == 1){
        //         ScenesManager.instance.SwitchScene("StartMenu");
        //     }else{
        //         x = -1;
        //         PlayerPrefs.SetInt("is_logged_in", 0);
        //         signin_window.SetActive(true);
        //     }
        // }else{
        //    if(!signin_window.activeSelf)signin_window.SetActive(true);
        // }
    }
}
