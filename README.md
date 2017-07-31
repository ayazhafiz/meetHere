# meetHere <img src="https://cdn.rawgit.com/ayazhafiz/meetHere/master/meetHere.svg" height=220 align="right"/>
> A hyper-efficient library for position manipulation.

[![Build Status](https://travis-ci.org/ayazhafiz/meetHere.svg?branch=master)](https://travis-ci.org/ayazhafiz/meetHere)
[![Coverage Status](https://coveralls.io/repos/github/ayazhafiz/meetHere/badge.svg?branch=master)](https://coveralls.io/github/ayazhafiz/meetHere?branch=master)
[![GitHub](https://img.shields.io/badge/View%20On-GitHub-blue.svg)](https://github.com/ayazhafiz/meetHere)

meetHere is a library for quick, elegant manipulation of positional data on a 2D
plane, and comes with first-class Google Maps support for on-the-fly
determination of locations relevant to you and your users. Prominent features
include:
- [x] High-precision geometric center calculations
- [x] Positional center scoring
- [x] Determining relevant nearby locations
- [x] Powerful async operations
- [x] First-class TypeScript support
- [x] C++ bindings
- [ ] Time-minimum center (coming soon)

## Installation
```bash
$ yarn add meethere
```

## Build
```bash
$ npm i meethere --save
```

Or [build it yourself](#develop)!

## Usage
You can find the full API [here](http://meethere.js.org).

Currently, two classes are available:

#### `Position` - a blazing-fast, 2D-locations manipulation prototype.
> For general-purpose use, like in physics libraries or games.

```javascript
import { Position } from 'meethere';

let Plane = new Position([[0, 1], [1.5, 3], [-9, 1.07]]);
Plane.add([3, 4]);
Plane.center // => [ 1.4990254494359172, 2.999397294475018 ]
Plane.median // => [ -1.125, 2.2675 ]
Plane.score // => 0.11155562920022742
```

#### `MeetHere` - a subclass of `Position` with first-class Google Maps support.
> For user-interactive applications, like meetHere itself.

To use non-super methods of `MeetHere`, you'll need a Google Maps API key, as
described
[here](https://github.com/googlemaps/google-maps-services-js#api-keys).

```javascript
import { MeetHere } from 'meethere';

const MY_GOOGLE_MAPS_TOKEN = process.env.TOKEN;
const user = [33.0952311, -96.8640427];
const west = [33.0437115, -96.8157956];
const senior = [33.0284505, -96.7546927];

let Map = new MeetHere(
  [user, west, senior],
  MY_GOOGLE_MAPS_TOKEN
);
Map.meetHere // => [ 33.04371181611578, -96.81579457575877 ]
Map.nearby().then(console.log) // => { results: [...] }
```

## Develop
```bash
git clone git@github.com:ayazhafiz/meetHere.git && cd meetHere
yarn # or, npm install

# develop some features & test your code
yarn test # or, npm test

# compile
node-gyp configure
node-gyp build
yarn build # or, npm run build
```

## License
MIT, &copy; hafiz 2017
