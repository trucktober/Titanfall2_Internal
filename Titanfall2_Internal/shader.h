#pragma once

constexpr const char* shader = R"(

cbuffer ConstantBuffer : register(b0)
{
    matrix projection;
}

struct build 
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

build VS(float4 pos : POSITION, float4 color : COLOR ) 
{
    build s;
    s.pos = mul(projection, pos);
    s.color = color;
    return s;
}

float4 PS(build s) : SV_TARGET
{
    return s.color;
}
)";
