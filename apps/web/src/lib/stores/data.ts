import type {Measurement} from "@repo/shared-types";
import {get, writable} from "svelte/store";
import {browser} from "$app/environment";

interface Store {
    fetchedAt: null | Date;
    measurements: Measurement[];
    filterHours: number;
    isLoading: boolean;
    error: null | string;
}

const initialStore: Store = {
    fetchedAt: null,
    measurements: [],
    filterHours: 1,
    isLoading: false,
    error: null,
}

const store = writable<Store>(initialStore);

async function load(filterHours?: number) {
    if(!filterHours) {
        filterHours = get(store).filterHours;
    }
    store.update(state => ({...state, filterHours: filterHours || 1, isLoading: true, error: null}));
    try {
        const response = await fetch(`/api/aqs?hours=${filterHours}`);
        const measurements = await response.json();
        store.update(state => ({...state, measurements, isLoading: false, error: null, fetchedAt: new Date()}));
    } catch (e: unknown) {
        store.update(state => ({...state, isLoading: false, error: (e as Error).message}));
    }
}

export const data = {
    subscribe: store.subscribe,
    setFilterHours: async (hours: number): Promise<void> => {
        await load(hours);
    }
}

if (browser) {
    void load(initialStore.filterHours);
    setInterval(() => void load(), 2 * 60 * 1000);
}