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
    string stringData;
	string [] standings;
    private Thread lobbyReceiverThread;
    public Button run_button;
    // public Button back_button;
    bool is_running = false;
    private bool is_lobby_thread_active = false;
    void Start()
    {
        lobbyReceiverThread = new Thread(lobbyReceiver);
        print("Connection: " + IPCManager.instance.is_connected);
        is_lobby_thread_active = true;
        lobbyReceiverThread.Start();
        pin_field.text = IPCManager.instance.game_pin.ToString();
        for (int i = 0; i < 8; i++)
        {
            users[i].gameObject.SetActive(false);
        }
    }
    void Update()
    {
        if (toShow != -1 && toShow < 8)
        {
            users[toShow].text = stringData;
            users[toShow].gameObject.SetActive(true);
            toShow = -1;
        }
		if(toShow == 10){
			for(int i = 0; i < standings.Length - 1; i++){
				users[i].text = standings[i];
			}
			toShow = -1;
		}
    }

    public void run_game_clicked()
    {
        if (!is_running)
        {
            if (IPCManager.instance.is_connected)
            {
                IPCManager.instance.ipc_run_game();
                run_button.gameObject.GetComponentInChildren<Text>().text = "Exit";
                is_running = true;
            }
        }
        else
        {
            lobbyReceiverThread.Abort();
            is_lobby_thread_active = false;
            ScenesManager.instance.SwitchScene("StartMenu");
        }

    }

    void lobbyReceiver()
    {
        while (is_lobby_thread_active && IPCManager.instance.is_connected)
        {
			stringData = IPCManager.instance.ipc_standing();
			if(stringData.Equals("Rasengan")){
				print("Game finished");
				break;
			}
            if (!is_running)
            {
                print("New user connected: " + stringData);
                toShow = counter;
                if (counter < 8) counter++;
            }
            else
            {
                standings = stringData.Split(',');
                // for (int i = 0; i < standings.Length - 1; i++)
                // {
                //     users[i].text = standings[i];
                // }
				toShow = 10;
            }

        }
    }
    void OnDestroy()
    {
        if (lobbyReceiverThread.IsAlive) lobbyReceiverThread.Abort();
        print("Lobby thread: " + lobbyReceiverThread.IsAlive);
        is_lobby_thread_active = false;
    }
    void OnApplicationQuit()
    {
        if (lobbyReceiverThread.IsAlive) lobbyReceiverThread.Abort();
        print("Lobby thread: " + lobbyReceiverThread.IsAlive);
        is_lobby_thread_active = false;
    }
}