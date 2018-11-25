# position 📍
> A blazing-fast JavaScript library for handling locations on and off maps.

[![Build Status](https://travis-ci.org/ayazhafiz/position.svg?branch=master)](https://travis-ci.org/ayazhafiz/position)
[![Coverage Status](https://coveralls.io/repos/github/ayazhafiz/position/badge.svg?branch=master)](https://coveralls.io/github/ayazhafiz/position?branch=master)
[![GitHub](https://img.shields.io/badge/View%20On-GitHub-blue.svg)](https://github.com/ayazhafiz/meetHere)

`position` is a library for quick, elegant manipulation of positional data on a 2D
plane. Prominent features include:
- [x] High-precision geometric calculations
- [x] Rapid path determinations
- [x] Powerful async operations
- [x] First-class TypeScript support
- [x] C++ bindings
- [ ] Time-minimum center (coming soon)

> `position` was previously branded as `meetHere`, which had first-class support
> for the Google Maps API. Due to decreased usage quotas of the Google Maps API,
> this functionality has been removed.

## Installation
```bash
$ yarn add position.ts
```

```bash
$ npm i position.ts --save
```

Or [build it yourself](#develop)!

<sub>[Supported Node versions](#support)</sub>

## Usage
You can find the full API [here](https://ayazhafiz.github.io/position/).

#### `Position` - a blazing-fast, 2D-locations manipulation prototype.
> For general-purpose use, like in physics libraries or games.

```javascript
import { Position } from 'position.ts';

let Plane = new Position([[0, 1], [1.5, 3], [-9, 1.07]]);
Plane.add([3, 4]);
Plane.center // => [ 1.4990254494359172, 2.999397294475018 ]
Plane.median // => [ -1.125, 2.2675 ]
Plane.score // => 0.11155562920022742
```

### Extensions

Extending any class in `position` for your specific use case is trivial:

```javascript
import { Position } from 'position.ts';

class Plane3D extends Position {
  constructor(args) {
    super(...args);
  }

  get flightPath() {
    return this.dimensionalize(
      super.vrp(this.locations.map(v => [v[0], v[1]])),
      super.vrp(this.locations.map(v => [v[0], v[2]])),
      super.vrp(this.locations.map(v => [v[1], v[2]]))
    );
  }
}
```

## Develop
```bash
git clone git@github.com:ayazhafiz/position.git && cd position
yarn # or, npm install

# develop some features

# compile
node-gyp configure
node-gyp build
yarn build # or, npm run build

yarn test # or, npm test
```

## Support
`position` actively supports Node versions 4 and higher. More specifically:
- 4.x.x (LTS/argon)
- 5.x.x
- 6.x.x (LTS/boron)
- 7.x.x
- 8.x.x
- 9.x.x (nightly)
  * Note that failures in nightly versions are most likely a problem with Node,
  not with this package.

`position` may work with Node 0.x.x or io.js (1 - 3.x.x), but since these strains
are not supported by Node anymore, this library does not target build
compatibility with them. The sensitivity of this library's native bindings and
ES targeting may make it completely inoperable with anything below LTS/argon.

## License
MIT, &copy; hafiz 2017
