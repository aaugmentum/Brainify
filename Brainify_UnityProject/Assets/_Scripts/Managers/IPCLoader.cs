using UnityEngine;
using UnityEngine.UI;

public class IPCLoader : MonoBehaviour
{
    #region IPCFunctions Calls

    [Header("Login fields")]
    public InputField username;
    public InputField password;
    public InputField c_password;
    
    public void signin_button(){
        int result = IPCManager.signin(username.text, password.text);
        if(result == 1){
            ScenesManager.instance.SwitchScene("StartMenu");
        }else{
            print("Wrong username or password");
        }
    }

    public void register_button(){
        if(c_password.text.Equals(password.text)){
            int result = IPCManager.signup(username.text, password.text);
            if(result == 1){
                ScenesManager.instance.SwitchScene("Signin");
            }else{
                print("User already exists");
            }
        }else{
            print("Password do not match.");
        }
    }

    #endregion
}
