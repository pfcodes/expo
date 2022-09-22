import * as React from 'react';

import {
  getCachedUpdates,
} from '../native-modules/DevLauncherInternal';

type App = {
  id: string;
  url: string;
  name: string;
  timestamp: number;
};

export type RecentApp =
  | (App & {
      isEASUpdate: true;
      branchName: string;
      updateMessage: string;
    })
  | (App & { isEASUpdate: false });

type CachedUpdates = {
  cachedUpdates: RecentApp[];
  setCachedUpdates: ((cachedUpdates: RecentApp[]) => void)
};

const Context = React.createContext<CachedUpdates>({
  cachedUpdates: [],
  setCachedUpdates: (_cachedUpdates: RecentApp[]) => {}
});

type CachedUpdatesProviderProps = {
  children: React.ReactNode;
};

export function CachedUpdatesProvider({
  children
}: CachedUpdatesProviderProps) {
  const [cachedUpdates, setCachedUpdates] = React.useState<RecentApp[]>([]);
  return <Context.Provider value={{ cachedUpdates, setCachedUpdates }}>{children}</Context.Provider>;
}

export function useCachedUpdates() {
  const [error, setError] = React.useState('');
  const [isFetching, setIsFetching] = React.useState(false);
  const { cachedUpdates, setCachedUpdates } = React.useContext(Context);

  React.useEffect(() => {
    setIsFetching(true);
    getCachedUpdates()
      .then((apps) => {
        console.log('useCachedUpdates: apps.length = ' + apps.length);
        // use a map to index apps by their url:
        const cachedUpdates: { [id: string]: RecentApp } = {};

        for (const app of apps) {
          // index by url to eliminate multiple bundlers with the same address
          const id = `${app.url}`;
          app.id = id;

          const previousTimestamp = cachedUpdates[id]?.timestamp ?? 0;

          if (app.timestamp > previousTimestamp) {
            cachedUpdates[id] = app;
          }
        }

        // sorted by most recent timestamp first
        const sortedByMostRecent = Object.values(cachedUpdates).sort(
          (a, b) => b.timestamp - a.timestamp
        );

        console.log('useCachedUpdates: sortedByMostRecent.length = ' + sortedByMostRecent.length);
        setCachedUpdates(sortedByMostRecent);
        setIsFetching(false);
      })
      .catch((error) => {
        console.log('useCachedUpdates error');
        setIsFetching(false);
        setError(error.message);
        setCachedUpdates([]);
      });
  }, []);
  return {
    data: cachedUpdates,
    isFetching,
    error,
  };
}
