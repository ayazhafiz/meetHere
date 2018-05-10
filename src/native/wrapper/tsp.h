#ifndef WRAPPER_TSP_H
#define WRAPPER_TSP_H

#include <node.h>

namespace TSPWrapper
{
/**
 * @brief   Determines the shortest-travel path between planar points,
 *          interfaced with Node.js.
 */
void solve(const v8::FunctionCallbackInfo<v8::Value> & args);

}  // namespace TSPWrapper

#endif
