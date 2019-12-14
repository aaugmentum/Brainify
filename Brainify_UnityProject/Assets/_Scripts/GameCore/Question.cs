using UnityEngine;

[CreateAssetMenu(menuName = "Quiz/Question")]
public class Question : ScriptableObject
{
    [SerializeField] private int questionIndex;
    public int QuestionIndex => questionIndex;
    
    [SerializeField] private string questionInfo;
    public string QuestionInfo => questionInfo;

    [SerializeField] private AnswerOption[] answerOptions;
    public AnswerOption[] AnswerOptions => answerOptions;

    [SerializeField] private int score = 100;
    public int Score => score;

    public int GetCorrectAnswerIndex()
    {
        int correctAnswerIndex = -1;
        for (int i = 0; i < answerOptions.Length; i++)
        {
            if (answerOptions[i].IsCorrect)
                correctAnswerIndex = i;
        }

        return correctAnswerIndex;
    }
}

[System.Serializable]
public class AnswerOption
{
    [SerializeField] private string answerInfo;
    public string AnswerInfo => answerInfo;

    [SerializeField] private bool isCorrect;
    public bool IsCorrect => isCorrect;
}