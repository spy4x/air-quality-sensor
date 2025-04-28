<script lang="ts">
    import {createEventDispatcher} from "svelte";

    export let hours = 1;
    const dispatch = createEventDispatcher();

    const filterHoursOptions = [1, 3, 6, 12, 24];

    function onChange(e: Event) {
        dispatch("set", {hours: (e.target as HTMLSelectElement).value});
    }
</script>

<div class="flex align-center justify-center">
    <select
            class="mt-2 block rounded-md border-0 py-1.5 pl-3 pr-5 text-gray-900 ring-1 ring-inset ring-gray-300 focus:ring-2 focus:ring-indigo-600 text-lg sm:leading-6"
            on:change={onChange}
    >
        {#each filterHoursOptions as filterHoursOption}
            <option value={filterHoursOption} selected={filterHoursOption === hours}>{filterHoursOption}h</option>
        {/each}
    </select>
</div>
