import type { Handle } from '@sveltejs/kit';

export const handle: Handle = async ({ event, resolve }) => {
	const start = Date.now();
	console.log(event.request.method, event.request.url);
	const result = await resolve(event);
	console.log(event.request.method, event.request.url, result.status, Date.now() - start + 'ms');
	return result;
};
