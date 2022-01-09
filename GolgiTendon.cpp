/* -------------------------------------------------------------------------- *
 *                      OpenSim:  GolgiTendon.cpp                           *
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
#include "GolgiTendon.h"
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
GolgiTendon::GolgiTendon()
{
    constructProperties();
}

/* Convenience constructor. */
GolgiTendon::GolgiTendon(const std::string& name,
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
void GolgiTendon::constructProperties()
{
    
}

void GolgiTendon::addToSystem(SimTK::MultibodySystem& system)const
{
    Super::addToSystem(system);
    GolgiTendon* mutableThis = const_cast<GolgiTendon *>(this);
}

void GolgiTendon::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);
    
    muscleTendonHistory.setSize(0);
    muscleTendonHistory.setMemoryOwner(true);
    
    const Muscle& musc = getMuscle();
    
    PiecewiseLinearFunction muscleTendon;
    muscleTendon.setName(musc.getName());
    muscleTendonHistory.cloneAndAppend(muscleTendon);
    
}

//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
// Golgi Tendon muscle
//-----------------------------------------------------------------------------
const Muscle& GolgiTendon::getMuscle() const
{
    return getSocket<Muscle>("muscle").getConnectee();
}


//=============================================================================
// SIGNALS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the tendon length for the Golgi Tendon
 *
 * @param s         current state of the system
 */

double GolgiTendon::getTendonLength(const SimTK::State& s) const
{
    double time = s.getTime();
    double length = 0;
    double tendon_length = 0;
    double tendon_slack_length = 0;
    double golgi_length = 0;
    
    const Muscle& musc = getMuscle();
    
    tendon_length = musc.getTendonLength(s);
    tendon_slack_length = musc.getTendonSlackLength();
    golgi_length = tendon_length - tendon_slack_length;
    
    muscleTendonHistory.get(musc.getName()).addPoint(time, golgi_length);
    
    if((time - get_delay()) < muscleTendonHistory.get(musc.getName()).getXValues()[0])
    {
        length = 0;
    }
    else
    {
        length = golgi_length;
    }
    
    return length;
}

