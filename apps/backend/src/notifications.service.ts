import * as admin from 'firebase-admin';
import { messaging } from 'firebase-admin';
import { log } from './helpers';

class Topic {
  constructor(public name: string) {}
}
export const AirQualitySensorTopic = new Topic(`aqs`);

export class NotificationsService {
  private readonly messaging: messaging.Messaging;

  constructor() {
    if (!admin.apps.length) {
      admin.initializeApp();
    }
    this.messaging = messaging();
  }

  async sendToTopic(
    topic: Topic,
    title: string,
    body: string,
    data: Record<string, string> & { type: string },
    mutableContent = false,
  ): Promise<void> {
    const message: messaging.MessagingPayload = {
      notification: {
        title,
        body,
      },
      data: data,
    };

    log({
      message: `NotificationsService.sendToTopic() - Start`,
      topic,
      payload: message,
    });

    const result = await this.messaging.sendToTopic(topic.name, message, {
      mutableContent,
    });

    log({
      message: `NotificationsService.sendToTopic() - End`,
      result,
    });
  }

  async subscribeToTopics(tokens: string[], topics: Topic[]): Promise<void> {
    const promises = topics.map(topic => this.messaging.subscribeToTopic(tokens, topic.name));
    await Promise.all(promises);
  }

  async unsubscribeFromTopics(tokens: string[], topics: Topic[]): Promise<void> {
    const promises = topics.map(topic => this.messaging.unsubscribeFromTopic(tokens, topic.name));
    await Promise.all(promises);
  }
}
