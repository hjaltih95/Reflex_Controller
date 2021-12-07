/* -------------------------------------------------------------------------- *
 *                      OpenSim:  SimpleSpindle.cpp                           *
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
#include "SimpleSpindle.h"
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
SimpleSpindle::SimpleSpindle()
{
    constructProperties();
}

/* Convenience constructor. */
SimpleSpindle::SimpleSpindle(const std::string& name,
                             const Muscle& muscle,
                             double rest_length)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
    
    setName(name);
    connectSocket_muscle(muscle);
    
    constructProperties();
    set_normalized_rest_length(rest_length);
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
void SimpleSpindle::constructProperties()
{
    /*
    constructProperty_gain_length(1.0);
    constructProperty_gain_velocity(1.0);
     */
    constructProperty_normalized_rest_length(1.0);
}

void SimpleSpindle::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);

    
}

//=============================================================================
// SIGNALS
//=============================================================================

double SimpleSpindle::getSignal(const SimTK::State& s) const
{
    
    double signal = 0;
    double rest_length = get_normalized_rest_length();
    // optimal fiber length
    double f_o = 1;
    // muscle length
    double length = 0;
    // muscle stretsch
    double stretch = 0;
    // muscle lengthening speed
    double speed = 0;

    const Muscle& musc = getMuscle();
    // get optimal fiber length and muscle length
    f_o = musc.getOptimalFiberLength();
    length = musc.getLength(s);
    // Compute stretch, the msucle spindle only monitors the muscle fiber length not the muscle-tendon length
    stretch = length-rest_length*f_o;
    // compute the muscle lengthening(stretch) speed
    speed = musc.getLengtheningSpeed(s);
    signal = stretch;
    
    return signal;
}

//=============================================================================
// GET AND SET
//=============================================================================

//-----------------------------------------------------------------------------
// Spindle frame
//-----------------------------------------------------------------------------
const Muscle& SimpleSpindle::getMuscle() const
{
    return getSocket<Muscle>("muscle").getConnectee();
}

//=============================================================================
// COMPUTATIONS
//=============================================================================
//_____________________________________________________________________________
/**
 * Compute the signals for spindles
 *
 * @param s         current state of the system
 * @param singals  system wide signals to which this component can read off
 */
/*
void SimpleSpindle::computeSignals(const State& s,
                                          Vector &signals) const {
    // get time
    double t = s.getTime();
    /*
    // get the list of actuators assigned to the reflex controller
    const Set<const Actuator>& actuators = getActuatorSet();
    */
 
    /*
    double rest_length = get_normalized_rest_length();
    
    // make it so spindle can connect to muscles, and many spindles can connect to one muscle

    
    // optimal fiber length
    double f_o = 1;
    // muscle length
    double length = 0;
    // muscle stretsch
    double stretch = 0;
    // muscle lengthening speed
    double speed = 0;
    // max muscle lengthening (stretch) speed
    double max_speed = 0;
    //reflex control
    double signal = 0;
    
    for(int i=0; i<spindles.getSize(); ++i){
        const Muscle *musc = dynamic_cast<const Muscle*>(&spindles[i]);
        // get optimal fiber length and muscle length
        f_o = musc->getOptimalFiberLength();
        length = musc->getLength(s);
        // Compute stretch, the msucle spindle only monitors the muscle fiber length not the muscle-tendon length
        stretch = length-rest_length*f_o;
        // compute the muscle lengthening(stretch) speed
        speed = musc->getLengtheningSpeed(s);
        
        // Create vectors that stores the values of length, stretch and speed
        SimTK::Vector musLength(1,length);
        SimTK::Vector musStretch(1,stretch);
        SimTK::Vector musSpeed(1,speed);
        
        
        // Read out muscle length
        signal = length;
        
        
        
        
        // un-normalize muscle's maximum contraction velocity (fib_lengths/sec)
        max_speed =
            musc->getOptimalFiberLength()*musc->getMaxContractionVelocity();
        control = 0.5*get_gain()*(fabs(speed)+speed)/max_speed;

        SimTK::Vector actControls(1,control);
        // add reflex controls to whatever controls are already in place.
        musc->addInControls(actControls, controls);
        
    }
}
*/

