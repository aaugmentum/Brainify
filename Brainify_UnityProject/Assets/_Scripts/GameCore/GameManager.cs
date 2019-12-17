using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using Random = UnityEngine.Random;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;

    [SerializeField] private EventsContainer events;
    
    [SerializeField] private Text timerText;

    public float timeLeft;

    public List<Question> _questions = new List<Question>();
    // public Question[] Questions
    // {
    //     get => _questions;
    //     set => _questions = value;
    // }


    private int _currentQuestionIndex = 0;
    
    private IEnumerator _IE_WaitForNextQuestion;
    private IEnumerator _IE_Timer;

    private UserAnswerData _chosenAnswer;
    
    private List<int> _finishedQuestionsIndexes;
    
    public bool IsFinishedGame { get => (_finishedQuestionsIndexes.Count >= _questions.Count); }

    
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
        // Question question = GetRandomQuestion();
        // TODO Check TIMER
        UpdateTimer(false);

        Question question = _questions[_currentQuestionIndex];
        _currentQuestionIndex++;
        
        events.onUpdateQuestionUI?.Invoke(question);
        
        UpdateTimer(true);
    }

    public void AcceptUserAnswer(int chosenAnswerIndex)
    {
        UpdateTimer(false);
        
        bool isAnswerCorrect = CheckUserAnswers(chosenAnswerIndex);
        
        _finishedQuestionsIndexes.Add(_currentQuestionIndex);

        if (isAnswerCorrect){
            IPCManager.instance.ipc_answer((int) (timeLeft * 5));
            UpdateScore((int) (timeLeft * 5));
        }

        PostQuestionScreenType screenType;
        if (IsFinishedGame)
            screenType = PostQuestionScreenType.FinishClient;
        else
            screenType = (isAnswerCorrect) ? PostQuestionScreenType.Correct : PostQuestionScreenType.Incorrect;

        events.onDisplayPostQuestionScreen?.Invoke(screenType, (int) (timeLeft * 5));   

        if (screenType.Equals(PostQuestionScreenType.Correct) || screenType.Equals(PostQuestionScreenType.Incorrect))
        {
            if (_IE_WaitForNextQuestion != null)
                StopCoroutine(_IE_WaitForNextQuestion);

            _IE_WaitForNextQuestion = WaitForNextQuestion();
            StartCoroutine(_IE_WaitForNextQuestion);
        }
    }

    private IEnumerator WaitForNextQuestion()
    {
        yield return new WaitForSeconds(Constants.PostQuestionTime);
        // DisplayQuestion();
    }
    
    private void UpdateTimer(bool shouldTimerTick)
    {
        if (shouldTimerTick)
        {
            _IE_Timer = Timer();
            StartCoroutine(_IE_Timer);
        }
        else
        {
            if (_IE_Timer != null)
                StopCoroutine(_IE_Timer);
        }
    }
    
    private IEnumerator Timer()
    {
        float timeTotal = Constants.RoundTimer;
        timeLeft = timeTotal;

        while (timeLeft > 0)
        {
            timeLeft--;
            timerText.text = timeLeft.ToString();
            yield return new WaitForSeconds(1.0f);
        }
    }

    // private Question GetRandomQuestion()
    // {
    //     int randomIndex = GetRandomQuestionIndex();
    //     _currentQuestionIndex = randomIndex;
        
    //     return _questions[randomIndex];
    // }

    // private int GetRandomQuestionIndex()
    // {
    //     int randomIndex = 0;
    //     if (_finishedQuestionsIndexes.Count < _questions.Count)
    //     {
    //         do
    //         {
    //             randomIndex = Random.Range(0, _questions.Count);
    //         } while (_finishedQuestionsIndexes.Contains(randomIndex) || randomIndex == _currentQuestionIndex);
    //     }
    //     return randomIndex;
    // }

    private bool CheckUserAnswers(int chosenAnswerIndex)
    {
        int correctAnswerOptionIndex = _questions[_currentQuestionIndex].GetCorrectAnswerIndex();
        return (correctAnswerOptionIndex == chosenAnswerIndex);
    }
    
    private void UpdateScore(int addScore)
    {
        events.currentScore += addScore;
    }
    
    private void LoadQuestions()
    {
        if (!IPCManager.instance.answers_received)
            return;

        string[] splitted_questions = IPCManager.instance.questionData.Split('$');
        for (int i = 0; i < splitted_questions.Length - 1; i++)
        {
            print(splitted_questions[i]);
            string[] question_items = splitted_questions[i].Split('^');
            string[] question_answers = question_items[1].Split('@');
            print("Correct answer is" + question_items[2]);
            int correctAnswerIndex = System.Convert.ToInt32(question_items[2]);
            print(question_items[0]);
            print(question_items[1]);
            print("THERE ARE " + question_answers.Length + " ANSWERS");
            print(correctAnswerIndex);

            _questions.Add(new Question(i, question_items[0], question_answers, correctAnswerIndex));
        }

        // Object[] objects = Resources.LoadAll(Constants.PathQuestions, typeof(Question));
        // _questions = new Question[objects.Length];

        // for (int i = 0; i < _questions.Length; i++)
        // {
        //     _questions[i] = (Question) objects[i];
        // }
    }

    public void DisplayQuestionIndex(){
        // TODO
    }
}