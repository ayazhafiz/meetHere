import { Suite } from 'benchmark';
import * as Bindings from 'bindings';
import { Center } from '../src/namespaces/center';

const Cpp = Bindings('center');

const suite = new Suite();

const test = [[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]];

suite
  .add('Geometric JS', () => {
    Center.geometric(test, false, 0.001, 10);
  })
  .add('Geometric C++', () => {
    Cpp.geometric(test, false, 0.001, 10);
  })
  .on('cycle', event => {
    console.log(String(event.target));
  })
  .run({ async: true });
