using RocketSim;

RocketSim.RocketSim.RocketSim.StringInit("D:\\rlbot\\collision_meshes");

Arena arena = Arena.Create(GameMode.SOCCAR, 120f);

Car car = arena.AddCar(Team.BLUE, CarConfig.CAR_CONFIG_OCTANE);

CarState carState = new CarState();
carState.Pos = new Vec(0, 0, 17);
carState.Vel = new Vec(50, 0, 0);
car.State = carState;

BallState ballState = new BallState();
ballState.Pos = new Vec(0, 400, 100);
arena.Ball.State = ballState;

car.Controls.Throttle = 1;
car.Controls.Steer = 1;

arena.Step(100);

string carPos = $"{car.State.Pos.X}, {car.State.Pos.Y}, {car.State.Pos.Z}";
Console.WriteLine($"After {arena.TickCount} ticks, our car is at: {carPos}");
