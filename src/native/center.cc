#include <cmath>
#include <iostream>
#include <node.h>
#include <vector>

const double s2 = std::sqrt(2) / 2;
const double delta_x[] = {-1, -s2, 0, s2, 1, s2, 0, -s2};
const double delta_y[] = {0, s2, 1, s2, 0, -s2, -1, -s2};

double cost(const v8::Local<v8::Array> &points, const double &x,
            const double &y) {
  double cost = 0;
  const unsigned int length = points->Length();

  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(points->Get(i));
    cost += std::sqrt(std::pow(element->Get(0)->NumberValue() - x, 2) +
                      std::pow(element->Get(1)->NumberValue() - y, 2));
  }
  return cost;
}

std::vector<double> centerOfMass(const v8::Local<v8::Array> &points) {
  double x = 0;
  double y = 0;
  const unsigned int length = points->Length();

  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(points->Get(i));
    x += element->Get(0)->NumberValue();
    y += element->Get(1)->NumberValue();
  }

  std::vector<double> com;
  com.push_back(x /= length);
  com.push_back(y /= length);
  return com;
}

void geometric(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();
  v8::Local<v8::Array> points = v8::Local<v8::Array>::Cast(args[0]);
  bool subsearch = args[1]->BooleanValue();
  double epsilon = args[2]->NumberValue();
  double bounds = args[3]->NumberValue();

  std::vector<double> center = centerOfMass(points);
  double score = cost(points, center[0], center[1]);
  double step = score / points->Length() * bounds;

  while (step > epsilon) {
    bool improved = false;
    for (int i = 0; i < 8; subsearch ? ++i : (i += 2)) {
      const double _x = center[0] + step * delta_x[i];
      const double _y = center[1] + step * delta_y[i];
      const double _score = cost(points, _x, _y);

      if (_score < score) {
        center[0] = _x;
        center[1] = _y;
        score = _score;
        break;
      }
    }

    if (!improved) {
      step /= 2;
    }
  }

  v8::Local<v8::Array> _center = v8::Array::New(isolate);
  _center->Set(0, v8::Number::New(isolate, center[0]));
  _center->Set(1, v8::Number::New(isolate, center[1]));

  v8::Local<v8::Object> result = v8::Object::New(isolate);
  result->Set(v8::String::NewFromUtf8(isolate, "center"), _center);
  result->Set(v8::String::NewFromUtf8(isolate, "score"),
              v8::Number::New(isolate, cost(points, center[0], center[1])));

  args.GetReturnValue().Set(result);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "geometric", geometric);
}

NODE_MODULE(addon, init);
