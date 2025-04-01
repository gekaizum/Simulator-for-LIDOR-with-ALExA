//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package Physical_layer.Drone_block;
package simulations;
import inet.queueing.base.PassivePacketSinkBase;
import inet.queueing.contract.IPassivePacketSink;

//
// This module is a passive packet sink which is pushed with packets by the
// connected packet producer. All pushed packets are counted and deleted.
//
simple CustomPacketSink extends PassivePacketSinkBase like IPassivePacketSink
{
    parameters:
        string clockModule = default(""); // relative path of a module that implements IClock; optional
        double initialConsumptionOffset @unit(s) = default(0s); // initial duration before which packets are not consumed
        volatile double consumptionInterval @unit(s) = default(0s); // elapsed time between subsequent packets allowed to be pushed by the connected packet producer, 0 means any number of packets can be pushed at the same simulation time
        bool scheduleForAbsoluteTime = default(true); // when a clock is used relative means that setting the clock will not affect the simulation time of the event
        //@class(CustomPacketSink);
    gates:
        input in @labels(push);
}