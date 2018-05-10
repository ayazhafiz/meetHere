#ifndef WRAPPER_POINT_SET_H
#define WRAPPER_POINT_SET_H

#include <node.h>

namespace PointSetWrapper
{
/**
 * @brief   Calculates the mean of an arbitrary amount of points, interfaced
 *          with Node.js.
 */
void mean(const v8::FunctionCallbackInfo<v8::Value> & args);

/**
 * @brief   Calculates the geometric median of an arbitrary amount of points,
 *          interfaced with Node.js.
 */
void geometric(const v8::FunctionCallbackInfo<v8::Value> & args);

}  // namespace PointSetWrapper

#endif
