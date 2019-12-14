using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;

    [SerializeField] private EventsContainer events;
    
    private Question[] _questions;
    public Question[] Questions
    {
        get => _questions;
        set => _questions = value;
    }

    private int _currentQuestionIndex = -1;
    
    private List<int> _finishedQuestionsIndexes;
    public List<int> FinishedQuestionsIndexes => _finishedQuestionsIndexes;

    [HideInInspector] public int currentScore;
    
    private void Awake()
    {
        if (instance == null)
            instance = this;
        
        _finishedQuestionsIndexes = new List<int>();
    }

    private void Start()
    {
        LoadQuestions();
        DisplayQuestion();
    }

    public void DisplayQuestion()
    {
        Question question = GetRandomQuestion();
        
        _finishedQuestionsIndexes.Add(_currentQuestionIndex);
        _currentQuestionIndex = question.QuestionIndex;
        
        events.onUpdateQuestionUI?.Invoke(question);
    }

    public void AcceptUserAnswer(int chosenAnswerIndex)
    {
        bool isAnswerCorrect = CheckUserAnswers(chosenAnswerIndex);
        
        // Debug.Log($"IsCorrect: {isAnswerCorrect}");
        
        _finishedQuestionsIndexes.Add(_currentQuestionIndex);

        if (isAnswerCorrect)
            UpdateScore(_questions[_currentQuestionIndex].Score);
    }

    private Question GetRandomQuestion()
    {
        int randomIndex = GetRandomQuestionIndex();
        return _questions[randomIndex];
    }

    private int GetRandomQuestionIndex()
    {
        int randomIndex = -1;
        if (_finishedQuestionsIndexes.Count < Questions.Length)
        {
            do
            {
                randomIndex = Random.Range(0, Questions.Length);
            } while (_finishedQuestionsIndexes.Contains(randomIndex) || randomIndex == _currentQuestionIndex);
        }
        return randomIndex;
    }

    private bool CheckUserAnswers(int chosenAnswerIndex)
    {
        int correctAnswerOptionIndex = _questions[_currentQuestionIndex].GetCorrectAnswerIndex();
        return (correctAnswerOptionIndex == chosenAnswerIndex);
    }
    
    public void UpdateScore(int addScore)
    {
        currentScore += addScore;
    }
    
    private void LoadQuestions()
    {
        Object[] objects = Resources.LoadAll(Constants.PathQuestions, typeof(Question));
        _questions = new Question[objects.Length];

        for (int i = 0; i < _questions.Length; i++)
        {
            _questions[i] = (Question) objects[i];
        }
    }
}