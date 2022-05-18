import { Rect } from './Rect';

export class FloatImage {
    constructor(data: Float32Array, rows: number, columns: number, type: number);

    width(): number;

    height(): number;

    channels(): number;

    type(): number;

    extract(roi: Rect): FloatImage;

    toFloat32Array(): Float32Array;
}