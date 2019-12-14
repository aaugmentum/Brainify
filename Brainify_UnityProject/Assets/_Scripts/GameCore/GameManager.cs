using System.Collections.Generic;
using UnityEngine;
using Random = UnityEngine.Random;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;

    [SerializeField] private EventsContainer _events;
    
    private Question[] _questions;
    public Question[] Questions
    {
        get => _questions;
        set => _questions = value;
    }

    private int _currentQuestionIndex = -1;
    
    private int score;

    private UserAnswerData _userPickedAnswer;
    
    private List<int> _finishedQuestionsIndexes;
    public List<int> FinishedQuestionsIndexes => _finishedQuestionsIndexes;
    
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
        _events.onUpdateQuestionUI?.Invoke(question);
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
