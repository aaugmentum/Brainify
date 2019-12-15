using System.Collections;
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

    private int _currentQuestionIndex = 0;
    private IEnumerator _IE_WaitForNextQuestion;

    private UserAnswerData _chosenAnswer;
    private List<int> _finishedQuestionsIndexes;
    public List<int> FinishedQuestionsIndexes => _finishedQuestionsIndexes;
    public bool IsFinishedGame { get => (_finishedQuestionsIndexes.Count >= _questions.Length); }

    
    private void Awake()
    {
        events.currentScore = 0;
        
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
        Debug.Log($"Finished question: {_finishedQuestionsIndexes.Count}");
        for (int i = 0; i < _finishedQuestionsIndexes.Count; i++)
        {
            Debug.Log($"{i}: {_finishedQuestionsIndexes[i]}");
        }
        Question question = GetRandomQuestion();
        
        events.onUpdateQuestionUI?.Invoke(question);
    }

    public void AcceptUserAnswer(int chosenAnswerIndex)
    {
        bool isAnswerCorrect = CheckUserAnswers(chosenAnswerIndex);
        
        _finishedQuestionsIndexes.Add(_currentQuestionIndex);

        if (isAnswerCorrect)
            UpdateScore(_questions[_currentQuestionIndex].Score);

        PostQuestionScreenType screenType;
        if (IsFinishedGame)
            screenType = PostQuestionScreenType.Finish;
        else
            screenType = (isAnswerCorrect) ? PostQuestionScreenType.Correct : PostQuestionScreenType.Incorrect;

        events.onDisplayPostQuestionScreen?.Invoke(screenType, _questions[_currentQuestionIndex].Score);

        Debug.Log($"IsCorrect: {isAnswerCorrect} \t Score: {events.currentScore}");

        if (screenType.Equals(PostQuestionScreenType.Correct) || screenType.Equals(PostQuestionScreenType.Incorrect))
        {
            if (_IE_WaitForNextQuestion != null)
                StopCoroutine(_IE_WaitForNextQuestion);

            _IE_WaitForNextQuestion = WaitForNextQuestion();
            StartCoroutine(_IE_WaitForNextQuestion);
        }
    }

    IEnumerator WaitForNextQuestion()
    {
        yield return new WaitForSeconds(Constants.PostQuestionTime);
        DisplayQuestion();
    }

    private Question GetRandomQuestion()
    {
        int randomIndex = GetRandomQuestionIndex();
        _currentQuestionIndex = randomIndex;
        
        return _questions[randomIndex];
    }

    private int GetRandomQuestionIndex()
    {
        int randomIndex = 0;
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
        events.currentScore += addScore;
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