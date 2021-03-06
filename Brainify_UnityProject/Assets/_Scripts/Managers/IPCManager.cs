﻿using UnityEngine;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Threading;
using System;

public class IPCManager : MonoBehaviour
{
    [DllImport ("Simple")] 
    public static extern int connect_server(string ip);
    [DllImport ("Simple")] 
    public static extern int signin(string username, string password);
    [DllImport ("Simple")] 
    public static extern int signup(string username, string password);
    [DllImport ("Simple")] 
    public static extern string games();
    [DllImport ("Simple")] 
    public static extern int start_game(string gid);
    [DllImport ("Simple")] 
    public static extern void run_game();
    [DllImport ("Simple")] 
    public static extern int join(int pin);
    [DllImport ("Simple")] 
    public static extern void answer(int score);
    [DllImport ("Simple")] 
    public static extern string get_questions(string gid);
    [DllImport ("Simple")] 
    public static extern string receive_standings();
    [DllImport ("Simple")] 
    public static extern int receiver();
    [DllImport ("Simple")] 
    public static extern void signout();


    public static IPCManager instance;
    public SortedDictionary<string, string> gamesMap = new SortedDictionary<string, string>();
    public int result;
    public Thread connectionThread;
    public Thread receiverThread;
    public string questionData;
    public bool is_connected = false;
    public string gid;
    public bool answers_received = false;


    public int game_pin = 0;
    private int state = 0;
    private void Awake()
    {
        instance = this;
    }
    void Start(){
        // connectionThread = new Thread(() => {
        //     try{
        //         Connect();
        //     }catch(Exception e){
        //         Debug.LogException(e, this);
        //     }
        // });
        connectionThread = new Thread(Connect);
        connectionThread.IsBackground = true;
        connectionThread.Start();
    }
    void Update()
    {
        if(state == 1){
            state = 0;
            ScenesManager.instance.SwitchScene("Question");
        }else if(state == 2){
            print("Next question");
            UIManager.instance.FadeOutPostQuestionScreenUI();
            GameManager.instance.DisplayQuestion();
            state = 0;
        }else if(state == 3){
            ScenesManager.instance.SwitchScene("FinishClient");
            state = 0;
        }
    }

    public void createReceiverThread(){
        if(receiverThread == null || !receiverThread.IsAlive){
            receiverThread = new Thread(Receive);
            receiverThread.Start();
        }
    }
    void OnApplicationQuit()
    {
        if(receiverThread != null && receiverThread.IsAlive){
            receiverThread.Abort();
        }
        if(connectionThread != null && connectionThread.IsAlive){
            connectionThread.Abort();
        }
        print("App closed");
        ipc_signout();
    }

    public void getGames(){
        if(IPCManager.instance.is_connected){
            string gamesInfo = ipc_get_games_list();
            string[] gamesList = gamesInfo.Split(',');
            for(int i = 0; i < gamesList.Length - 1; i++){
                string[] gameData = gamesList[i].Split('/');
               gamesMap.Add(gameData[0], gameData[1]);
            }
        }
       
    }

    public void Connect(){
        print("Trying to connect");
      
        try{
            result = IPCManager.connect_server("127.0.0.1");
        }catch(Exception e){
            Debug.LogException(e, this);
        }

        if(result == 1){
            is_connected = true;
        }
        getGames();
        print(result == 1 ? "Connected" : "Error");
        connectionThread.Abort();
    }


    public void Receive(){
        if(IPCManager.instance.is_connected){
             print("GID: " + gid);
            questionData = ipc_get_questions(gid);
            answers_received = true;
            print(questionData);
            print("Receiver is active");
            while(true){
                int x = ipc_receiver();
                print("Got a request: " + x);        
                if(x == 3){
                    state = 3;
                    break;
                }
                switch(x){
                    case 1:
                        state = 1;
                    break;
                    case 2:
                        state = 2;
                    break;
                }
            }  
        }
       
    }
//IPC Functions with connection check
public int ipc_signin(string username, string password){
    int result = 0;
    if(is_connected)result = signin(username, password);
    return result;
}
public int ipc_signup(string username, string password){
    int result = 0;
    if(is_connected)result = signup(username, password);
    return result;
}
public int ipc_start_game(string gid){
    int result = 0;
    if(is_connected)result = start_game(gid);
    return result;
}
public string ipc_get_games_list(){
    string result = "";
    if(is_connected)result = games();
    return result;
}
public string ipc_get_questions(string gid){
    string result = "";
    if(is_connected)result = get_questions(gid);
    return result;
}
public int ipc_join_game(int pin){
    int result = 0;
    if(is_connected)result = join(pin);
    return result;
}
public void ipc_signout(){
    if(is_connected)signout();
}
public int ipc_receiver(){
    int result = 0;
    if(is_connected)result = receiver();
    return result;
}
public void ipc_run_game(){
    if(is_connected)run_game();
}
public string ipc_standing(){
    string result = "";
    if(is_connected) result = receive_standings();
    return result;
}
public void ipc_answer(int score){
    if(is_connected)answer(score);
}
}