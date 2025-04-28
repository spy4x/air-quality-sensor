import { z } from 'zod';

export const SensorDataSchema = z.object({
  u: z.number(),
  d: z.array(
    z.object({
      u: z.number(),
      c: z.number(),
      pm2p5: z.number(),
      t: z.number(),
      h: z.number()
    })
  )
});
export type SensorData = z.infer<typeof SensorDataSchema>;

export const IdSchema = z.object({
  id: z.number().int().positive()
});
export const CreatedAtSchema = z.object({
  createdAt: z.date()
});
export const BaseSchema = IdSchema.merge(CreatedAtSchema);

export const MeasurementBaseSchema = CreatedAtSchema.merge(
  z.object({
    co2: z.number().int().positive(),
    pm2p5: z.number().int().positive(),
    temperature: z.number().min(-273.15).max(100),
    humidity: z.number().min(0).max(100),
    uptime: z.number().int().positive()
  })
);

export type MeasurementBase = z.infer<typeof MeasurementBaseSchema>;
export const MeasurementSchema = MeasurementBaseSchema.merge(BaseSchema);
export type Measurement = z.infer<typeof MeasurementSchema>;
