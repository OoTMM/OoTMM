
export type RawRoom = {
  sceneId: number;
  roomId: number;
  setupId: number;
  vromBase: number;
  vromHeader: number;
};

export type Actor = {
  actorId: number;
  halfDays: number;
  typeId: number;
  rx: number;
  ry: number;
  rz: number;
  params: number;
  pos: [number, number, number];
};

export type RoomActors = {
  sceneId: number;
  roomId: number;
  setupId: number;
  actors: Actor[];
};

export type RoomActor = {
  sceneId: number;
  roomId: number;
  setupId: number;
  actor: Actor;
};

export type AddressingTable = {
  scenesTable: number[];
  setupsTable: number[];
  roomsTable: number[];
  bitCount: number;
};

export type Check = {
  name: string;
  name2?: string;
  type: string;
  subtype?: string;
  item: string;
  roomActor: RoomActor;
  sliceId?: number;
  letter?: string;
};

export type ActorHandler = (checks: Check[], actor: RoomActor) => void;
export type ActorHandlers = { [actorId: number]: ActorHandler };
