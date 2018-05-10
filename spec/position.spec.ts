import { Position } from '../src/index';
import { expect } from 'chai';
import 'mocha';

describe('Position', () => {
  describe('instantiation', () => {
    it('works with number Arrays', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1]]);
      expect(test)
        .to.have.property('locations')
        .that.is.an('Array')
        .that.deep.equals([[1, 2], [5, 6.6], [-7, 8.1]]);
      expect(test)
        .to.have.property('options')
        .that.has.property('epsilon')
        .that.is.an('number');
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
      expect(test.move([5, 3], [3, 5])).to.deep.equal([5, 3]);
      expect(test.locations).to.deep.equal([[1, 2], [3, 5]]);
    });
    it('returns invalid index for adjustment of non-existent element', () => {
      const test = new Position([[1, 2]]);
      expect(test.move([5, 3], [3, 5])).to.equal(-1);
      expect(test.locations).to.deep.equal([[1, 2]]);
    });
  });
  describe('center', () => {
    it('finds geometric center of points', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.center).to.deep.equal([
        0.9998479030807611,
        2.000560357070879,
      ]);
    });
    it('finds geometric center of points with oblique search', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]], {
        subsearch: true,
      });
      expect(test.center).to.deep.equal([
        0.9999063853985626,
        2.001119760004479,
      ]);
    });
    it('finds mean of points', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.mean).to.deep.equal([0.525, 3.75]);
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
      ]);
      expect(test.bestPath).to.deep.equal([0, 2, 7, 1, 4, 5, 9, 3, 6, 8, 10]);
    });
    it('finds naive~shortest drive paths', () => {
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
      ]);
      expect(test.quickPath).to.deep.equal([0, 2, 7, 1, 4, 5, 9, 3, 6, 8, 10]);
    });
    it('calculates polynomial', () => {
      const test = new Position([[0, 1], [1, 2], [3, 10]]);
      expect(
        test.polynomial.map((v) => Math.round(v * 1e6) / 1e6),
      ).to.deep.equal([1, -0, 1]);
    });
  });
  describe('calculates cost', () => {
    it('calculates cost for mean', () => {
      const test = new Position([[0, 0], [0, 1], [1, 0]]);
      expect(test.meanCost).to.equal(1.9621165057908914);
    });
    it('calculates cost for center', () => {
      const test = new Position([[0, 0], [0, 1], [1, 0]]);
      expect(test.centerCost).to.equal(1.9318524378402253);
    });
    it('calculates the difference in center values', () => {
      const test = new Position([[1, 2], [5, 6.6], [-7, 8.1], [3.1, -1.7]]);
      expect(test.geometricSignificance).to.equal(0.06535988277952172);
    });
  });
});
