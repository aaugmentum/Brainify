Shader "UI/Fast-Additive"
{
    Properties
    {
        _Color ("Tint", Color) = (1,1,1,1)
    }
 
    CGINCLUDE
    #include "UnityCG.cginc"
    #include "UnityUI.cginc"
 
    fixed4 _Color;
    fixed4 _TextureSampleAdd;
 
    struct appdata_t
    {
        float4 vertex   : POSITION;
        float4 color    : COLOR;
        float2 texcoord : TEXCOORD0;
    };
 
    struct v2f
    {
        float4 vertex   : SV_POSITION;
        fixed4 color    : COLOR;
        half2 texcoord  : TEXCOORD0;
    };
 
    v2f vert(appdata_t IN)
    {
        v2f OUT;
        OUT.vertex = UnityObjectToClipPos(IN.vertex);
 
        OUT.texcoord = IN.texcoord;
     
        #ifdef UNITY_HALF_TEXEL_OFFSET
        OUT.vertex.xy += (_ScreenParams.zw-1.0)*float2(-1,1);
        #endif
     
        OUT.color = IN.color * _Color;
        return OUT;
    }
 
    sampler2D _MainTex;
    fixed4 frag(v2f IN) : SV_Target
    {
        return (tex2D(_MainTex, IN.texcoord) + _TextureSampleAdd) * IN.color;
    }
    ENDCG
 
    SubShader
    {
        Tags
        {
            "Queue"="Transparent"
            "IgnoreProjector"="True"
            "RenderType"="Transparent"
            "PreviewType"="Plane"
            "CanUseSpriteAtlas"="True"
        }
 
        Cull Off
        Lighting Off
        ZWrite Off
        ZTest [unity_GUIZTestMode]
        Blend SrcAlpha One
 
        Pass
        {
        CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
        ENDCG
        }
    }
}