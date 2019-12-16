using UnityEngine;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Threading;

public class IPCManager : MonoBehaviour
{
    [DllImport ("IPCPlugin")] 
    public static extern int signin(string username, string password);
    [DllImport ("IPCPlugin")] 
    public static extern int signup(string username, string password);
    [DllImport ("IPCPlugin")] 
    public static extern int connect_server();
    [DllImport ("IPCPlugin")] 
    public static extern int start_game(string gid);
    [DllImport ("IPCPlugin")] 
    public static extern string games();
    [DllImport ("IPCPlugin")] 
    public static extern string get_questions(string gid);
    [DllImport ("IPCPlugin")] 
    public static extern int join(int pin);
    [DllImport ("IPCPlugin")] 
    public static extern void signout();
    [DllImport ("IPCPlugin")] 
    public static extern int receiver();
    [DllImport ("IPCPlugin")] 
    public static extern string player_join();
    [DllImport ("IPCPlugin")] 
    public static extern void run_game();


    public static IPCManager instance;
    public SortedDictionary<string, string> gamesMap = new SortedDictionary<string, string>();
    public int result;
    public Thread connectionThread;
    public Thread receiverThread;
    public string questionData;
    public int is_connected = 0;
    public string gid;

    public int game_pin;
    private int state = 0;
    private void Awake()
    {
        instance = this;
    }
    void Start(){
        connectionThread = new Thread(Connect);
        receiverThread = new Thread(Receive);
        connectionThread.Start();
    }
    void Update()
    {
        if(state == 1){
            state = 0;
            ScenesManager.instance.SwitchScene("Question");
        }else if(state == 2){
            print("Next question");
            state = 0;
        }
    }
    void OnApplicationQuit()
    {
        connectionThread.Abort();
        receiverThread.Abort();
        signout();
    }

    public void getGames(){
        string gamesInfo = games();
        string[] gamesList = gamesInfo.Split(',');
        for(int i = 0; i < gamesList.Length - 1; i++){
            string[] gameData = gamesList[i].Split('/');
            gamesMap.Add(gameData[0], gameData[1]);
        }
        print(gamesInfo);
    }

    public void Connect(){
        print("Trying to connect");
        result = IPCManager.connect_server();
        getGames();
        if(result == 1){
            is_connected = 1;
        }else{
            is_connected = 2;
        }
        print(result == 1 ? "Connected" : "Error");
        connectionThread.Abort();
    }


    public void Receive(){
        print("GID: " + gid);
        questionData = get_questions(gid);
        print("Question: " + questionData);
        print("Receiver is active");
        while(true){
            int x = receiver();
            print("Got a request: " + x);
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
