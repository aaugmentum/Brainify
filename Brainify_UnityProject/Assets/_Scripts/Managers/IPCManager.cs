using UnityEngine;

public class IPCManager : MonoBehaviour
{
    public static IPCManager instance;

    private void Awake()
    {
        instance = this;
    }

    #region IPCFunctions

    #endregion
}
