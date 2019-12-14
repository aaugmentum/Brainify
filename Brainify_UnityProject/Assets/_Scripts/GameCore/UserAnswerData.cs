using UnityEngine;
using UnityEngine.UI;

public class UserAnswerData : MonoBehaviour
{
    [SerializeField] private EventsContainer events;
    
    [SerializeField] private Text answerText;
    public Text AnswerText
    {
        get => answerText;
        set => answerText = value;
    }
    
    private int _answerIndex;
    public int AnswerIndex
    {
        get => _answerIndex;
        set => _answerIndex = value;
    }

    public void UpdateData(int index, string text)
    {
        _answerIndex = index;
        answerText.text = text;
    }
}
