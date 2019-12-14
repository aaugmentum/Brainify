using UnityEngine;
using UnityEngine.Events;

[CreateAssetMenu(menuName = "Quiz/EventsContainer")]
public class EventsContainer : ScriptableObject
{
    #region Events
    public UnityAction<Question> onUpdateQuestionUI;
    #endregion
}
