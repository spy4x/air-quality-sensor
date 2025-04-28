<script lang="ts">
    import {Filter, Charts} from '@components';
    import {data} from '@stores'
    import {onMount} from "svelte";

    let secondsAgoTimer: NodeJS.Timeout;
    let updatedSecondsAgo = 0;

    onMount(() => {
        secondsAgoTimer = setInterval(() => {
            updatedSecondsAgo = $data.fetchedAt ? Math.round((Date.now() - $data.fetchedAt.getTime()) / 1000) : 0;
        }, 1000);
    });

</script>

<div class="container mx-auto text-white my-10" style="max-width: 1024px">
    <h1 class="text-5xl font-bold text-center ">Air Quality</h1>
    {#if $data.isLoading}
        <p>Loading...</p>
    {:else if $data.error}
        <p>Error: {$data.error}</p>
    {:else}
        <p class="text-center mt-1">Last updated {updatedSecondsAgo} seconds ago</p>
    {/if}

    <Filter hours={$data.filterHours} on:set={e => data.setFilterHours(e.detail.hours)} />
    <Charts measurements={$data.measurements} />
<!--    <pre>Store: {JSON.stringify($data, null, 2)}</pre>-->
</div>
