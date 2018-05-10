import { MeetHere } from '../src/index';
import { expect, should, use } from 'chai';
import * as promise from 'chai-as-promised';
import 'mocha';

should();
use(promise);

describe('MeetHere', () => {
  describe('instantiation', () => {
    it('works with number Arrays', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
        { epsilon: 1e-6 },
      );
      expect(test)
        .to.have.property('locations')
        .that.is.an('Array')
        .that.deep.equals([
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ]);
      expect(test)
        .to.have.property('client')
        .that.is.an('Object');
      expect(test)
        .to.have.property('options')
        .that.has.property('epsilon');
    });
  });
  describe('functionality', () => {
    it('calculates place to meet', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      test.meetHere.should.deep.equal([33.04373236065685, -96.81583367822624]);
    });
    describe('gives distances to center', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      it('works', () => {
        return test
          .distanceMatrix('km', true)
          .should.be.an('Object')
          .with.property('distances')
          .that.is.an('Array');
      });
      it('works with defaults', () => {
        return test
          .distanceMatrix()
          .should.be.an('Object')
          .with.property('distances')
          .that.is.an('Array');
      });
    });
    describe('gives nearby places', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      it('works', () => {
        return test
          .nearby({}, true)
          .should.eventually.be.an('Object')
          .with.property('results')
          .that.is.an('Array');
      });
      it('works with defaults', () => {
        return test
          .nearby()
          .should.eventually.be.an('Object')
          .with.property('results')
          .that.is.an('Array');
      });
    });
    it('gives nearby roads', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      return test
        .roads()
        .should.eventually.be.an('Object')
        .with.property('snappedPoints');
    });
    it('gives nearby roads with mean', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      return test
        .roads(false)
        .should.eventually.be.an('Object')
        .with.property('snappedPoints');
    });
    describe('gives timezone at center', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      it('works', () => {
        return test
          .timezone({}, true)
          .should.eventually.be.an('Object')
          .with.property('timeZoneId')
          .that.equals('America/Chicago');
      });
      it('works with defaults', () => {
        return test
          .timezone()
          .should.eventually.be.an('Object')
          .with.property('timeZoneId')
          .that.equals('America/Chicago');
      });
    });
    describe('gives distances from points to center', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );
      it('works', () => {
        return test
          .travel({}, true)
          .should.eventually.be.an('Object')
          .with.property('rows')
          .that.is.an('Array');
      });
      it('works with defaults', () => {
        return test
          .travel()
          .should.eventually.be.an('Object')
          .with.property('rows')
          .that.is.an('Array');
      });
    });
  });
  describe('errors', () => {
    it('returns error for invalid options on distance matrix search', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );

      return test
        .travel({ departure_time: -100 })
        .should.eventually.be.an('Object')
        .with.property('status')
        .that.equals('INVALID_REQUEST');
    });
    it('returns error for invalid options on nearby search', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927],
        ],
        process.env.GOOGLE_MAPS_TOKEN,
      );

      return test
        .nearby({ type: 'notatype', radius: -100 })
        .should.eventually.be.an('Object')
        .with.property('status')
        .that.equals('INVALID_REQUEST');
    });
    it('returns error for no nearby roads', () => {
      const test = new MeetHere([], process.env.GOOGLE_MAPS_TOKEN);
      return test
        .roads()
        .should.eventually.be.an('Object')
        .with.property('error')
        .and.property('status')
        .that.equals('INVALID_ARGUMENT');
    });
    it('returns error for invalid timestamp on timezone request', () => {
      const test = new MeetHere([], process.env.GOOGLE_MAPS_TOKEN);
      return test
        .timezone({ timestamp: 0 })
        .should.eventually.be.an('Object')
        .and.property('status')
        .that.equals('INVALID_REQUEST');
    });
  });
});
