#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "modulacion.h"
#define E 2.718281
#define PI 3.141592 
//******FUNCIONES DE MODULACION:********//
float mConstant(double t, float params[3]) {
    return 1;
}
float mLinear(double t, float params[3]) {
    return t/params[0];
}
float mInvlinear(double t, float params[3]) {
    if (t>params[0]) return 0;
    return 1-t/params[0];
}
float mSin(double t, float params[3]) {
    return 1+params[0]*sin(params[1]*t);
}
float mExp(double t, float params[3]) {
    return pow(E, ( 5*(t-params[0])/params[0]));
}
float mInvexp(double t, float params[3]) {
    return pow(E, (-5*t/params[0]));
}
float mQuartCos(double t, float params[3]) {
    return cos(PI*t/(2*params[0]));
}
float mQuartSin(double t, float params[3]) {
    return sin(PI*t/(2*params[0]));
}
float mHalfCos(double t, float params[3]) {
    return (1+cos(PI*t/params[0]))/2;
}
float mHalfSin(double t, float params[3]) {
    return (1+sin(PI*((t/params[0])-0.5)) )/2;
}
float mLog(double t, float params[3]) {
    return log10((9*t/params[0])+1);
}
float mInvlog(double t, float params[3]) {
    if(t>=params[0]) return 0;
    return log10((-9*t/params[0])+10);
}
/////**EN CONSTRUCCION**/////
float mTri(double t, float params[3]) {
    return 1;
}
float mPulses(double t, float params[3]) {
    return 1;
}
