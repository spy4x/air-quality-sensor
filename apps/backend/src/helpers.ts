import * as functions from 'firebase-functions';
import { inspect as utilInspect } from 'util';

const IS_PROD = process.env.NODE_ENV === 'production';
const MAX_LINE_WIDTH = 120;

export function log(data: Object) {
  const inspectedData = inspect(data);
  if (IS_PROD) {
    functions.logger.info(inspectedData, { structuredData: true });
  } else {
    console.log(inspectedData);
  }
}

export function error(error: Error) {
  if (IS_PROD) {
    functions.logger.error(error, { structuredData: true });
  } else {
    console.error(error);
  }
}

export function inspect(object: unknown): string {
  return utilInspect(object, {
    depth: 15, // deep nesting, but avoid infinity for security reasons
    colors: !IS_PROD, // Google Cloud Logger shows colors as special symbols, like "[32m", instead of coloring text
    breakLength: IS_PROD ? Infinity : MAX_LINE_WIDTH,
  });
}
