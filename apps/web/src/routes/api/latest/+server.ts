import { type RequestHandler, json } from '@sveltejs/kit';
import { db } from '@repo/db';

export const GET: RequestHandler = async () => {
	const latestMeasurement = await db.measurements.findLatest();
	
	if (!latestMeasurement) {
		return json({ error: 'No measurements found' }, { status: 404 });
	}
	
	return json(latestMeasurement);
};
