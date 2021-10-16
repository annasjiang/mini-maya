#version 150
// ^ Change this to version 130 if you have compatibility issues

in vec2 vs_jointWeights;
in ivec2 vs_jointIDs;

uniform mat4 u_Model;
uniform mat4 u_ViewProj;

uniform mat4 u_bindMats[100];
uniform mat4 u_overallTrans[100];

in vec4 vs_Pos;
in vec4 vs_Col;

out vec4 fs_Col;

void main()
{
    fs_Col = vs_Col;

    vec4 pos1 = u_overallTrans[vs_jointIDs[0]] * u_bindMats[vs_jointIDs[0]] * vs_Pos;
    vec4 pos2 = u_overallTrans[vs_jointIDs[1]] * u_bindMats[vs_jointIDs[1]] * vs_Pos;

    vec4 pos = vs_jointWeights[0] * pos1 + vs_jointWeights[1] * pos2;
    vec4 modelposition = pos;

    gl_Position = u_ViewProj * modelposition;
}

