using UnityEngine;
using UnityEngine.UI;
using System.Threading;


public class LobbyControllerScript : MonoBehaviour
{
    // Start is called before the first frame update
    public Text pin_field;
    public Text[] users;
    int counter = 0;
    int toShow = -1;
    string username;
    string standingsData;
    private Thread userCheckThread;
    private Thread standingsThread;
    public Button run_button;
    bool is_running = false;
    void Start()
    {
        userCheckThread = new Thread(checkUsers);
        standingsThread = new Thread(get_standing);
        userCheckThread.Start();
        pin_field.text = IPCManager.instance.game_pin.ToString();
        for(int i = 0; i < 8; i++){
            users[i].gameObject.SetActive(false);
        }
    }
    void Update()
    {
        if(toShow != -1 && toShow < 8){
            users[toShow].text = username;
            users[toShow].gameObject.SetActive(true);
            toShow = -1;
        }
    }

    // Update is called once per frame
    void checkUsers(){
        while(true){
            username = IPCManager.player_join();
            print("New user " + username);
            toShow = counter;
            counter++;
        }
    }

    public void run_game_clicked(){
        if(!is_running){
            userCheckThread.Abort();
            IPCManager.run_game();
            standingsThread.Start();
            run_button.gameObject.GetComponentInChildren<Text>().text = "Exit";
            is_running = true;
        }else{
            standingsThread.Abort();
            ScenesManager.instance.SwitchScene("StartMenu");
        }
        
    }

    void get_standing(){
        while(true){
            standingsData = IPCManager.receive_standing();
            string[] standings = standingsData.Split(',');
            for(int i = 0; i < standings.Length - 1; i++){
                users[i].text = standings[i];
            }
        }
    }
    void OnApplicationQuit()
    {
     userCheckThread.Abort();   
     standingsThread.Abort();   
    }
}
