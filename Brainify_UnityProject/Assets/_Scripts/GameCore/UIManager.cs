using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    [SerializeField] private EventsContainer events;
    
    [Space]
    [Header("Question Canvas Group")]
    [SerializeField] private CanvasGroup mainCanvasGroup;
    public CanvasGroup MainCanvasGroup => mainCanvasGroup;
    
    [Space]
    [Header("Answer Button")]
    [SerializeField] private UserAnswerData answerPrefab;
    
    [Space]
    [Header("Question Fields")]
    [SerializeField] private Text questionInfoText;
    public Text QuestionInfoText => questionInfoText;
    [SerializeField] private Button[] answerButtons;

    [Space]
    [Header("Post Question Screen Fields")]
    [SerializeField] private Image postQuestionBG;
    public Image PostQuestionBg => postQuestionBG;
    [SerializeField] private Text postQuestionTitleText;
    public Text PostQuestionTitleText => postQuestionTitleText;
    [SerializeField] private Text postQuestionScoreText;
    public Text PostQuestionScoreText => postQuestionScoreText;
    [SerializeField] private RectTransform finishUI;
    public RectTransform FinishUI => finishUI;
    
    private List<UserAnswerData> _currentAnswersData = new List<UserAnswerData>();

    private void OnEnable()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
        events.onUpdateQuestionUI += UpdateQuestionUI;
    }

    private void OnDisable()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
    }

    private void OnDestroy()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
    }

    private void UpdateQuestionUI(Question question)
    {
        QuestionInfoText.text = question.QuestionInfo;
        InitializeAnswers(question);
    }

    private void InitializeAnswers(Question question)
    {
        EraseAnswers();
        
        for (int i = 0; i < question.AnswerOptions.Length; i++)
        {
            string answerInfo = question.AnswerOptions[i].AnswerInfo;
            
            UserAnswerData newAnswerData = answerButtons[i].GetComponent<UserAnswerData>();
            newAnswerData.UpdateData(answerInfo);
            
            _currentAnswersData.Add(newAnswerData);
        }
    }

    private void EraseAnswers()
    {
        for (int i = 0; i < _currentAnswersData.Count; i++)
        {
            _currentAnswersData[i].AnswerText = null;
        }
        
        _currentAnswersData.Clear();
    }
}

public enum PostQuestionScreenType
{
    Correct,
    Incorrect,
    Finish
}