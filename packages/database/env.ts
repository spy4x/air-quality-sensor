import { config } from 'dotenv';

const isProd = process.env.NODE_ENV === 'prod' || process.env.NODE_ENV === 'production';

const isEnvLoadedAlready =
  process.env.DB_HOST && process.env.DB_USER && process.env.DB_PASS && process.env.DB_NAME;

if (!isEnvLoadedAlready) {
  const basePath = `../..`;
  config({ path: isProd ? `${basePath}/.env.prod` : `${basePath}/.env` });
}

export const env = {
  isProd,
  db: {
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    pass: process.env.DB_PASS,
    db: process.env.DB_NAME
  }
};
