using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    public static UIManager instance;

    [SerializeField] private EventsContainer events;
    
    [Header("Question Canvas Group")]
    [SerializeField] private CanvasGroup mainCanvasGroup;
    
    [Header("Question Fields")]
    [SerializeField] private Text questionIndexText;
    [SerializeField] private Text questionInfoText;
    [SerializeField] private Button[] answerButtons;

    [Header("Post Question Screen Fields")] 
    [SerializeField] private Animator postQuestionAnimator;
    [SerializeField] private Image postQuestionBG;
    [SerializeField] private Text postQuestionTitleText;
    [SerializeField] private Text postQuestionSubtitleText;
    [SerializeField] private RectTransform finishUI;

    [Header("Post Question Screen Params")]
    [SerializeField] private Color screenColorCorrect;
    [SerializeField] private Color screenColorIncorrect;
    [SerializeField] private Color screenColorWaiting;
    [SerializeField] private Color screenColorFinish;
    
    private static readonly int ScreenState = Animator.StringToHash("ScreenState");

    private IEnumerator _IE_HoldPostQuestionScreen;
    
    private List<UserAnswerData> _currentAnswersData = new List<UserAnswerData>();

    private void OnEnable()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
        events.onUpdateQuestionUI += UpdateQuestionUI;

        events.onDisplayPostQuestionScreen -= DisplayPostQuestionScreenUI;
        events.onDisplayPostQuestionScreen += DisplayPostQuestionScreenUI;
    }

    private void OnDisable()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
        events.onDisplayPostQuestionScreen -= DisplayPostQuestionScreenUI;
    }

    private void OnDestroy()
    {
        events.onUpdateQuestionUI -= UpdateQuestionUI;
        events.onDisplayPostQuestionScreen -= DisplayPostQuestionScreenUI;
    }

    private void Awake() {
        if (instance == null){
            instance = this;
        }
    }

    private void UpdateQuestionUI(Question question)
    {
        questionIndexText.text = "Question " + (question.QuestionIndex + 1).ToString();
        questionInfoText.text = question.QuestionInfo;
        InitializeAnswers(question);
    }

    private void DisplayPostQuestionScreenUI(PostQuestionScreenType type, int addScore)
    {
        UpdatePostQuestionScreenUI(type, addScore);
        postQuestionAnimator.SetInteger(ScreenState, 2);
        mainCanvasGroup.blocksRaycasts = false;        
    }

    public void FadeOutPostQuestionScreenUI(){
        postQuestionAnimator.SetInteger(ScreenState, 1);
        mainCanvasGroup.blocksRaycasts = true;
    }
    
    private void UpdatePostQuestionScreenUI(PostQuestionScreenType type, int addScore)
    {
        switch (type)
        {
            case PostQuestionScreenType.Correct:
                postQuestionBG.color = screenColorCorrect;
                postQuestionTitleText.text = Constants.PostQuestionTextCorrect;
                postQuestionSubtitleText.text = "+" + addScore + " points";
                break;
            case PostQuestionScreenType.Incorrect:
                postQuestionBG.color = screenColorIncorrect;
                postQuestionTitleText.text = Constants.PostQuestionTextIncorrect;
                postQuestionSubtitleText.text = "+0 points";
                break;
            case PostQuestionScreenType.Waiting:
                postQuestionBG.color = screenColorWaiting;
                postQuestionTitleText.text = Constants.PostQuestionTextWaiting1;
                postQuestionSubtitleText.text = Constants.PostQuestionTextWaiting2;
                break;
            case PostQuestionScreenType.FinishClient:
                postQuestionBG.color = screenColorFinish;
                postQuestionTitleText.text = Constants.PostQuestionTextFinish;
                postQuestionSubtitleText.text = Constants.PostQuestionTextFinishScore + events.currentScore + " points";
                break;
        }
    }

    private void InitializeAnswers(Question question)
    {
        EraseAnswers();
        
        for (int i = 0; i < 4; i++)
        {
            string answerInfo = question.options[i];
            
            UserAnswerData newAnswerData = answerButtons[i].GetComponent<UserAnswerData>();
            newAnswerData.UpdateData(answerInfo);
            
            _currentAnswersData.Add(newAnswerData);
        }
    }

    private void EraseAnswers()
    {
        for (int i = 0; i < _currentAnswersData.Count; i++)
            _currentAnswersData[i].AnswerText.text = string.Empty;
        
        _currentAnswersData.Clear();
    }
}

public enum PostQuestionScreenType
{
    Correct,
    Incorrect,
    Waiting,
    FinishClient,
    Finish
}