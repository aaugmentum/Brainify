﻿﻿using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;

public class IPCLoader : MonoBehaviour
{
    #region IPCFunctions Calls

    [Header("Login fields")]
    public InputField username;
    public InputField password;
    public InputField c_password;
    public Text error_message;

    [Header("Pin field")]

    public InputField pin_field;

    
    public void signin_button(){
        if(IPCManager.instance.is_connected){
            int result = IPCManager.instance.ipc_signin(username.text, password.text);
            if(result == 1){
                ScenesManager.instance.SwitchScene("StartMenu");
                PlayerPrefs.SetString("username", username.text);
                PlayerPrefs.SetString("password", password.text);
                PlayerPrefs.SetInt("is_logged_in", 1);
            }else{
                print("Wrong username or password");
                error_message.text = "Wrong username or password";
            }
        }else{
            error_message.text = "No connection";
        }
    }

    public void register_button(){
        error_message.text = " ";
        if(IPCManager.instance.is_connected){
            if(c_password.text.Equals(password.text)){
                int result = IPCManager.instance.ipc_signup(username.text, password.text);
                if(result == 1){
                    ScenesManager.instance.SwitchScene("Signin");
                }else{
                    print("User already exists");
                    error_message.text = "User already exists";
                }
            }else{
                print("Password do not match");
                error_message.text = "Password do not match";
            }
        }else{
            error_message.text = "No connection";
        }
        
    }

    public void join_game_button(){
        if(IPCManager.instance.is_connected){
            int pin = System.Convert.ToInt32(pin_field.text);
            print("PIN: " + pin);
            int result = IPCManager.instance.ipc_join_game(pin);
            if(result != 0){
                IPCManager.instance.gid = result.ToString();
                print("Game id: " + IPCManager.instance.gid);
                IPCManager.instance.createReceiverThread();
                ScenesManager.instance.SwitchScene("LobbyUser");
            }
        }
       
    }


   

    #endregion
}