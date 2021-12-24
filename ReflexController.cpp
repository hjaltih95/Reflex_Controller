/* -------------------------------------------------------------------------- *
 *                      OpenSim:  ReflexController.cpp                           *
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
#include "ReflexController.h"
#include <OpenSim/OpenSim.h>
#include "OpenSim/Simulation/Model/Muscle.h"
#include "SimpleSpindle.h"
#include "GolgiTendon.h"


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
                                   double rest_length,
                                   double gain_l,
                                   double gain_v)
{
    OPENSIM_THROW_IF(name.empty(), ComponentHasNoName, getClassName());
       
    setName(name);
    
    constructProperties();
    set_normalized_rest_length(rest_length);
    set_gain_length(gain_l);
    set_gain_velocity(gain_v);
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
    constructProperty_gain_length(1.0);
    constructProperty_gain_velocity(1.0);
    constructProperty_spindle_list();
    constructProperty_golgi_list();
    
    _spindleSet.setMemoryOwner(false);
    _golgiSet.setMemoryOwner(false);

}


void ReflexController::extendConnectToModel(Model &model)
{
    Super::extendConnectToModel(model);
    
    _spindleSet.setMemoryOwner(false);
    _spindleSet.setSize(0);
    
    int nac = getProperty_spindle_list().size();
    if (nac == 0)
        return;
    
    auto spindles = model.getComponentList<SimpleSpindle>();
    if (IO::Uppercase(get_spindle_list(0)) == "ALL") {
        for (auto& spindle : spindles) {
            _spindleSet.adoptAndAppend(&spindle);
        }
        return;
    }
    
    else {
        for (int i = 0; i < nac; i++) {
            bool found = false;
            for (auto& spindle : spindles) {
                if (get_spindle_list(i) == spindle.getName()) {
                    _spindleSet.adoptAndAppend(&spindle);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cerr << "WARN: ReflexController::connectToModel : SimpleSpindle "
                << get_spindle_list(i) <<
                " was not found and will be ignored." << endl;
            }
        }
    }
    
    _golgiSet.setMemoryOwner(false);
    _golgiSet.setSize(0);
    
    int nac1 = getProperty_golgi_list().size();
    if (nac1 == 0)
        return;
    
    auto golgis = model.getComponentList<GolgiTendon>();
    if (IO::Uppercase(get_golgi_list(0)) == "ALL") {
        for (auto& golgi : golgis) {
            _golgiSet.adoptAndAppend(&golgi);
        }
        return;
    }
    
    else {
        for (int i = 0; i < nac1; i++) {
            bool found = false;
            for (auto& golgi : golgis) {
                if (get_golgi_list(i) == golgi.getName()) {
                    _golgiSet.adoptAndAppend(&golgi);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cerr << "WARN: ReflexController::connectToModel : GolgiTendon "
                << get_golgi_list(i) <<
                " was not found and will be ignored." << endl;
            }
        }
    }
    
    
    
    Set<const Actuator>& actuators = updActuators();

    
    
    int cnt=0;
    
    while(cnt < actuators.getSize()){
        const Muscle *musc = dynamic_cast<const Muscle*>(&actuators[cnt]);
        // control muscles only
        if(!musc){
            log_warn("ReflexController assigned a non-muscle actuator '{}', "
                    "which will be ignored.", actuators[cnt].getName());
            actuators.remove(cnt);
        }else
            cnt++;
    }
}

//=============================================================================
// GET AND SET
//=============================================================================

void ReflexController::setSpindles(const Set<SimpleSpindle>& spindles)
{
    _spindleSet.setMemoryOwner(false);
    
    _spindleSet.setSize(0);
    updProperty_spindle_list().clear();
    for (int i = 0; i<spindles.getSize(); i++) {
        addSpindle(spindles[i]);
    }
}

void ReflexController::addSpindle(const SimpleSpindle& spindle) {
    _spindleSet.adoptAndAppend(&spindle);
    
    int found = updProperty_spindle_list().findIndex(spindle.getName());
    if (found < 0)
        updProperty_spindle_list().appendValue(spindle.getName());
}

Set <const SimpleSpindle>& ReflexController::updSpindles() { return _spindleSet; }

const Set<const SimpleSpindle>& ReflexController::getSpindleSet() const { return _spindleSet; }

void ReflexController::setGolgis(const Set<GolgiTendon>& golgis)
{
    _golgiSet.setMemoryOwner(false);
    updProperty_golgi_list().clear();
    for (int i = 0; i < golgis.getSize(); i++) {
        addGolgi(golgis[i]);
    }
}
void ReflexController::addGolgi(const GolgiTendon& golgi)
{
    _golgiSet.adoptAndAppend(&golgi);
    
    int found = updProperty_golgi_list().findIndex(golgi.getName());
    if (found < 0)
        updProperty_golgi_list().appendValue(golgi.getName());
}

Set <const GolgiTendon>& ReflexController::updGolgis() { return _golgiSet; }

const Set< const GolgiTendon>& ReflexController::getGolgiSet() const { return _golgiSet; }

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

    double t_o = 1;
    double f_o = 1;
    double stretch = 0;
    //reflex control
    double control = 0;
    double length = 0;
    double speed = 0;
    double max_speed = 0;
    double k_l = get_gain_length();
    double k_v = get_gain_velocity();
    
    
    
    const Set<const SimpleSpindle>& spindles = getSpindleSet();
    const Set<const GolgiTendon>& golgis = getGolgiSet();
    
    // make a for loop for all the spindles and golgi tendon (we assume that the reflex controller employs the same muscles with both a golgi-tendon organ and a spindle)
    for (int i = 0; i< spindles.getSize(); i++) {
        const SimpleSpindle& spindle = spindles.get(i);
        const GolgiTendon& golgi = golgis.get(i);
        
        stretch = spindle.getSpindleLength(s);
        speed = spindle.getSpindleSpeed(s);
        length = golgi.getTLength(s);

        const Muscle& musc = spindle.getMuscle();
    
        f_o = musc.getOptimalFiberLength();
        t_o = musc.getTendonSlackLength();
        max_speed = f_o*musc.getMaxContractionVelocity();
        
    
        control = 0.5*k_l*(fabs(stretch)+stretch)/f_o;
        control += 0.5*k_v*(fabs(speed)+speed)/max_speed;
        control += 0.5*k_l*(fabs(length)+length)/t_o;
        

        SimTK::Vector actControls(1,control);
        // add reflex controls to whatever controls are already in place.
        // make a member function to get the refernce of the spindles that have the referneces to the muscles
        musc.addInControls(actControls, controls);
    }
        
}

