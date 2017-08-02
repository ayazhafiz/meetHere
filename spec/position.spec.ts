import { Position } from '../src/index';
import { expect } from 'chai';
import 'mocha';

describe('Position', () => {
  describe('instantiation', () => {
    it('works with number Arrays', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1]]);
      expect(test).to.have
        .property('locations')
        .that.is.an('Array')
        .that.deep.equals([[1, 2], [5, 6.6], [-7, 8.1]]);
      expect(test).to.have
        .property('options')
        .that.has.property('epsilon')
        .that.is.an('number');
    });
  });
  describe('center', () => {
    it('finds geometric center of points', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.center).to.deep.equal([
        0.9998479030807611,
        2.000560357070879
      ]);
    });
    it('finds geometric center of points with oblique search', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]], {
        subsearch: true
      });
      expect(test.center).to.deep.equal([
        0.9999063853985626,
        2.001119760004479
      ]);
    });
    it('finds median of points', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.median).to.deep.equal([0.525, 3.75]);
    });
    it('calculates score', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.score).to.equal(0.06535988277952172);
    });
    it('finds shortest path between paths', () => {
      const test = new Position([
        [5.4, 0.3],
        [0.8, 7.3],
        [1.3, 1.2],
        [7.6, 9],
        [4.6, 6.7],
        [3.8, 8.4],
        [8.9, 9],
        [0, 2.1],
        [8.9, 7.6],
        [6.3, 8.1],
        [9, 2.8],
        [8.1, 3.7],
        [1.2, 5.1],
        [2.3, 1.9],
        [2.6, 9.2]
      ]);
      expect(test.path).to.deep.equal([
        0,
        13,
        2,
        7,
        12,
        1,
        14,
        5,
        4,
        9,
        3,
        6,
        8,
        11,
        10
      ]);
    });
  });
  describe('manipulation', () => {
    it('adds points to locations', () => {
      const test = new Position([[1, 2]]);
      test.add([5, 3]);
      expect(test.locations).to.deep.equal([[1, 2], [5, 3]]);
    });
    it('removes points in locations', () => {
      const test = new Position([[1, 2], [5, 3]]);
      expect(test.remove([5, 3])).to.deep.equal([5, 3]);
      expect(test.locations).to.deep.equal([[1, 2]]);
    });
    it('returns invalid index for removal of non-existent element', () => {
      const test = new Position([[1, 2]]);
      expect(test.remove([5, 3])).to.equal(-1);
      expect(test.locations).to.deep.equal([[1, 2]]);
    });
    it('adjusts points in locations', () => {
      const test = new Position([[1, 2], [5, 3]]);
      expect(test.adjust([5, 3], [3, 5])).to.deep.equal([5, 3]);
      expect(test.locations).to.deep.equal([[1, 2], [3, 5]]);
    });
    it('returns invalid index for adjustment of non-existent element', () => {
      const test = new Position([[1, 2]]);
      expect(test.adjust([5, 3], [3, 5])).to.equal(-1);
      expect(test.locations).to.deep.equal([[1, 2]]);
    });
  });
});
