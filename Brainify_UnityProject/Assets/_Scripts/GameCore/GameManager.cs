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

    private int _currentQuestionIndex = -1;
    
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
        _currentQuestionIndex++;

        UpdateTimer(false);

        Question question = _questions[_currentQuestionIndex];
        
        events.onUpdateQuestionUI?.Invoke(question);
        
        UpdateTimer(true);
    }

    public void AcceptUserAnswer(int chosenAnswerIndex)
    {        
        bool isAnswerCorrect = CheckUserAnswers(chosenAnswerIndex);
        
        _finishedQuestionsIndexes.Add(_currentQuestionIndex);

        int userScore = (int) (timeLeft * 5);

        if (isAnswerCorrect){
            IPCManager.instance.ipc_answer(userScore);
            UpdateScore(userScore);
        }

        PostQuestionScreenType screenType;
        if (IsFinishedGame)
            screenType = PostQuestionScreenType.FinishClient;
        else
            screenType = (isAnswerCorrect) ? PostQuestionScreenType.Correct : PostQuestionScreenType.Incorrect;

        events.onDisplayPostQuestionScreen?.Invoke(screenType, userScore);   
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
            timerText.text = timeLeft.ToString();
            timeLeft--;
            yield return new WaitForSeconds(1.0f);
        }
    }

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
    }
}