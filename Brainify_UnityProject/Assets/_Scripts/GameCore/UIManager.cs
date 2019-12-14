using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    [SerializeField] private EventsContainer events;
    
    [Space]
    [SerializeField] private CanvasGroup mainCanvasGroup;
    public CanvasGroup MainCanvasGroup => mainCanvasGroup;
    
    [Space]
    [SerializeField] private UserAnswerData answerPrefab;
    
    [Space]
    [SerializeField] private Text questionInfoText;
    public Text QuestionInfoText => questionInfoText;
    [SerializeField] private RectTransform answerButtonsParent;
    public RectTransform AnswerButtonsParent => answerButtonsParent;

    [Space]
    [SerializeField] private Image postQuestionBG;
    public Image PostQuestionBg => postQuestionBG;
    [SerializeField] private Text postQuestionTitleText;
    public Text PostQuestionTitleText => postQuestionTitleText;
    [SerializeField] private Text postQuestionScoreText;
    public Text PostQuestionScoreText => postQuestionScoreText;
    [SerializeField] private RectTransform finishUI;
    public RectTransform FinishUI => finishUI;
    
    private List<UserAnswerData> _currentAnswerOptions = new List<UserAnswerData>();

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
        DestroyAnswers();
        
        for (int i = 0; i < question.AnswerOptions.Length; i++)
        {
            string answerInfo = question.AnswerOptions[i].AnswerInfo;
            int answerIndex = i;
            
            UserAnswerData newAnswerData = Instantiate(answerPrefab, AnswerButtonsParent);
            newAnswerData.UpdateData(answerIndex, answerInfo);
            
            _currentAnswerOptions.Add(newAnswerData);
        }
    }

    private void DestroyAnswers()
    {
        for (int i = 0; i < _currentAnswerOptions.Count; i++)
            Destroy(_currentAnswerOptions[i].gameObject);
        
        _currentAnswerOptions.Clear();
    }
}

public enum PostQuestionScreenType
{
    Correct,
    Incorrect,
    Finish
}