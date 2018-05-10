#ifndef WRAPPER_CARTESIAN_H
#define WRAPPER_CARTESIAN_H

#include <node.h>

namespace CartesianWrapper
{
/**
 * @brief   Calculates the cartesian (earthly) distance between two lat/lng
 *          points, interfaced with Node.js.
 */
void distance(const v8::FunctionCallbackInfo<v8::Value> & args);

}  // namespace CartesianWrapper

#endif
