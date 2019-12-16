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
    string name_to_put;
    private Thread userCheckThread;
    public Button run_button;
    void Start()
    {
        userCheckThread = new Thread(checkUsers);
        userCheckThread.Start();
        pin_field.text = IPCManager.instance.game_pin.ToString();
        for(int i = 0; i < 8; i++){
            users[i].gameObject.SetActive(false);
        }
    }
    void Update()
    {
        if(toShow != -1 && toShow < 8){
            users[toShow].text = name_to_put;
            users[toShow].gameObject.SetActive(true);
            toShow = -1;
        }
    }

    // Update is called once per frame
    void checkUsers(){
        string username;
        while(true){
            username = IPCManager.player_join();
            print("New user");
            toShow = counter;
            name_to_put = username;
            counter++;
        }
    }

    void run_game_clicked(){
        userCheckThread.Abort();
        IPCManager.run_game();
        run_button.gameObject.SetActive(false);
    }
}
