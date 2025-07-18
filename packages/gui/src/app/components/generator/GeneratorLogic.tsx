import { NavTop, Tab } from '../nav';
import { Tricks } from '../Tricks';
import { JunkLocations } from '../JunkLocations';

export function GeneratorLogic() {
  return (
    <NavTop>
      <Tab name="Tricks"><Tricks glitches={false}/></Tab>
      <Tab name="Glitches"><Tricks glitches={true}/></Tab>
      <Tab name="Junk Locations"><JunkLocations/></Tab>
    </NavTop>
  );
}
