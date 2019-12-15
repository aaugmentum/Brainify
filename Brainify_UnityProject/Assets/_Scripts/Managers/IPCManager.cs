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
    public static extern string games();
    [DllImport ("IPCPlugin")] 
    public static extern string get_questions(string gid);

    [DllImport ("IPCPlugin")] 
    public static extern void logout();


    public static IPCManager instance;
    public SortedDictionary<string, string> gamesMap = new SortedDictionary<string, string>();
    public int result;
    public Thread connectionThread;
    public string question;
    private void Awake()
    {
        instance = this;
    }
    void Start(){
        result = 0;
        connectionThread = new Thread(Connect);
        connectionThread.Start();
    }

    void Update()
    {
        if(result == -1){
            print("Closed thread");
            result = 2;
        }
    }
    void OnApplicationQuit()
    {
        logout();
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
            result = IPCManager.connect_server();
            print(result == 1 ? "Connected" : "Error");
            getGames();
            if(result == 1)connectionThread.Abort();
    }

    public void Logout(){
        logout();
        PlayerPrefs.SetInt("is_logged_in", 0);
    }

    #region IPCFunctions

    #endregion
}
