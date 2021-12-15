#ifndef OPENSIM_SimpleSpindle_H_
#define OPENSIM_SimpleSpindle_H_
/* -------------------------------------------------------------------------- *
 *                      OpenSim: SimpleSpindle.h                              *
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


//============================================================================
// INCLUDE
//============================================================================
#include "osimSimpleSpindleDLL.h"
#include "OpenSim/Simulation/Model/Muscle.h"
#include "OpenSim/Simulation/Model/ModelComponent.h"
#include "OpenSim/Simulation/Control/Controller.h"



namespace OpenSim {

//=============================================================================
//=============================================================================
/**
 * ToyReflexController is a concrete controller that excites muscles in response
 * to muscle lengthening to simulate a simple stretch reflex. This controller 
 * is meant to serve as an example how to implement a controller in
 * OpenSim. It is intended for demonstration purposes only. 
 *
 * @author  Ajay Seth
 */
class OSIMSIMPLESPINDLE_API SimpleSpindle : public ModelComponent {
OpenSim_DECLARE_CONCRETE_OBJECT(SimpleSpindle, ModelComponent);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    
    OpenSim_DECLARE_PROPERTY(normalized_rest_length, double,
        "The intended rest length of the spindle");
//==============================================================================
// SOCKETS
//==============================================================================

    OpenSim_DECLARE_SOCKET(muscle, Muscle, "The muscle that the spindle measures");
    
//=============================================================================
// OUTPUTS
//=============================================================================
    // we get our propriceptive afferents
    OpenSim_DECLARE_OUTPUT(signal, double, getSignal, SimTK::Stage::Position);
    // add outputs for Ia and II afferents
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    SimpleSpindle();
    SimpleSpindle( const std::string& name,
                  const Muscle& muscle,
                  double rest_length);

    // Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.

//--------------------------------------------------------------------------
// SPINDLE PARAMETER ACCESSORS
//--------------------------------------------------------------------------
    // get/set the rest length of the spindle
    double getNormalizedRestLength() const;
    void setNormalizedRestLength(double normalizedRestLength);
    
  // SOCKET get/set
    
    //get the muscle frame to witch this spindle attatches to
    const Muscle& getMuscle() const;
    
    
//--------------------------------------------------------------------------
// SPINDLE STATE DEPENDENT ACCESSORS
//--------------------------------------------------------------------------
/** @name Spindle State Dependendt Access Methods
    Get quanitites of interest common to all spindles*/
    void setSignal(SimTK::State& s, double signal) const;
    double getSignal(const SimTK::State& s) const;
    



private:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& aModel) override;

    
protected:
    double _normalizedRestLength;
    //=========================================================================
};  // END of class SimpleSpindle

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_SimpleSpindle_H_


