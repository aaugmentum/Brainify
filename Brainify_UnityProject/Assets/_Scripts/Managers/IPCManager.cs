using UnityEngine;
using System.Runtime.InteropServices;

public class IPCManager : MonoBehaviour
{
    [DllImport ("IPCPlugin")] 
    public static extern int signin(string username, string password);
    [DllImport ("IPCPlugin")] 
    public static extern int signup(string username, string password);
    [DllImport ("IPCPlugin")] 
    public static extern int connect_server();

    public static IPCManager instance;

    private void Awake()
    {
        instance = this;
    }
    void Start(){
        int x = IPCManager.connect_server();
        print(x);
    }

    #region IPCFunctions

    #endregion
}
