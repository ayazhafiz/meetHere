#ifndef WRAPPER_POLYNOMIAL_H
#define WRAPPER_POLYNOMIAL_H

#include <node.h>

namespace PolynomialWrapper
{
/**
 * @brief   Calculates the best-fit polynomial function of an arbitrary set of
 *          points, interfaced with Node.js.
 */
void bestFit(const v8::FunctionCallbackInfo<v8::Value> & args);

}  // namespace PolynomialWrapper

#endif
