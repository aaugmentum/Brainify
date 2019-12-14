using UnityEngine;
using UnityEngine.UI;

public class UserAnswerData : MonoBehaviour
{
    [SerializeField] private Text answerText;
    public Text AnswerText
    {
        get => answerText;
        set => answerText = value;
    }
    
    [SerializeField] private int _answerIndex;
    public int AnswerIndex
    {
        get => _answerIndex;
        set => _answerIndex = value;
    }

    public void UpdateData(string text)
    {
        answerText.text = text;
    }
}
