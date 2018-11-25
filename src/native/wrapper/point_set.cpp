#include "point_set.h"

extern "C"
{
#include "../point_set.h"
#include "../toolkit/ips.h"
}

/**
 * @brief   Calculates the mean of an arbitrary amount of points, interfaced
 *          with Node.js.
 */
void PointSetWrapper::mean(const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);

  const unsigned int length = _points->Length();

  // pass locations to C++ array
  double points[length][2];
  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                 = element->Get(0)->NumberValue();
    points[i][1]                 = element->Get(1)->NumberValue();
  }

  // get results
  Grid_2D      center        = PointSet.mean(points, length);
  double       center_arr[2] = {center.x, center.y};
  const double score         = IPS.net_distance(2,
                                        (const double *)center_arr,
                                        2,
                                        (const double **)points,
                                        length);

  // convert center back to JS Array
  v8::Local<v8::Array> _center = v8::Array::New(isolate);
  _center->Set(0, v8::Number::New(isolate, center.x));
  _center->Set(1, v8::Number::New(isolate, center.y));

  // create object to hold center and score
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "center"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "score"),
              v8::Number::New(isolate, score));

  args.GetReturnValue().Set(result);
}

/**
 * @brief   Calculates the geometric median of an arbitrary amount of points,
 *          interfaced with Node.js.
 */
void PointSetWrapper::geometric(
    const v8::FunctionCallbackInfo<v8::Value> & args)
{
  v8::Isolate * isolate = args.GetIsolate();

  // get args
  v8::Local<v8::Array> _points   = v8::Local<v8::Array>::Cast(args[0]);
  const uint64_t       numPoints = _points->Length();
  const bool           subsearch = args[1]->BooleanValue();
  const double         epsilon   = args[2]->NumberValue();
  const double         bounds    = args[3]->NumberValue();
  const struct GeometricCenterOptions opts = {epsilon, bounds, subsearch};

  // pass locations to native array
  double points[numPoints][2];
  for (unsigned int i = 0; i < numPoints; ++i) {
    v8::Local<v8::Array> _element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0]                  = _element->Get(0)->NumberValue();
    points[i][1]                  = _element->Get(1)->NumberValue();
  }

  // calculate geometric center
  Grid_2D      center = PointSet.geometric_median(points, numPoints, &opts);
  double       center_arr[2] = {center.x, center.y};
  const double score         = IPS.net_distance(2,
                                        (const double *)center_arr,
                                        2,
                                        (const double **)points,
                                        numPoints);

  // convert center back to JS Array
  v8::Local<v8::Array> _center = v8::Array::New(isolate);
  _center->Set(0, v8::Number::New(isolate, center.x));
  _center->Set(1, v8::Number::New(isolate, center.y));

  // create object to hold center and score
  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "center"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "score"),
              v8::Number::New(isolate, score));

  args.GetReturnValue().Set(result);
}
