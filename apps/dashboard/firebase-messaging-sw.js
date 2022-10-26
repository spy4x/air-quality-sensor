// Import and configure the Firebase SDK
// These scripts are made available when the app is served or deployed on Firebase Hosting
// If you do not serve/host your project using Firebase Hosting see https://firebase.google.com/docs/web/setup
importScripts('/__/firebase/9.9.1/firebase-app-compat.js');
importScripts('/__/firebase/9.9.1/firebase-messaging-compat.js');
firebase.initializeApp({
  apiKey: 'AIzaSyDVlnsfBwKSGiQUWKCIcfAeuCqUKhNiaQo',
  authDomain: 'antonshubin-production.firebaseapp.com',
  databaseURL: 'https://antonshubin-production.firebaseio.com',
  projectId: 'antonshubin-production',
  storageBucket: 'antonshubin-production.appspot.com',
  messagingSenderId: '1075534054040',
  appId: '1:1075534054040:web:0354532ec04948be7c86ed',
  measurementId: 'G-W2MMXJMEZC',
});

const messaging = firebase.messaging();

// If you would like to customize notifications that are received in the
// background (Web app is closed or not in browser focus) then you should
// implement this optional method.
// Keep in mind that FCM will still show notification messages automatically
// and you should use data messages for custom notifications.
// For more info see:
// https://firebase.google.com/docs/cloud-messaging/concept-options
// messaging.onBackgroundMessage(function (payload) {
//   console.log('[firebase-messaging-sw.js] Received background message ', payload);
//   // Customize notification here
//   const notificationTitle = 'Background Message Title';
//   const notificationOptions = {
//     body: 'Background Message body.',
//     icon: '/firebase-logo.png',
//   };
//
//   self.registration.showNotification(notificationTitle, notificationOptions);
// });
