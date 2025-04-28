<script lang="ts">
    import Chart from './chart.svelte';
    import type {Measurement} from "@repo/shared-types";
    import {models} from "./models";
    import {browser} from "$app/environment";

    export let measurements: Measurement[] = [];
    let isDebugParamInURL = browser && new URLSearchParams(window.location.search).has('debug');
</script>

<div class="container mt-10 pb-10 flex flex-wrap gap-10 align-center justify-center">
    <!--    <pre>{JSON.stringify(measurements, null, 4)}</pre>-->
    {#each models as model}
        {#if !model.isForDebugOnly || isDebugParamInURL}
            <Chart {measurements} model={model}/>
        {/if}
    {/each}
</div>