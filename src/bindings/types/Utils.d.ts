import {Image} from "./Image";

export function readImage(filePath: string): Image;
export function readImage(filePath: string, asGray: boolean): Image;

export function showImage(image: Image);
export function showImage(image: Image, windowName: string);
export function showImage(image: Image, windowName: string, asGray: boolean);

export function saveImage(saveAs: string, image: Image);

export function destroyWindow(windowName: string): undefined;

export function destroyAllWindows(): undefined;

export function waitKey(milliseconds: number): number;

export function saveImage(saveAs: string, image: Image);

export function classify(image: Image);