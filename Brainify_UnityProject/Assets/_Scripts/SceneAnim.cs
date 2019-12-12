using UnityEngine;
using DG.Tweening;
using Random = UnityEngine.Random;

public class SceneAnim : MonoBehaviour
{
    [SerializeField] private GameObject[] flyingObjects;
    [SerializeField] private Vector2 offsetY = new Vector2(0.1f,0.5f);
    [SerializeField] private Vector2 duration = new Vector2(1f,5f);
    
    private float _randomDuration, _randomOffsetY;

    private Tweener[] _flyingTweens;

    private Vector3[] _originalPosition;

    private void Awake()
    {
        _flyingTweens = new Tweener[flyingObjects.Length];
        _originalPosition = new Vector3[flyingObjects.Length];

        for (int i = 0; i < flyingObjects.Length; i++)
            _originalPosition[i] = flyingObjects[i].transform.localPosition;
    }

    private void Start()
    {
        StartAnimations();
    }

    private void StartAnimations()
    {
        for (int i = 0; i < flyingObjects.Length; i++)
        {
            _randomOffsetY = Random.Range(offsetY.x, offsetY.y);
            _randomDuration = Random.Range(duration.x, duration.y);
            Vector3 newPosition = flyingObjects[i].transform.localPosition + new Vector3(0f, _randomOffsetY, 0f);
            
            _flyingTweens[i] = flyingObjects[i].transform.DOLocalMove(newPosition, _randomDuration)
                .SetEase(Ease.InOutFlash)
                .SetLoops(-1, LoopType.Yoyo);
        }
    }
    
    private void StopAnimations(float returnDuration)
    {
        for (int i = 0; i < _flyingTweens.Length; i++)
        {
            if (_flyingTweens[i] == null)
                continue;
                
            _flyingTweens[i].Kill();
            _flyingTweens[i] = flyingObjects[i].transform.DOLocalMove(_originalPosition[i], returnDuration).SetEase(Ease.InOutSine);
        }
    }
}
