#ifndef OPENSIM_GolgiTendon_H_
#define OPENSIM_GolgiTendon_H_
/* -------------------------------------------------------------------------- *
 *                      OpenSim: GolgiTendon.h                              *
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


//============================================================================
// INCLUDE
//============================================================================
#include "osimGolgiTendonDLL.h"
#include "OpenSim/Simulation/Control/Controller.h"
#include "OpenSim/Simulation/Model/Muscle.h"



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
class OSIMGOLGITENDON_API GolgiTendon : public ModelComponent {
OpenSim_DECLARE_CONCRETE_OBJECT(GolgiTendon, ModelComponent);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    
//==============================================================================
// SOCKETS
//==============================================================================
    OpenSim_DECLARE_SOCKET(muscle, Muscle, "The muscle that the Golgi Tendon measures");
    
//=============================================================================
// OUTPUTS
//=============================================================================
    // we get our propriceptive afferents
    OpenSim_DECLARE_OUTPUT(length, double, getTLength, SimTK::Stage::Position);
    //
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    GolgiTendon();
    GolgiTendon(const std::string& name,
                const Muscle& muscle);

    // Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.
    
//--------------------------------------------------------------------------
// GOLGI TENDON PARAMETER ACCESSORS
//--------------------------------------------------------------------------
    
  // SOCKET get/set
    
    // get a reference to the muscle the golgi tendon is attatched to
    const Muscle& getMuscle() const;
    

//--------------------------------------------------------------------------
// GOLGI TENDON STATE DEPENDENT ACCESSORS
//--------------------------------------------------------------------------
/** @name Golgi Tendon State Dependendt Access Methods
    Get quanitites of interest common to all spindles*/
    void setTLength(SimTK::State& s, double signal) const;
    double getTLength(const SimTK::State& s) const;
        

private:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& aModel) override;

    
protected:
    //=========================================================================
};  // END of class GolgiTendon

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_GolgiTendon_H_


