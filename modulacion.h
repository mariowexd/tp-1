#ifndef _modulacion_h_
#define _modulacion_h_

/*typedef  {
    CONSTANT,
    LINEAR,
    INVLINEAR,
    SIN,
    EXP,
    INVEXP,
    QUARTCOS,
    QUARTSIN,
    HALFCOS,
    HALFSIN,
    LOG,
    INVLOG,
    TRI,
    PULSES
}funmod_t;*/

float mConstant(double t, float params[3]);
float mLinear(double t, float params[3]);
float mInvlinear(double t, float params[3]);
float mSin(double t, float params[3]);
float mExp(double t, float params[3]);
float mInvexp(double t, float params[3]);
float mQuartCos(double t, float params[3]);
float mQuartSin(double t, float params[3]);
float mHalfCos(double t, float params[3]);
float mHalfSin(double t, float params[3]);
float mLog(double t, float params[3]);
float mInvlog(double t, float params[3]);
float mTri(double t, float params[3]);
float mPulses(double t, float params[3]);
#endif 
