using UnityEngine;
using System.Collections;
using System.Collections.Generic;

// [CreateAssetMenu(menuName = "Quiz/Question")]
public class Question
{
    [SerializeField] private int questionIndex;
    public int QuestionIndex => questionIndex;
    
    [SerializeField] private string questionInfo;
    public string QuestionInfo => questionInfo;

    [SerializeField] public AnswerOption[] answerOptions;
    public AnswerOption[] AnswerOptions => answerOptions;

    [SerializeField] private int score = 100;
    public int Score => score;

    // constructor
    public Question (int index, string info, string[] answers, int correct_answer){
        questionIndex = index;
        questionInfo = info;
        answerOptions = CreateAnswerOptions(answers, correct_answer);
    }

    private AnswerOption[] CreateAnswerOptions(string[] answers, int correct_answer){
        AnswerOption tempAnswerOption = new AnswerOption();
        Debug.Log($"TempAnswer: {tempAnswerOption.AnswerInfo} : {tempAnswerOption.IsCorrect}");


        AnswerOption[] answerOptions = new AnswerOption[4];

        for (int i = 0; i < 4; i++){
            answerOptions[i] = tempAnswerOption;
        }

        for (int i = 0; i < 4; i++){
            Debug.Log($"Answer{i}: {tempAnswerOption.AnswerInfo} : {tempAnswerOption.IsCorrect}");

            answerOptions[i].AnswerInfo = answers[i];
            answerOptions[i].IsCorrect = false;
            if (i == correct_answer)
                answerOptions[i].IsCorrect = true;
        }

        return answerOptions;
    }

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
    public string AnswerInfo
    {
        get => AnswerInfo;
        set => AnswerInfo = value;
    }

    [SerializeField] private bool isCorrect;
    public bool IsCorrect
    {
        get => IsCorrect;
        set => IsCorrect = value;
    }

    public AnswerOption(){
        answerInfo = "answer";
        isCorrect = false;
    }
}

