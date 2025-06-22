import type { Measurement, MeasurementBase } from '@repo/shared-types';
import { sql } from './lib';

// Uncomment for debugging queries
sql.options.debug = (_, query, parameters) =>
  console.log(query, parameters.length ? parameters : '');
// TODO: automatically log if query takes too long

export const db = {
  $connect: async (): Promise<void> => {
    await sql`SELECT 1`;
  },
  $shutdown: async (): Promise<void> => sql.end({ timeout: 5 }),
  measurements: {
    createMany: async (measurements: MeasurementBase[]): Promise<void> => {
      await sql`INSERT INTO measurements ${sql(measurements)}`;
    },
    findMany: async (hours: number): Promise<Measurement[]> => {
      const createdAtGt = new Date(Date.now() - hours * 60 * 60 * 1000);
      return await sql`SELECT * FROM measurements WHERE created_at > ${createdAtGt} ORDER BY created_at ASC`;
    },
    findLatest: async (): Promise<Measurement | null> => {
      const result = await sql`SELECT * FROM measurements ORDER BY created_at DESC LIMIT 1`;
      return result[0] || null;
    }
  }
};
