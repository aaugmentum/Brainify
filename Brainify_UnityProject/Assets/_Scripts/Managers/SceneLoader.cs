using UnityEngine;

public class SceneLoader : MonoBehaviour
{
    public void LoadScene(string sceneName)
    {
        ScenesManager.instance.SwitchScene(sceneName);
    }

    public void QuitGame()
    {
        ScenesManager.instance.Quit();
    }
}
