import postgres from 'postgres';
import { env } from './env';

export const sql = postgres({
  ...env.db,
  transform: postgres.camel
});
