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
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );
      expect(test).to.have
        .property('locations')
        .that.is.an('Array')
        .that.deep.equals([
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ]);
      expect(test).to.have.property('client').that.is.an('Object');
      expect(test).to.have.property('options').that.has.property('epsilon');
    });
  });
  describe('functionality', () => {
    it('calculates place to meet', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );
      test.meetHere.should.deep.equal([33.04373236065685, -96.81583367822624]);
    });
    it('gives nearby roads', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );
      return test
        .roads()
        .should.eventually.be.an('Object')
        .with.property('snappedPoints');
    });
    it('gives nearby roads with median', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );
      return test
        .roads(false)
        .should.eventually.be.an('Object')
        .with.property('snappedPoints');
    });
    it('gives nearby places', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );
      return test
        .nearby()
        .should.eventually.be.an('Object')
        .with.property('results')
        .that.is.an('Array');
    });
  });
  describe('errors', () => {
    it('returns error for invalid token', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        'invalidToken'
      );
      expect(() => test.roads()).to.throw(
        'Missing either a valid API key, or a client ID and secret'
      );
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
    it('returns error for invalid options', () => {
      const test = new MeetHere(
        [
          [33.0952311, -96.8640427],
          [33.0437115, -96.8157956],
          [33.0284505, -96.7546927]
        ],
        process.env.GOOGLE_MAPS_TOKEN
      );

      return test
        .nearby({ type: 'notatype' })
        .should.eventually.be.an('Object')
        .with.property('status')
        .that.equals('INVALID_REQUEST');
    });
  });
});
