import { Tabs, Tab } from './Tab';

export default function AppSkeleton() {
  return (
    <Tabs>
      <Tab name="ROM Config">
        <main>
          <h1>Loading...</h1>
        </main>
      </Tab>
    </Tabs>
  );
}
