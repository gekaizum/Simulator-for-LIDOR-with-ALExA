// DroneMessages.msg

message SetBase
{
    string state = "SETBASE";
}

message TakeOff
{
    string state = "TAKEOFF";
}

message LandDrone
{
    string state = "LANDING";
}

message MoveTo
{
    string state = "MOVE";
}

message SetVelocity
{
    string state = "SETVEL";
}

message SetAcceleration
{
    string state = "SETACCEL";
}

message StopDrone
{
    string state = "STOP";
}

message PowerOnDrone
{
    string state = "POWER_ON";
}

message PowerOffDrone
{
    string state = "POWER_OFF";
}

message GetStatusDrone
{
    string state = "SENDSTAT";
}

message GetPositionDrone
{
    string state = "SEND_POS";
}

message GetAltitudeDrone
{
    string state = "SEND_ALT";
}

message GetBatteryDrone
{
    string state = "SEND_BATT";
}