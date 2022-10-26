import * as functions from 'firebase-functions';
import * as admin from 'firebase-admin';
import { subSeconds } from 'date-fns';
import { error, log } from './helpers';
import { AirQualitySensorTopic, NotificationsService } from './notifications.service';

admin.initializeApp();
const db = admin.firestore();
const notificationsService = new NotificationsService();

const AQS_COLLECTION_PATH = 'aqs';

interface Value {
  u: number;
  c: number;
  pm1p0: number;
  pm2p5: number;
  pm10p0: number;
  t: number;
  h: number;
}

interface Payload {
  u: number;
  d: Value[];
}
export const aqs = functions.https.onRequest(async (request, response): Promise<void> => {
  try {
    const body: Payload = request.body;
    log({ method: request.method, body: body });
    switch (request.method) {
      case 'POST': {
        await saveData(body);
        const lastValue = body.d[body.d.length - 1];
        if (lastValue.c > 1000) {
          await notificationsService.sendToTopic(
            AirQualitySensorTopic,
            `CO2 level is too high. ${lastValue.c} ppm`,
            `Time to refresh air.`,
            { type: 'HIGH_CO2', co2: lastValue.c.toString() },
          );
        }
        if (lastValue.pm2p5 > 50) {
          await notificationsService.sendToTopic(
            AirQualitySensorTopic,
            `PM2.5 level is too high. ${lastValue.pm2p5} μg/m3`,
            `Beware of pollution!`,
            { type: 'HIGH_PM2P5', pm2p5: lastValue.pm2p5.toString() },
          );
        }
        if (lastValue.pm10p0 > 150) {
          await notificationsService.sendToTopic(
            AirQualitySensorTopic,
            `PM10.0 level is too high. ${lastValue.pm10p0} μg/m3`,
            `Beware of pollution!`,
            { type: 'HIGH_PM10P0', pm10p0: lastValue.pm10p0.toString() },
          );
        }
        response.json({ success: true });
        return;
      }
      default: {
        response.sendStatus(415);
        return;
      }
    }
  } catch (err: unknown) {
    error(err as Error);
    response.sendStatus(500);
  }
});

async function saveData(payload: Payload) {
  const batch = db.batch();
  const now = new Date();
  const ids: string[] = [];
  payload.d.forEach(item => {
    const data = {
      at: subSeconds(now, payload.u - item.u),
      ...item,
    };
    const doc = db.collection(AQS_COLLECTION_PATH).doc();
    ids.push(doc.id);
    batch.set(doc, data);
  });
  await batch.commit();
  log({ savedDocumentIds: ids });
}

export const aqsPushNotifications = functions.https.onRequest(async (request, response): Promise<void> => {
  try {
    log({ method: request.method, body: request.body });
    switch (request.method) {
      case 'POST': {
        await notificationsService.subscribeToTopics([request.body.token], [AirQualitySensorTopic]);
        response.json({ success: true });
        return;
      }
      case 'DELETE': {
        await notificationsService.unsubscribeFromTopics([request.body.token], [AirQualitySensorTopic]);
        response.json({ success: true });
        return;
      }
      default: {
        response.sendStatus(415);
        return;
      }
    }
  } catch (err: unknown) {
    error(err as Error);
    response.sendStatus(500);
  }
});
