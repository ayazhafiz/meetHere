#include <cmath>
#include <node.h>
#include <vector>

const double s2 = std::sqrt(2) / 2;
const double delta_x[] = {-1, -s2, 0, s2, 1, s2, 0, -s2};
const double delta_y[] = {0, s2, 1, s2, 0, -s2, -1, -s2};

double cost(const double points[][2], const unsigned int &length,
            const double &x, const double &y) {
  double cost = 0;

  for (unsigned int i = 0; i < length; ++i) {
    cost += std::sqrt(std::pow(points[i][0] - x, 2) +
                      std::pow(points[i][1] - y, 2));
  }
  return cost;
}

std::vector<double> centerOfMass(const double points[][2],
                                 const unsigned int &length) {
  double x = 0;
  double y = 0;

  for (unsigned int i = 0; i < length; ++i) {
    x += points[i][0];
    y += points[i][1];
  }

  std::vector<double> com;
  com.push_back(x /= length);
  com.push_back(y /= length);
  return com;
}

void geometric(const v8::FunctionCallbackInfo<v8::Value> &args) {
  v8::Isolate *isolate = args.GetIsolate();

  v8::Local<v8::Array> _points = v8::Local<v8::Array>::Cast(args[0]);
  const unsigned int length = _points->Length();

  double points[length][2];

  for (unsigned int i = 0; i < length; ++i) {
    v8::Local<v8::Array> element = v8::Local<v8::Array>::Cast(_points->Get(i));
    points[i][0] = element->Get(0)->NumberValue();
    points[i][1] = element->Get(1)->NumberValue();
  }

  bool subsearch = args[1]->BooleanValue();
  double epsilon = args[2]->NumberValue();
  double bounds = args[3]->NumberValue();

  std::vector<double> center = centerOfMass(points, length);
  double score = cost(points, length, center[0], center[1]);
  double step = score / length * bounds;

  while (step > epsilon) {
    bool improved = false;
    for (int i = 0; i < 8; subsearch ? ++i : (i += 2)) {
      const double _x = center[0] + step * delta_x[i];
      const double _y = center[1] + step * delta_y[i];
      const double _score = cost(points, length, _x, _y);

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
              v8::Number::New(isolate, score));

  args.GetReturnValue().Set(result);
}

void init(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "geometric", geometric);
}

NODE_MODULE(addon, init);
