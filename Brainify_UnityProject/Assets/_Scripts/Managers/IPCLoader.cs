using UnityEngine;
using UnityEngine.UI;

public class IPCLoader : MonoBehaviour
{
    #region IPCFunctions Calls

    [Header("Login fields")]
    public InputField username;
    public InputField password;
    public InputField c_password;
    public Text error_message;

    [Header("Game buttons")]
    public Button[] game_buttons;
    
    public void signin_button(){
        int result = IPCManager.signin(username.text, password.text);
        if(result == 1){
            ScenesManager.instance.SwitchScene("StartMenu");
            PlayerPrefs.SetString("username", username.text);
            PlayerPrefs.SetString("password", password.text);
            PlayerPrefs.SetInt("is_logged_in", 1);
        }else{
            print("Wrong username or password");
            error_message.text = "Wrong username or password";

        }
    }

    public void register_button(){
        error_message.text = " ";

        if(c_password.text.Equals(password.text)){
            int result = IPCManager.signup(username.text, password.text);
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
    }

    #endregion
}
