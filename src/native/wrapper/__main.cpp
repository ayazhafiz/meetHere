#include "cartesian.h"
#include "point_set.h"
#include "polynomial.h"
#include "tsp.h"

#include <node.h>

void init(v8::Local<v8::Object> exports)
{
  NODE_SET_METHOD(exports, "distance", CartesianWrapper::distance);
  NODE_SET_METHOD(exports, "mean", PointSetWrapper::mean);
  NODE_SET_METHOD(exports, "geometric", PointSetWrapper::geometric);
  NODE_SET_METHOD(exports, "bestFit", PolynomialWrapper::bestFit);
  NODE_SET_METHOD(exports, "tsp", TSPWrapper::solve);
}

NODE_MODULE(addon, init);
