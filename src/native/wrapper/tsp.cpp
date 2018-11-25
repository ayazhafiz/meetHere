#include "tsp.h"

extern "C"
{
#include "../tsp.h"
}

/**
 * @brief   Returns the degree of norm corresponding to a method of the
 *          Travelling Salesman Problem.
 *
 * @param   m      method
 *
 * @return  the degree of norm to use
 */
uint64_t visitMethod(const char m)
{
  switch (m) {
    case 't':  // TSP
      return 2;
    case 'n':  // Naive VRP
      return 1;

    default:
      return 2;
  }
}

/**
 * @brief   Determines the shortest-travel path between planar points,
 *          interfaced with Node.js.
 */
void TSPWrapper::solve(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points   = v8::Local<v8::Array>::Cast(args[0]);
  const uint64_t       numPoints = _points->Length();
  const uint64_t       startCity = args[1]->Uint32Value();
  const char           method    = (char)(args[2]->Uint32Value());

  // pass locations to C++ array
  double points[numPoints][2];
  for (uint64_t i = 0; i < numPoints; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                  = _element->Get(0)->NumberValue();
    points[i][1]                  = _element->Get(1)->NumberValue();
  }

  uint64_t * order = TSP.solve((const double **)points,
                               numPoints,
                               2,
                               startCity,
                               visitMethod(method));

  v8::Local<v8::Array> _order = v8::Array::New(isolate);
  for (uint64_t i = 0; i < numPoints; ++i) {
    _order->Set(i, v8::Number::New(isolate, order[i]));
  }

  delete[] order;

  args.GetReturnValue().Set(_order);
}
