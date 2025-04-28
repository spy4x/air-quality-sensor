import type { Measurement } from '@repo/shared-types';

export const chartReferenceBoxTransparency = 0.2;
const colorCodes = {
	GOOD: [33, 166, 5],
	OK: [239, 200, 2],
	BAD: [255, 153, 0],
	VERY_BAD: [255, 68, 0],
	DANGER: [253, 0, 0]
};

export interface ChartReference {
	title: string;
	color: string | number[];
	max: number;
	min: number;
	placement?: 'start' | 'end';
}

export interface Model {
	title: string;
	field: keyof Measurement;
	fieldTitle?: string;
	color: string;
	yMin: number;
	yMax?: number;
	unit?: string;
	references?: ChartReference[];
	isForDebugOnly?: boolean;
}

export const models: Model[] = [
	{
		title: `Freshness, CO2`,
		field: 'co2',
		fieldTitle: 'CO2',
		color: 'rgb(75, 192, 192)',
		yMin: 400,
		yMax: 1200,
		unit: 'ppm',
		references: [
			{
				title: 'Headache',
				color: colorCodes.VERY_BAD,
				max: 2000,
				min: 1500,
				placement: 'end'
			},
			{
				title: 'Low productivity, Laziness',
				color: colorCodes.BAD,
				max: 1500,
				min: 1200,
				placement: 'end'
			},
			{
				title: 'Time to refresh air',
				color: colorCodes.OK,
				max: 1200,
				min: 800
			},
			{
				title: 'Fresh air',
				color: colorCodes.GOOD,
				max: 800,
				min: 400
			}
		]
	},
	{
		title: 'Pollution, PM 2.5',
		field: 'pm2p5',
		fieldTitle: 'PM 2.5',
		color: 'orange',
		yMin: 0,
		yMax: 35,
		unit: 'μg/m3',
		references: [
			{
				title: 'Hazardous',
				color: colorCodes.DANGER,
				max: 500,
				min: 100,
				placement: 'end'
			},
			{
				title: 'Very Unhealthy',
				color: colorCodes.VERY_BAD,
				max: 100,
				min: 55,
				placement: 'end'
			},
			{
				title: 'Unhealthy',
				color: colorCodes.BAD,
				max: 55,
				min: 35,
				placement: 'end'
			},
			{
				title: 'Moderate',
				color: colorCodes.OK,
				max: 35,
				min: 12
			},
			{
				title: 'Clean',
				color: colorCodes.GOOD,
				max: 12,
				min: 0
			}
		]
	},
	{
		title: 'Temperature',
		field: 'temperature',
		fieldTitle: '',
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
				placement: 'end'
			},
			{
				title: 'Hot',
				color: colorCodes.OK,
				max: 32,
				min: 27
			},
			{
				title: 'Optimum',
				color: colorCodes.GOOD,
				max: 27,
				min: 22
			},
			{
				title: 'Chill',
				color: `rgba(0,255,233,${chartReferenceBoxTransparency})`,
				max: 22,
				min: 15,
				placement: 'start'
			},
			{
				title: 'Cold',
				color: `rgba(0,33,255,${chartReferenceBoxTransparency})`,
				max: 15,
				min: -50,
				placement: 'start'
			}
		]
	},
	{
		title: 'Humidity',
		field: 'humidity',
		fieldTitle: '',
		color: 'blue',
		unit: '%',
		yMin: 25,
		yMax: 70,
		references: [
			{
				title: 'Too humid',
				color: colorCodes.BAD,
				max: 100,
				min: 70,
				placement: 'end'
			},
			{
				title: 'Moderate',
				color: colorCodes.OK,
				max: 70,
				min: 60
			},
			{
				title: 'Optimum',
				color: colorCodes.GOOD,
				max: 60,
				min: 40
			},
			{
				title: 'Moderate',
				color: colorCodes.OK,
				max: 40,
				min: 25
			},
			{
				title: 'Too dry',
				color: colorCodes.BAD,
				max: 25,
				min: 0,
				placement: 'start'
			}
		]
	},

	{
		title: 'Time alive',
		isForDebugOnly: true,
		field: 'uptime',
		fieldTitle: '',
		color: 'forestgreen',
		yMin: 0,
		unit: 'Sec'
	}
];
