import { NavSide, Tab } from './nav';

export default function AppSkeleton() {
  return (
    <NavSide>
      <Tab name="ROM Config">
        <main>
          <h1>Loading...</h1>
        </main>
      </Tab>
    </NavSide>
  );
}
