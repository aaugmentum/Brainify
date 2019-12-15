using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;

public class UIManager : MonoBehaviour
{
    [SerializeField] private EventsContainer events;
    
    [Header("Question Canvas Group")]
    [SerializeField] private CanvasGroup mainCanvasGroup;
    
    [Header("Question Fields")]
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

    private void UpdateQuestionUI(Question question)
    {
        questionInfoText.text = question.QuestionInfo;
        InitializeAnswers(question);
    }

    private void DisplayPostQuestionScreenUI(PostQuestionScreenType type, int addScore)
    {
        UpdatePostQuestionScreenUI(type, addScore);
        postQuestionAnimator.SetInteger(ScreenState, 2);
        mainCanvasGroup.blocksRaycasts = false;

        if (type.Equals(PostQuestionScreenType.Correct) || type.Equals(PostQuestionScreenType.Incorrect))
        {
            if (_IE_HoldPostQuestionScreen != null)
                StopCoroutine(_IE_HoldPostQuestionScreen);

            _IE_HoldPostQuestionScreen = HoldPostQuestionScreenUI();
            StartCoroutine(_IE_HoldPostQuestionScreen);
        }
    }

    IEnumerator HoldPostQuestionScreenUI()
    {
        yield return new WaitForSeconds(Constants.PostQuestionTime);
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
            case PostQuestionScreenType.Finish:
                postQuestionBG.color = screenColorFinish;
                postQuestionTitleText.gameObject.SetActive(false);
                postQuestionSubtitleText.gameObject.SetActive(false);
                finishUI.gameObject.SetActive(true);
                break;
        }
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
            _currentAnswersData[i].AnswerText.text = string.Empty;
        
        _currentAnswersData.Clear();
    }
}

public enum PostQuestionScreenType
{
    Correct,
    Incorrect,
    Waiting,
    Finish
}