#include "polynomial.h"

extern "C"
{
#include "../polynomial.h"
}

void PolynomialWrapper::bestFit(
    const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  v8::Local<v8::Array> _points   = v8::Local<v8::Array>::Cast(args[0]);
  const uint64_t       numPoints = _points->Length();
  double               xPos[numPoints];
  double               yPos[numPoints];
  v8::Local<v8::Array> orig;

  for (uint64_t i = 0; i < numPoints; ++i) {
    orig    = v8::Local<v8::Array>::Cast(_points->Get(i));
    xPos[i] = orig->Get(0)->NumberValue();
    yPos[i] = orig->Get(1)->NumberValue();
  }

  uint64_t degree = args[1]->Uint32Value();
  if (!degree) {
    degree = Polynomial.guess_degree(xPos, yPos, numPoints);
  }

  // calculate polynomial
  double * coeffs = Polynomial.best_fit(xPos, yPos, numPoints, degree);

  // pass coeffs back to JS Array
  v8::Local<v8::Array> _coeffs = v8::Array::New(isolate);
  for (unsigned char i = 0; i < degree + 1; ++i) {
    _coeffs->Set(i, v8::Number::New(isolate, coeffs[i]));
  }

  args.GetReturnValue().Set(_coeffs);

  delete[] coeffs;
}
