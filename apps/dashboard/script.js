const chartReferenceBoxTransparency = 0.2;
const colorCodes = {
  GOOD: [33, 166, 5],
  OK: [239, 200, 2],
  BAD: [255, 153, 0],
  VERY_BAD: [255, 68, 0],
  DANGER: [253, 0, 0],
};
const models = [
  {
    title: 'Temperature',
    field: 't',
    color: 'red',
    unit: '°C',
    yMin: 20,
    yMax: 30,
    references: [
      {
        title: 'Too hot',
        color: colorCodes.DANGER,
        max: 50,
        min: 32,
        placement: 'end',
      },
      {
        title: 'Hot',
        color: colorCodes.OK,
        max: 32,
        min: 27,
      },
      {
        title: 'Optimum',
        color: colorCodes.GOOD,
        max: 27,
        min: 22,
      },
      {
        title: 'Chill',
        color: `rgba(0,255,233,${chartReferenceBoxTransparency})`,
        max: 22,
        min: 15,
        placement: 'start',
      },
      {
        title: 'Cold',
        color: `rgba(0,33,255,${chartReferenceBoxTransparency})`,
        max: 15,
        min: -50,
        placement: 'start',
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: 'Humidity',
    field: 'h',
    color: 'blue',
    unit: '%',
    yMin: 0,
    yMax: 100,
    references: [
      {
        title: 'Too humid',
        color: colorCodes.BAD,
        max: 100,
        min: 70,
      },
      {
        title: 'Moderate',
        color: colorCodes.OK,
        max: 70,
        min: 60,
      },
      {
        title: 'Optimum',
        color: colorCodes.GOOD,
        max: 60,
        min: 40,
      },
      {
        title: 'Moderate',
        color: colorCodes.OK,
        max: 40,
        min: 25,
      },
      {
        title: 'Too dry',
        color: colorCodes.BAD,
        max: 25,
        min: 0,
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: `Freshness, CO2`,
    field: 'c',
    fieldTitle: 'CO2',
    color: 'rgb(75, 192, 192)',
    yMin: 400,
    yMax: 2000,
    unit: 'ppm',
    references: [
      {
        title: 'Headache',
        color: colorCodes.VERY_BAD,
        max: 2000,
        min: 1500,
      },
      {
        title: 'Low productivity, Laziness',
        color: colorCodes.BAD,
        max: 1500,
        min: 1200,
      },
      {
        title: 'Time to refresh air',
        color: colorCodes.OK,
        max: 1200,
        min: 800,
      },
      {
        title: 'Fresh air',
        color: colorCodes.GOOD,
        max: 800,
        min: 400,
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: 'Pollution, PM 1.0',
    field: 'pm1p0',
    fieldTitle: 'PM 1.0',
    color: 'orange',
    yMin: 0,
    yMax: 100,
    unit: 'μg/m3',
    references: [
      {
        title: 'Hazardous',
        color: colorCodes.DANGER,
        max: 500,
        min: 100,
        placement: 'end',
      },
      {
        title: 'Very Unhealthy',
        color: colorCodes.VERY_BAD,
        max: 100,
        min: 55,
      },
      {
        title: 'Unhealthy',
        color: colorCodes.BAD,
        max: 55,
        min: 35,
      },
      {
        title: 'Moderate',
        color: colorCodes.OK,
        max: 35,
        min: 12,
      },
      {
        title: 'Clean',
        color: colorCodes.GOOD,
        max: 12,
        min: 0,
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: 'Pollution, PM 2.5',
    field: 'pm2p5',
    fieldTitle: 'PM 2.5',
    color: 'orange',
    yMin: 0,
    yMax: 100,
    unit: 'μg/m3',
    references: [
      {
        title: 'Hazardous',
        color: colorCodes.DANGER,
        max: 500,
        min: 100,
        placement: 'end',
      },
      {
        title: 'Very Unhealthy',
        color: colorCodes.VERY_BAD,
        max: 100,
        min: 55,
      },
      {
        title: 'Unhealthy',
        color: colorCodes.BAD,
        max: 55,
        min: 35,
      },
      {
        title: 'Moderate',
        color: colorCodes.OK,
        max: 35,
        min: 12,
      },
      {
        title: 'Clean',
        color: colorCodes.GOOD,
        max: 12,
        min: 0,
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: 'Pollution, PM 10',
    field: 'pm10p0',
    fieldTitle: 'PM 10',
    color: 'orange',
    yMin: 0,
    yMax: 300,
    unit: 'μg/m3',
    references: [
      {
        title: 'Hazardous',
        color: colorCodes.DANGER,
        max: 500,
        min: 300,
        placement: 'end',
      },
      {
        title: 'Very Unhealthy',
        color: colorCodes.VERY_BAD,
        max: 300,
        min: 120,
      },
      {
        title: 'Unhealthy',
        color: colorCodes.BAD,
        max: 120,
        min: 80,
      },
      {
        title: 'Moderate',
        color: colorCodes.OK,
        max: 80,
        min: 40,
      },
      {
        title: 'Clean',
        color: colorCodes.GOOD,
        max: 40,
        min: 0,
      },
    ],
    chart: undefined,
    widget: undefined,
  },
  {
    title: 'Time alive',
    isForDebugOnly: true,
    field: 'u',
    color: 'forestgreen',
    yMin: 0,
    unit: 'Sec',
    chart: undefined,
    widget: undefined,
  },
];
const lastUpdatedAgoEl = document.querySelector('#lastUpdatedAgo');
const filtersEl = document.querySelector('#filters');
const fcmTokenPath = `fcmToken`;
let db, messaging, latestEntry, unsubscribeFromReatimeUpdates, activeFilterId;

document.addEventListener('DOMContentLoaded', async function () {
  buildCharts();
  initFirebase();
  filterRecent(3);
  setInterval(updateLastUpdatedAgoText, 1000);
  initPushNotifications();
});

function initFirebase() {
  if (typeof firebase === 'undefined')
    throw new Error('hosting/init-error: Firebase SDK not detected. You must include it before /__/firebase/init.js');

  firebase.initializeApp({
    projectId: 'antonshubin-production',
    apiKey: 'AIzaSyDVlnsfBwKSGiQUWKCIcfAeuCqUKhNiaQo',
    authDomain: 'antonshubin-production.firebaseapp.com',
    databaseURL: 'https://antonshubin-production.firebaseio.com',
    storageBucket: 'antonshubin-production.appspot.com',
    messagingSenderId: '1075534054040',
    appId: '1:1075534054040:web:0354532ec04948be7c86ed',
    measurementId: 'G-W2MMXJMEZC',
  });

  db = firebase.firestore();
  messaging = firebase.messaging();
}

function buildCharts() {
  const container = document.querySelector('#charts');

  getModels().forEach(m => {
    container.innerHTML += `
        <div class="chart">
            <h3 class="chart__title">
              <span>
                ${m.title || capitalize(m.field)}
              </span>
              <span class="chart__title__value-wrapper">
                <span class="chart__title__value" id="chart-value-${m.field}">0</span>
                <span class="chart__title__unit">${m.unit}</span>
             </span>
            </h3>
            <canvas id="chart-${m.field || m.title}" width="400" height="400"></canvas>
        </div>`;

    setTimeout(() => {
      m.widget = container.querySelector(`#chart-value-${m.field}`);
    });
  });

  const tension = 0.5;
  const defaultColor = 'rgb(75, 192, 192)';
  const pointRadius = 3;
  const pointHoverRadius = 6;

  function referenceToAnnotation(reference) {
    return {
      type: 'box',
      backgroundColor:
        typeof reference.color === 'string' || reference.color instanceof String
          ? reference.color
          : getReferenceBoxColor(reference.color),
      borderWidth: 0,
      yMax: reference.max,
      yMin: reference.min,
      label: {
        drawTime: 'afterDraw',
        display: true,
        content: reference.title,
        position: {
          x: 'center',
          y: reference.placement || 'center',
        },
      },
    };
  }

  function syncTooltips(chart) {
    let activeTooltip = null;
    return () => {
      const tooltipItem = chart.tooltip._tooltipItems[0];
      if (tooltipItem) {
        activeTooltip = tooltipItem;
      } else if (activeTooltip) {
        // clear all tooltips
        activeTooltip = null;
        removeTooltips();
        return;
      } else {
        return;
      }

      getModels().forEach(m => {
        if (m.chart === chart) {
          return; // skip itself
        }
        m.chart.tooltip.setActiveElements([
          { datasetIndex: activeTooltip.datasetIndex, index: activeTooltip.dataIndex },
        ]);
        m.chart.setActiveElements([{ datasetIndex: activeTooltip.datasetIndex, index: activeTooltip.dataIndex }]);
        m.chart.update();
      });
    };
  }
  function removeTooltips(exceptChart) {
    getModels().forEach(m => {
      if (m.chart === exceptChart) {
        return; // skip itself
      }
      m.chart.tooltip.setActiveElements([], { x: 0, y: 0 });
      m.chart.setActiveElements([], { x: 0, y: 0 });
      m.chart.update();
    });
  }

  getModels().forEach(m => {
    const data = {
      labels: [],
      datasets: [
        {
          label: (m.fieldTitle || capitalize(m.field)) + ', ' + m.unit,
          borderColor: m.color || defaultColor,
          backgroundColor: m.color || defaultColor,
          tension,
          pointRadius,
          pointHoverRadius,
          data: [],
        },
      ],
    };

    const config = {
      type: 'line',
      data: data,
      options: {
        interaction: {
          mode: 'index',
          intersect: false,
        },
        plugins: {
          legend: false,
          annotation: {
            common: {
              drawTime: 'beforeDraw',
            },
            annotations: m.references && m.references.map(r => referenceToAnnotation(r)),
          },
          tooltip: {
            displayColors: false,
            callbacks: {
              label: tooltip => `${tooltip.parsed.y} ${m.unit}`,
            },
          },
        },
        scales: {
          x: {
            type: 'time',
            time: {
              tooltipFormat: 'HH:mm',
              displayFormats: {
                hour: `HH:mm`,
              },
            },
          },
          y: {
            suggestedMin: m.yMin,
            suggestedMax: m.yMax,
            title: {
              display: true,
              text: m.unit,
            },
          },
        },
      },
    };

    const ctx = document.getElementById(`chart-${m.field || m.title}`).getContext('2d');
    m.chart = new Chart(ctx, config);
    m.chart.canvas.onmousemove = syncTooltips(m.chart);
    m.chart.canvas.onmouseout = removeTooltips;
  });
}

function getModels() {
  if (location.search.includes('debug')) {
    return models;
  }
  return models.filter(m => !m.isForDebugOnly);
}

function updateCharts(entry) {
  getModels().forEach(m => {
    // remove old values
    const now = new Date();
    const startFromMs = new Date().setTime(now.getTime() - activeFilterId * 60 * 60 * 1000);
    const startFromIndex = m.chart.data.labels.findIndex(l => l.getTime() > startFromMs);
    m.chart.data.labels.splice(0, startFromIndex);
    m.chart.data.datasets[0].data.splice(0, startFromIndex);

    // add new value
    const value = entry[m.field];
    m.widget.innerHTML = value;
    m.widget.parentElement.style.backgroundColor = getColorByValue(value, m);
    m.chart.data.labels.push(entry.at);
    if (m.field) {
      m.chart.data.datasets[0].data.push(entry[m.field]);
    }
  });
}

function updateLastUpdatedAgoText() {
  let text;
  if (!latestEntry) {
    text = `Loading data...`;
  } else {
    text = `Last updated ${ago(latestEntry.at)}`;
  }
  lastUpdatedAgoEl.innerHTML = text;
}

function ago(date) {
  const milliseconds = new Date().getTime() - date.getTime();
  const seconds = milliseconds / 1000;
  if (seconds < 10) {
    return `just now`;
  }
  if (seconds < 120) {
    return `${seconds.toFixed()} seconds ago`;
  }
  const minutes = seconds / 60;
  if (minutes < 60) {
    return `${minutes.toFixed()} minutes ago`;
  }
  return `more than 1 hour ago`;
}

function capitalize(string) {
  return string.charAt(0).toUpperCase() + string.slice(1);
}

function getColorByValue(value, model) {
  if (!model.references) {
    return 'grey';
  }
  const reference = model.references.find(r => r.min <= value && value <= r.max);
  if (!reference) {
    return 'grey';
  }
  return typeof reference.color === 'string' || reference.color instanceof String
    ? reference.color
    : `rgb(${reference.color.join(',')}`;
}

function getReferenceBoxColor(colorCode) {
  return `rgba(${colorCode.join(', ')}, ${chartReferenceBoxTransparency})`;
}

function filterRecent(hoursBefore) {
  if (unsubscribeFromReatimeUpdates) {
    unsubscribeFromReatimeUpdates();
    getModels().forEach(m => {
      m.chart.data.labels = [];
      if (m.field) {
        m.chart.data.datasets[0].data = [];
      }
      m.chart.update();
    });
  }
  activeFilterId = hoursBefore;
  filtersEl.querySelectorAll(`[data-filter-id]`).forEach(filterEl => filterEl.classList.remove('active'));
  filtersEl.querySelector(`[data-filter-id="${activeFilterId}"]`).classList.add('active');
  const dateBefore = new Date(new Date().getTime() - hoursBefore * 60 * 60 * 1000);
  unsubscribeFromReatimeUpdates = db
    .collection('aqs')
    .where('at', '>', dateBefore)
    .orderBy('at')
    .onSnapshot(querySnapshot => {
      querySnapshot.docChanges().forEach(change => {
        if (change.type === 'added') {
          const d = change.doc;
          latestEntry = { id: d.id, ...d.data(), at: d.data().at.toDate() };
          updateCharts(latestEntry);
          updateLastUpdatedAgoText();
        }
      });
      getModels().forEach(m => m.chart.update());
    });
}

function initPushNotifications() {
  if (!('Notification' in window)) {
    return;
  }
  if (localStorage.getItem(fcmTokenPath)) {
    window.unsubscribeFromNotificationsButton.style.display = 'block';
  } else {
    window.subscribeToNotificationsButton.style.display = 'block';
  }
}

async function subscribeToNotifications(clickEvent) {
  const button = clickEvent.currentTarget;
  button.querySelector('.content').style.display = 'none';
  button.querySelector('.loader').style.display = 'block';
  console.log('Requesting permission...');
  const permission = await Notification.requestPermission();
  if (permission !== 'granted') {
    console.log('Notification permission was not granted.');
    return;
  }
  console.log('Notification permission granted.');
  try {
    const token = await messaging.getToken();
    if (!token) {
      console.error('No registration token available. Request permission to generate one.');
    }
    console.log({ token });
    const response = await fetch(`/api/aqs/push-notifications`, {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json',
      },
      body: JSON.stringify({
        token,
      }),
    });
    console.log({ response });
    localStorage.setItem(fcmTokenPath, token);
    window.unsubscribeFromNotificationsButton.style.display = 'block';
    window.subscribeToNotificationsButton.style.display = 'none';
  } catch (err) {
    console.error('An error occurred while retrieving token. ', err);
  } finally {
    button.querySelector('.content').style.display = 'block';
    button.querySelector('.loader').style.display = 'none';
  }
}

async function unsubscribeFromNotifications(clickEvent) {
  const button = clickEvent.currentTarget;
  button.querySelector('.content').style.display = 'none';
  button.querySelector('.loader').style.display = 'block';
  try {
    const token = localStorage.getItem(fcmTokenPath);
    if (token) {
      console.log({ token });
      // Send the token to your server and update the UI if necessary
      const response = await fetch(`/api/aqs/push-notifications`, {
        method: 'DELETE',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify({
          token,
        }),
      });
      console.log({ response });
      localStorage.removeItem(fcmTokenPath);
    }
    window.unsubscribeFromNotificationsButton.style.display = 'none';
    window.subscribeToNotificationsButton.style.display = 'block';
  } catch (error) {
    console.error(error);
  } finally {
    button.querySelector('.content').style.display = 'block';
    button.querySelector('.loader').style.display = 'none';
  }
}
