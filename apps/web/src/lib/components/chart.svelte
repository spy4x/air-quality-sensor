<script lang="ts">
    import {chartReferenceBoxTransparency, type Model} from "./models";
    import Chart from 'chart.js/auto';
    import type {Measurement} from "@repo/shared-types";

    export let model: Model;
    export let measurements: Measurement[];

    let chartCanvas: HTMLCanvasElement;
    let chart: null | Chart;
    const tension = 0.5;
    const defaultColor = 'rgb(75, 192, 192)';
    const pointRadius = 3;
    const pointHoverRadius = 6;

    function getMeasurements(field: string): { value: number, createdAt: Date }[] {
        return measurements.map(m => ({value: m[field as keyof Measurement], createdAt: m.createdAt}))
    }

    $: data = measurements && getMeasurements(model.field)


    $: datalines = {
        labels: data.map(d => d.createdAt),
        datasets: [
            {
                label: (model.fieldTitle) + ', ' + model.unit,
                borderColor: model.color || 'rgb(75, 192, 192)',
                backgroundColor: model.color || 'rgb(75, 192, 192)',
                tension,
                pointRadius,
                pointHoverRadius,
                data: data.map(d => d.value),
            },
        ],
    }

    $: {
        measurements.toString(); // TODO: remove this hack
        if (chart) {
            // update
            console.log('update', model.field);
            chart.data = datalines;
            chart.update();
        } else if (chartCanvas) {
            // create
            console.log('create', model.field);
            const ctx = chartCanvas.getContext('2d');
            ctx && (chart = new Chart(ctx, {
                type: 'line',
                data: datalines,
                width: 400,
                height: 400,
            }))
        }
    }

    function getColorByValue(value: number) {
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

    //
    // function getReferenceBoxColor(colorCode: number[]) {
    //     return `rgba(${colorCode.join(', ')}, ${chartReferenceBoxTransparency})`;
    // }
    //
    // function capitalize(string: string): string {
    //     return string.charAt(0).toUpperCase() + string.slice(1);
    // }
    //
    // function referenceToAnnotation(reference) {
    //     return {
    //         type: 'box',
    //         backgroundColor:
    //             typeof reference.color === 'string' || reference.color instanceof String
    //                 ? reference.color
    //                 : getReferenceBoxColor(reference.color),
    //         borderWidth: 0,
    //         yMax: reference.max,
    //         yMin: reference.min,
    //         label: {
    //             drawTime: 'afterDraw',
    //             display: true,
    //             content: reference.title,
    //             position: {
    //                 x: 'center',
    //                 y: reference.placement || 'center',
    //             },
    //         },
    //     };
    // }
    //
    // function syncTooltips(chart) {
    //     let activeTooltip = null;
    //     return () => {
    //         const tooltipItem = chart.tooltip._tooltipItems[0];
    //         if (tooltipItem) {
    //             activeTooltip = tooltipItem;
    //         } else if (activeTooltip) {
    //             // clear all tooltips
    //             activeTooltip = null;
    //             removeTooltips();
    //             return;
    //         } else {
    //             return;
    //         }
    //
    //         getModels().forEach(m => {
    //             if (m.chart === chart) {
    //                 return; // skip itself
    //             }
    //             m.chart.tooltip.setActiveElements([
    //                 { datasetIndex: activeTooltip.datasetIndex, index: activeTooltip.dataIndex },
    //             ]);
    //             m.chart.setActiveElements([{ datasetIndex: activeTooltip.datasetIndex, index: activeTooltip.dataIndex }]);
    //             m.chart.update();
    //         });
    //     };
    // }
    // function removeTooltips(exceptChart) {
    //     getModels().forEach(m => {
    //         if (m.chart === exceptChart) {
    //             return; // skip itself
    //         }
    //         m.chart.tooltip.setActiveElements([], { x: 0, y: 0 });
    //         m.chart.setActiveElements([], { x: 0, y: 0 });
    //         m.chart.update();
    //     });
    // }
    //
    // getModels().forEach(m => {
    //     const data = {
    //         labels: [],
    //         datasets: [
    //             {
    //                 label: (m.fieldTitle || capitalize(m.field)) + ', ' + m.unit,
    //                 borderColor: m.color || defaultColor,
    //                 backgroundColor: m.color || defaultColor,
    //                 tension,
    //                 pointRadius,
    //                 pointHoverRadius,
    //                 data: [],
    //             },
    //         ],
    //     };
    //
    //     const config = {
    //         type: 'line',
    //         data: data,
    //         options: {
    //             interaction: {
    //                 mode: 'index',
    //                 intersect: false,
    //             },
    //             plugins: {
    //                 legend: false,
    //                 annotation: {
    //                     common: {
    //                         drawTime: 'beforeDraw',
    //                     },
    //                     annotations: m.references && m.references.map(r => referenceToAnnotation(r)),
    //                 },
    //                 tooltip: {
    //                     displayColors: false,
    //                     callbacks: {
    //                         label: (tooltip:any) => `${tooltip.parsed.y} ${m.unit}`,
    //                     },
    //                 },
    //             },
    //             scales: {
    //                 x: {
    //                     type: 'time',
    //                     time: {
    //                         tooltipFormat: 'HH:mm',
    //                         displayFormats: {
    //                             hour: `HH:mm`,
    //                         },
    //                     },
    //                 },
    //                 y: {
    //                     suggestedMin: m.yMin,
    //                     suggestedMax: m.yMax,
    //                     title: {
    //                         display: true,
    //                         text: m.unit,
    //                     },
    //                 },
    //             },
    //         },
    //     };
    //
    //     const element = document.getElementById(`chart-${m.field || m.title}`) as null | HTMLCanvasElement;
    //     if(!element) {
    //         console.error(`Element with id 'chart-${m.field || m.title}' not found`);
    //         return;
    //     }
    //     const ctx = element.getContext('2d');
    //     if(!ctx) {
    //         console.error(`Context for element with id 'chart-${m.field || m.title}' not found`);
    //         return;
    //     }
    //     m.chart = new Chart(ctx, config);
    //     m.chart.canvas.onmousemove = syncTooltips(m.chart);
    //     m.chart.canvas.onmouseout = removeTooltips;
    // });
</script>

<div class="flex items-center flex-col">
    <h2 class="text-2xl font-bold flex justify-center gap-2 items-center"
    >
        <span>{model.title}</span>
        <!-- Tag: -->
        <span class="text-sm border-0 rounded-full px-2 py-1" style="background-color: {getColorByValue(
        data[data.length - 1]?.value
    )}"
        >{data[data.length - 1]?.value} {model.unit}</span>
    </h2>

    <div style="max-width: 400px; max-height: 400px">
    <canvas bind:this={chartCanvas} height="400" id="canvas-{model.field}" width="400"></canvas>
    </div>
    <!--<pre>{-->
    <!--    JSON.stringify(data, null, 2)-->
    <!--}</pre>-->
</div>
