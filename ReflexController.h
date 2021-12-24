#ifndef OPENSIM_ReflexController_H_
#define OPENSIM_ReflexController_H_
/* -------------------------------------------------------------------------- *
 *                      OpenSim: ReflexController.h                              *
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
#include "osimReflexControllerDLL.h"
#include "OpenSim/Simulation/Control/Controller.h"
#include "OpenSim/Simulation/Model/Muscle.h"



namespace OpenSim {


// Forward declarations of classes that are used by the ReflexController
class SimpleSpindle;
class GolgiTendon;



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
class OSIMREFLEXCONTROLLER_API ReflexController : public Controller {
OpenSim_DECLARE_CONCRETE_OBJECT(ReflexController, Controller);

public:
//=============================================================================
// PROPERTIES
//=============================================================================
    
    OpenSim_DECLARE_PROPERTY(normalized_rest_length, double,
        "The intended rest length of the spindle");
    OpenSim_DECLARE_PROPERTY(gain_length, double,
    "The factor by which the stretch reflex is scaled.");
    OpenSim_DECLARE_PROPERTY(gain_velocity, double, "The factor by which the stretch reflex speed is scaled");
    OpenSim_DECLARE_LIST_PROPERTY(spindle_list, std::string, "The list of model spindles that this controller will depend upond for control");
        OpenSim_DECLARE_LIST_PROPERTY(golgi_list, std::string, "The list of model golgi-tendons that this controller will depend upond for control");

//==============================================================================
// SOCKETS
//==============================================================================
    
//=============================================================================
// METHODS
//=============================================================================
    //--------------------------------------------------------------------------
    // CONSTRUCTION AND DESTRUCTION
    //--------------------------------------------------------------------------
    /** Default constructor. */
    ReflexController();
    ReflexController(const std::string& name,
                     double rest_length,
                     double gain_l,
                     double gain_v);

    // Uses default (compiler-generated) destructor, copy constructor and copy 
    // assignment operator.
    
    //--------------------------------------------------------------------------
    // Controller Interface
    //--------------------------------------------------------------------------
    
    /** replace the current set of spindles with the provided set*/
    void setSpindles(const Set<SimpleSpindle>& spindles);
    /**add the current set of spindles*/
    void addSpindle(const SimpleSpindle& spindle);
    /**get a const refernece to the current set of spindles*/
    const Set<const SimpleSpindle>& getSpindleSet() const;
    /** get a writable reference to the set of const spindles for this controllerr*/
    Set<const SimpleSpindle>& updSpindles();
    

    /** replace the current set of golgi-tendons with the provided set*/
    void setGolgis(const Set<GolgiTendon>& golgis);
    /**add the current set of golgi-tendons*/
    void addGolgi(const GolgiTendon& golgi);
    /**get a const refernece to the current set of golgi-tendons*/
    const Set<const GolgiTendon>& getGolgiSet() const;
    /** get a writable reference to the set of const gogli-tendons for this controllerr*/
    Set<const GolgiTendon>& updGolgis();
    

    /** Compute the controls for stretch reflex
     *  This method defines the behavior of the stretch reflex
     *
     * @param s         system state 
     * @param controls  writable model controls
     */
    void computeControls(const SimTK::State& s,
                         SimTK::Vector &controls) const override;


private:
    // Connect properties to local pointers.  */
    void constructProperties();
    // ModelComponent interface to connect this component to its model
    void extendConnectToModel(Model& aModel) override;

    // the set of Model spindles that this controller controls
    Set<const SimpleSpindle> _spindleSet;
    
    Set<const GolgiTendon> _golgiSet;
    
protected:
    double _normalizedRestLength;
    //=========================================================================
};  // END of class ReflexController

}; //namespace
//=============================================================================
//=============================================================================

#endif // OPENSIM_ReflexController_H_


