using UnityEngine;

public class Sound : MonoBehaviour
{
    public AudioClip defaultClip;
    public AudioClip[] clips;

    public void PlayDefaultSound()
    {
        SoundsManager.instance.PlaySound(defaultClip);
    }

    public void PlaySoundClip(int clipIndex = 0)
    {
        if (clipIndex >= clips.Length) return;
        SoundsManager.instance.PlaySound(clips[clipIndex]);
    }
}
