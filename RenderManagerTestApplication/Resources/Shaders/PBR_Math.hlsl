static const float M_PI = 3.14159265f;
static const float EPSILON = 1e-6f;

/************************************************/
////////////////////Specular D////////////////////
/************************************************/

// GGX specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float
Specular_D_GGX(float alpha, float NdotH) {
  const float alpha2 = alpha * alpha;

  const float denom = (NdotH * NdotH) * (alpha2 - 1.0f) + 1.0f;
  return alpha2 * rcp(max(EPSILON, M_PI * denom * denom));
};

/************************************************/
////////////////////Specular F////////////////////
/************************************************/

// Shlick's approximation of Fresnel
// https://en.wikipedia.org/wiki/Schlick%27s_approximation
float3
Specular_F_Shlick(float3 f0, float3 f90, float x) {
    return f0 + (f90 - f0) * pow(1.0f - x, 5.0f);
};

/************************************************/
////////////////////Specular G////////////////////
/************************************************/

// Schlick-Smith specular G (visibility) with Hable's LdotH optimization
// http://www.cs.virginia.edu/~jdl/bib/appearance/analytic%20models/schlick94b.pdf
// http://graphicrants.blogspot.se/2013/08/specular-brdf-reference.html
float
Specular_G_Shlick_Smith_Hable(float alpha, float LdotH) {
    //return 1.0f / lerp(LdotH * LdotH, 1.0f, alpha * alpha * 0.25f);
    return rcp(lerp(LdotH * LdotH, 1.0f, alpha * alpha * 0.25f));
};





float
Specular_D(float alpha, float NdotH) {
    return Specular_D_GGX(alpha, NdotH);
};

float3
Specular_F(float3 SpecularColor, float LdotH) {
    return Specular_F_Shlick(SpecularColor, (1.0f).xxx, LdotH);
};

float
Specular_G(float alpha, float LdotH) {
    return Specular_G_Shlick_Smith_Hable(alpha, LdotH);
};





// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float
Diffuse_Burley(float NdotL,
               float NdotV,
               float LdotH,
               float roughness) {
  float fd90 = 0.5f + 2.0f * roughness * LdotH * LdotH;
  return Specular_F_Shlick((1.0f).xxx, fd90.xxx, NdotL).x * Specular_F_Shlick((1.0f).xxx, fd90.xxx, NdotV).x;
};