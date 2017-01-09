


#ifndef INCLUDED_COLOR_H
#define INCLUDED_COLOR_H

#pragma once

#include <math/vector.h>


inline math::float3 hsv_to_rgb(const math::float3& hsv)
{
  using namespace math;

  float3 rgb = clamp(float3(abs(3.0f - 6.0f * hsv.x) - 1.0f,
                     2.0f - abs(2.0f - 6.0f * hsv.x),
                     2.0f - abs(4.0f - 6.0f * hsv.x)), 0.0f, 1.0f) * hsv.y * hsv.z + hsv.z * (1.0f - hsv.y);
  return rgb;
}

inline math::float3 rgb_to_hsv(const math::float3& c)
{
  using namespace math;

  const float4 cx = float4(1, 0, 1.0f/6.0f, 2.0f/6.0f);

  float3 d = float3(c.y, c.x, c.z) - float3(c.z, c.y, c.x);
  
  float4 c1 = d.x >= 0.0f ? float4(c.x, c.y, c.y, 1) : float4(c.z, c.x, c.y, -1);
  float4 c2 = c1.w * d.y >= 0.0f ? float4(cx.x, cx.y, cx.y, cx.y) : (c1.w * d.z >= 0.0f ? float4(cx.y, cx.x, cx.y, cx.w) : float4(cx.y, cx.y, cx.x, cx.z));
  
  float delta = -dot(float3(d.z, d.y, d.x), float3(c2.x, c2.y, c2.z));
  float max = dot(float3(c1.x, c1.y, c1.z), float3(c2.x, c2.y, c2.z));
  
  return float3(abs(delta) > 0.0f ? (dot(float3(d.x, d.z, d.y), float3(c2.x, c2.y, c2.z)) / delta * (1.0f/6.0f) + c2.w - c1.w * 0.25f + 0.25f) : 0.0f,
                max > 0.0f ? abs(delta) / max : 0.0f,
                max);
}

#endif  //  INCLUDED_COLOR_H
