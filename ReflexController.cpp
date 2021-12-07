/* -------------------------------------------------------------------------- *
 *                      OpenSim:  ReflexController.cpp                           *
 * -------------------------------------------------------------------------- *
 * The OpenSim API is a toolkit for musculoskeletal modeling and simulation.  *
 * See http://opensim.stanford.edu and the NOTICE file for more information.  *
 * OpenSim is developed at Stanford University and supported by the US        *
 * National Institutes of Health (U54 GM072970, R24 HD065690) and by DARPA    *
 * through the Warrior Web program.                                           *
 *                                                                            *
 * Copyright (c) 2005-2017 Stanford University and the Authors                *
 * Author(s): Ajay Seth                                                       *
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
#include "ReflexController.h"
#include <OpenSim/OpenSim.h>
#include "OpenSim/Simulation/Model/Muscle.h"
#include "SimpleSpindle.h"


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
ReflexController::ReflexController()
{
    constructProperties();
}

/* Convenience constructor. */
ReflexController::ReflexController(const std::string& name,
                                   const Muscle& muscle,
                                   double rest_length,
                                   double gain)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    connectSocket_muscle(muscle);
    
    constructProperties();
    set_normalized_rest_length(rest_length);
    set_gain(gain);
    /*
    set_gain_length(gain_l);
    set_gain_velocity(gain_v);
     */
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
void ReflexController::constructProperties()
{
    /*
    constructProperty_gain_length(1.0);
    constructProperty_gain_velocity(1.0);
     */
    constructProperty_normalized_rest_length(1.0);
    constructProperty_gain(1.0);
}

void ReflexController::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);

    Set<const Actuator>& actuators = updActuators();
    
    int cnt=0;
    
    while(cnt < actuators.getSize()){
        const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[cnt]);
        // control muscles only
        if(!musc){
            log_warn("ToyReflexController assigned a non-muscle actuator '{}', "
                    "which will be ignored.", actuators[cnt].getName());
            actuators.remove(cnt);
        }else
            cnt++;
    }
}

//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
// Muscle to control
//-----------------------------------------------------------------------------



//=============================================================================
// COMPUTATIONS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals for spindles
 *
 * @param s         current state of the system
 * @param controls  system wide controls to which this component can read off
 */

void ReflexController::computeControls(const State& s,
                                          Vector &controls) const {
    // get time
    double t = s.getTime();
    

    double f_o = 0;
    double stretch = 0;
    // muscle lengthening speed
    double speed = 0;
    // max muscle lengthening (stretch) speed
    double max_speed = 0;
    //reflex control
    double control = 0;
    
    const Muscle& musc = getMuscle();

    const SimpleSpindle *spindle = new SimpleSpindle();
    
    stretch = spindle->getSignal(s);
    
    f_o = musc.getOptimalFiberLength();
    
    control = 0.5*get_gain()*(fabs(stretch)+stretch)/f_o;

    SimTK::Vector actControls(1,control);
    // add reflex controls to whatever controls are already in place.
    musc.addInControls(actControls, controls);
        
}

