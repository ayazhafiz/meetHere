/**
 * Array prototype extension
 *
 * @interface
 */
declare global {
  interface Array<T> {
    deepIndexOf(value: T): number;
  }
}

export function arrayUtil(): void {
  /**
   * Finds the index of a nested object in an array
   *
   * @extends Array
   */
  Array.prototype.deepIndexOf = function(value: Array<number>): number {
    for (let i = 0; i < this.length; i++) {
      if (
        this[i].length === value.length &&
        this[i].every((v, j) => v === value[j])
      ) {
        return i;
      }
    }
    return -1;
  };
}
