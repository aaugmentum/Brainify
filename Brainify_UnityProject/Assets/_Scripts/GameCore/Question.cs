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

    public string[] options;
    public int correctAnsIndex = -1;


    [SerializeField] private int score = 100;
    public int Score => score;

    public Question (int index, string info, string[] answers, int correct_answer){
        questionIndex = index;
        questionInfo = info;
        options = answers;
        correctAnsIndex = correct_answer;
        // answerOptions = CreateAnswerOptions(answers, correct_answer);
    }

    private AnswerOption[] CreateAnswerOptions(string[] answers, int correct_answer){
        AnswerOption tempAnswerOption = new AnswerOption();
  
        AnswerOption[] answerOptions = new AnswerOption[4];

        for (int i = 0; i < 4; i++){
            answerOptions[i] = tempAnswerOption;
        }

        for (int i = 0; i < 4; i++){
            answerOptions[i].AnswerInfo = answers[i];
            answerOptions[i].IsCorrect = false;
            if (i == correct_answer)
                answerOptions[i].IsCorrect = true;
        }

        return answerOptions;
    }

    public int GetCorrectAnswerIndex()
    {
        return correctAnsIndex;
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

