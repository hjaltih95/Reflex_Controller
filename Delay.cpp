/* -------------------------------------------------------------------------- *
 *                      OpenSim:  Delay.cpp                                   *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2021 Stanford University, TU Delft and the Authors      *
 * Author(s): Ajay Seth, Hjalti Hilmarsson                                    *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may    *
 * not use this file except in compliance with the License. You may obtain a  *
 * copy of the License at http://www.apache.org/licenses/LICENSE-2.0.         *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an "AS IS" BASIS,          *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied    *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 * -------------------------------------------------------------------------- */



//=============================================================================
// INCLUDES
//=============================================================================
#include "Delay.h"
#include <OpenSim/OpenSim.h>
#include "OpenSim/Simulation/Model/Muscle.h"



// This allows us to use OpenSim functions, classes, etc., without having to
// prefix the names of those things with "OpenSim::".
using namespace OpenSim;
using namespace std;
using namespace SimTK;


//=============================================================================
// CONSTRUCTOR(S) AND DESTRUCTOR
//=============================================================================
//_____________________________________________________________________________
/* Default constructor. */
Delay::Delay()
{
    constructProperties();
}

/* Convenience constructor. */
Delay::Delay(const std::string& name,
             const Muscle& muscle,
             double delay)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    connectSocket_muscle(muscle);
    
    constructProperties();
    set_delay(delay);


}

//=============================================================================
// SETUP PROPERTIES
//=============================================================================
//
/* Connect properties to local pointers.
 *
 * Properties should be given a meaningful name and an informative comment.
 * The name you give each property is the tag that will be used in the XML
 * file. The comment will appear before the property in the XML file.
 * In addition, the comments are used for tool tips in the OpenSim GUI.
 *
 * All properties are added to the property set. Once added, they can be
 * read in and written to files.
 ____________________________________________________________________________


 * Construct Properties
 */
void Delay::constructProperties()
{
    
}

void Delay::addToSystem(SimTK::MultibodySystem& system) const
{
    Super::addToSystem(system);
    Delay* mutableThis = const_cast<Delay *>(this);
    
}


void Delay::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);
    muscleHistory.setSize(0);
    muscleHistory.setMemoryOwner(true);
    
    const Muscle& musc = getMuscle();
    
    PiecewiseLinearFunction muscleSignal;
    muscleSignal.setName(musc.getName());
    muscleHistory.cloneAndAppend(muscleSignal);
    
}

//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Muscle& Delay::getMuscle() const
{
    return getSocket<Muscle>("muscle").getConnectee();
}


//=============================================================================
// SIGNALS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals after a delay /tau
 *
 * @param s         current state of the system
 */

double Delay::getSignal(const SimTK::State& s) const
{
    double signal = getInputValue<double>(s, "signal");
    double time = s.getTime();
    double delaySignal = 0;
    
    const Muscle& musc = getMuscle();
    
    muscleHistory.get(musc.getName()).addPoint(time, signal);
    
    if((time - get_delay()) < muscleHistory.get(musc.getName()).getXValues()[0])
    {
        delaySignal = 0;
    }
    else
    {
        delaySignal = muscleHistory.get(musc.getName()).calcValue(SimTK::Vector(1,time-get_delay()));
    }
    
    return delaySignal;
}

