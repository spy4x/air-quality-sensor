import { type RequestHandler, json } from '@sveltejs/kit';
import { SensorDataSchema } from '@repo/shared-types';
import { db } from '@repo/db';

export const GET: RequestHandler = async ({ url }) => {
	const hours = Number(url.searchParams.get('hours')) || 1;
	const measurements = await db.measurements.findMany(hours);
	return json(measurements);
};

export const POST: RequestHandler = async ({ request }) => {
	const jsonPayload = await request.json();
	const parseResult = SensorDataSchema.safeParse(jsonPayload);
	if (!parseResult.success) {
		return jsonPayload(parseResult.error.formErrors, 400);
	}
	const payload = parseResult.data;

	const originalUptimeMoment = payload.u * 1000;
	const now = Date.now();

	await db.measurements.createMany(
		payload.d.map((v) => ({
			co2: v.c,
			pm2p5: v.pm2p5,
			temperature: v.t,
			humidity: v.h,
			uptime: v.u,
			createdAt: new Date(now - (originalUptimeMoment - v.u * 1000))
		}))
	);

	return json({ success: true });
};
